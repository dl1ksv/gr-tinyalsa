/* -*- c++ -*- */
/* 
 * Copyright 2015 Volker Schroer.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef ANDROID
#include <android/log.h>
#endif

#include <gnuradio/io_signature.h>
#include "talsa_sink_impl.h"

namespace gr {
  namespace tinyalsa {

    talsa_sink::sptr
    talsa_sink::make(int sample_rate,int card,int device)
    {
      return gnuradio::get_initial_sptr
        (new talsa_sink_impl(sample_rate, card, device));
    }

    /*
     * The private constructor
     */
    talsa_sink_impl::talsa_sink_impl(int sampling_rate, int card, int device)
      : gr::sync_block("talsa_sink",
              gr::io_signature::make(2, 2, sizeof(float)),
              gr::io_signature::make(0, 0, 0)),
	      d_sampling_rate(sampling_rate),
	      d_card(card),
	      d_device(device)
	      
  {
  	uint rate;
  	struct pcm_params *params;
 	params = NULL ;
        d_pcm_handle = 0;

 	params = pcm_params_get(card, device, PCM_OUT);
	if( params == NULL )
            bail("talsa_sink_impl: Could not get card parameter. Does the card exists ?");
        // Check requested sample rate
	rate = pcm_params_get_min(params,PCM_PARAM_RATE);
        if( d_sampling_rate < rate )
            bail("talsa_sink_impl: sample rate too low");
        rate = pcm_params_get_max(params,PCM_PARAM_RATE);
        if(rate < d_sampling_rate)
            bail("talsa_sink_impl: sample rate too high");
        // To be done: more checks
        pcm_params_free(params);
        config.channels = 2;
        config.rate = d_sampling_rate;
        config.period_size = 1024;
        config.period_count = 4;
        config.format = PCM_FORMAT_S16_LE;
        config.start_threshold = 0;
        config.stop_threshold = 0;
        config.silence_threshold = 0;

        d_pcm_handle = pcm_open(card, device, PCM_OUT, &config);
        if (!d_pcm_handle || !pcm_is_ready(d_pcm_handle))
            bail(boost::str(boost::format("talsa_sink_impl::talsa_sink_impl: Unable to open PCM device %1%") % pcm_get_error(d_pcm_handle) ).c_str());
        d_buffer_size = pcm_frames_to_bytes(d_pcm_handle, pcm_get_buffer_size(d_pcm_handle)); // channels * period_size *period_count *2 -- wg. 16 bit
        d_buffer =NULL;
        d_buffer=new char[d_buffer_size];
        if (d_buffer == NULL)
            bail(boost::str(boost::format("talsa_sink_impl::talsa_sink_impl: Unable to create Buffer[ %1% ]") % d_buffer_size ).c_str());
        else
            output_msg(boost::str(boost::format("talsa_sink_impl::talsa_sink_impl: Created Buffer[ %1% ]") % d_buffer_size ).c_str());
  }

    /*
     * Our virtual destructor.
     */
    talsa_sink_impl::~talsa_sink_impl()
    {
        delete [] d_buffer;
    }

    int
    talsa_sink_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {   int n,bi,ret,count,items;
        const float **in = (const float **) &input_items[0];
        int16_t *buf = (int16_t *) d_buffer;
        static const float scale_factor = std::pow(2.0f, 16-1) - 1;
        assert(d_buffer_size == config.period_size * 4);
        count = 0;
        items=noutput_items/config.period_size;
//output_msg(boost::str(boost::format("talsa_sink_impl::work: to process %1% samples, channels %2%,period %3%") % noutput_items % config.channels %config.period_size).c_str());
        for(n = 0; n < items; n ++) {
          // process one period of data period
          bi = 0;
          for(unsigned int i = 0; i < config.period_size; i++) {
            for (unsigned int chan = 0; chan < config.channels; chan++) {
              buf[bi++] = (int16_t ) (in[chan][i] * scale_factor);
            }
          }
          count += config.period_size;

          // update src pointers
          for(unsigned int chan = 0; chan < config.channels; chan++)
            in[chan] += config.period_size;
          ret=pcm_write(d_pcm_handle,d_buffer,2*bi);
          if(ret != 0) {
            output_msg(boost::str(boost::format("talsa_sink_impl::work: Error writing buffer %1%") % pcm_get_error(d_pcm_handle) ).c_str());
            return -1; // No fixing this problem.  Say we're done.
          }
        }
//output_msg(boost::str(boost::format("talsa_sink_impl::work: processed %1% samples,n %2%") % count %n).c_str());
        return count;
    }
    void
    talsa_sink_impl::output_msg (const char *msg)
    {
      GR_LOG_ERROR(d_logger,  msg);
    }

    void
    talsa_sink_impl::bail(const char *msg) throw (std::runtime_error)
    {
      output_msg(msg);
      throw std::runtime_error("talsa_sink");
    }

  } /* namespace tinyalsa */
} /* namespace gr */


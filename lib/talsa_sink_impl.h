/* -*- c++ -*- */
/* 
 * Copyright 2015 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_TINYALSA_TALSA_SINK_IMPL_H
#define INCLUDED_TINYALSA_TALSA_SINK_IMPL_H

#include <tinyalsa/talsa_sink.h>
extern "C" {
#include "asoundlib.h"
}
namespace gr {
  namespace tinyalsa {

    class talsa_sink_impl : public talsa_sink
    {
     private:
      // Nothing to declare in this block.

     public:
      talsa_sink_impl(int sampling_rate,int card,int device);
      ~talsa_sink_impl();
      void output_msg(const char *msg);
      void bail(const char *msg) throw (std::runtime_error);

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
               gr_vector_void_star &output_items);
     private:
     int d_card;
     int d_device;
     int d_sampling_rate;
     int d_buffer_size;
     char * d_buffer;
     struct pcm_config config;
     struct pcm *d_pcm_handle;
    };

  } // namespace tinyalsa
} // namespace gr

#endif /* INCLUDED_TINYALSA_TALSA_SINK_IMPL_H */


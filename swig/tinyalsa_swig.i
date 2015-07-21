/* -*- c++ -*- */

#define TINYALSA_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "tinyalsa_swig_doc.i"

%{
#include "tinyalsa/talsa_sink.h"
%}


%include "tinyalsa/talsa_sink.h"
GR_SWIG_BLOCK_MAGIC2(tinyalsa, talsa_sink);

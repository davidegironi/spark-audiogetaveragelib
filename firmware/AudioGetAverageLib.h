//********************************************************************
// AudioGetAverageLib for Spark
// http://github.com/davidegironi/spark-audiogetaveragelib
// Copyright (c) Davide Gironi, 2014 
//
// References:
//   http://davidegironi.blogspot.it/2013/05/avr-atmega-audio-input-root-mean-square.html
//
// Released under GPLv3.
// Please refer to LICENSE file for licensing information.
//********************************************************************

#ifndef __AUDIOGETAVERAGE_H
#define __AUDIOGETAVERAGE_H

// BIAS setup ----------------
//enable or disable dynamic bias
//with dynamic bias the zero raw bias is estimated using max and min raw value read
#define AUDIOGETAVERAGE_DYNAMICBIAS 0
//zero value, raw value read when there is no sound
#define AUDIOGETAVERAGE_BIASZERORAW 2048

// RMS setup ----------------
//define the smooth factor for the getval value (from 0:more filter to 64:less filter)
#define AUDIOGETAVERAGE_SMOOTHFILTERVAL 30

// TYPE Average setup ----------------
//number of samples to take for measure
#define AUDIOGETAVERAGE_SAMPLES 32
//sample interval time (microseconds), defines the sample rate
#define AUDIOGETAVERAGE_SAMPLESINTERVALMICROSEC 250 //4Khz
//define the correction value for rms radix calculation
#define AUDIOGETAVERAGE_CORRECTION 8

#include "application.h"

class AudioGetAverageLib {
    public:
        AudioGetAverageLib(short channel);
        int getRms();
        int getPeakToPeak();
        int getSpl(double voltnow, double voltref, double dbref);
    private:
        short _channel;
};

#endif
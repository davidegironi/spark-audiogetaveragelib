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

#include "AudioGetAverageLib.h"

#include <math.h>

/**
 * Setup and initialize library
 */
AudioGetAverageLib::AudioGetAverageLib(short channel) {
    _channel = channel;
}

/**
 * Get RMS value from audio signal
 * signal must be biased to ADVREF voltage / 2
 */
int AudioGetAverageLib::getRms() {
	static int16_t ret = 0;
	
	#if AUDIOGETAVERAGE_DYNAMICBIAS == 1
	int16_t biasadc = 0;
	int16_t biasadcMin = 4096;
	int16_t biasadcMax = 0;
	#endif
	
	int adcRaw = 0;
	
    //get value using avarage method
	int32_t rmssquaresum = 0;
    
    //samplerate counter
    unsigned long samplerateCounter = micros();

	//root mean square to measure value
	for(uint8_t i=0; i<AUDIOGETAVERAGE_SAMPLES; i++) {
		adcRaw = analogRead(_channel);
	    #if AUDIOGETAVERAGE_DYNAMICBIAS == 1
		if(adcRaw < biasadcMin)
			biasadcMin = adcRaw;
		if(adcRaw > biasadcMax)
			biasadcMax = adcRaw;
		biasadc = (biasadcMin + (biasadcMax-biasadcMin)/2);
		adcRaw = adcRaw - biasadc; //signal is now biased in the ADC center
		#else
		adcRaw = adcRaw - AUDIOGETAVERAGE_BIASZERORAW;
		#endif
		
		rmssquaresum += ((int32_t)adcRaw * (int32_t)adcRaw);

	    //wait for the next sample to get read, guarantee the sample rate
		while (micros()-samplerateCounter < AUDIOGETAVERAGE_SAMPLESINTERVALMICROSEC) ;
	    samplerateCounter = micros(); //update interval timer
	    
	}
	//root
	int16_t valsqrt = sqrt(rmssquaresum/AUDIOGETAVERAGE_SAMPLES) - AUDIOGETAVERAGE_CORRECTION;

	//smooth filter
	ret = ((long)(64-AUDIOGETAVERAGE_SMOOTHFILTERVAL)*ret+(long)AUDIOGETAVERAGE_SMOOTHFILTERVAL*valsqrt)>>6;

    if(ret<0)
		ret = 0;
	
	return ret;
}

/*
 * Get Spl value,
 * depending on actual voltage read, voltage at a db reference, the db reference
 */
int AudioGetAverageLib::getSpl(double voltnow, double voltref, double dbref) {
    int ret = (20 * log10(voltnow/voltref)) + dbref;
	return ret;
}
/*
 * ofBaseClock.h
 *
 *  Created on: 05/09/2012
 *      Author: arturo
 */

#ifndef OFBASECLOCK_H_
#define OFBASECLOCK_H_

class ofBaseClock{
public:
	virtual ~ofBaseClock(){

	}

	virtual void start()=0;
	virtual unsigned long getElapsedTimeMicros()=0;
	virtual unsigned long getLastFrameMicros()=0;
	virtual unsigned long getSystemTimeMicros()=0;
	virtual void reset()=0;
	virtual void update()=0;

	static const int MillisPerSecond=1000;
	static const int MicrosPerSecond=1000000;
	static const double SecondsPerMillis=1./1000.;
	static const double SecondsPerMicros=1./1000000.;
	static const int MicrosPerMillis=1000;
	static const double MillisPerMicro=1./1000.;


	virtual double getElapsedTimeSeconds(){
		return getElapsedTimeMicros() * SecondsPerMicros;
	}

	virtual unsigned long getElapsedTimeMillis(){
		return getElapsedTimeMicros() * MillisPerMicro;
	}


	virtual double getLastFrameSeconds(){
		return getLastFrameMicros() * SecondsPerMicros;
	}

	virtual unsigned long getLastFrameMillis(){
		return getLastFrameMicros() * MillisPerMicro;
	}


	virtual double getSystemTimeSeconds(){
		return getSystemTimeMicros()*SecondsPerMicros;
	}

	virtual unsigned long getSystemTimeMillis(){
		return getSystemTimeMicros()*MillisPerMicro;
	}


};


#endif /* OFBASECLOCK_H_ */

/*
 * ofTimeUtils.h
 *
 *  Created on: 05/09/2012
 *      Author: arturo
 */

#ifndef OFTIMEUTILS_H_
#define OFTIMEUTILS_H_

#include "ofSoundStream.h"
#include "ofBaseClock.h"

class ofSystemClock: public ofBaseClock{
public:
	ofSystemClock();

	void start();

	unsigned long getElapsedTimeMicros();
	unsigned long getLastFrameMicros();
	unsigned long getSystemTimeMicros();

	void reset();
	void update();

private:
	unsigned long startTime, lastFrameTime, prevFrameTime;
};

class ofFrameClock: public ofBaseClock{
public:
	ofFrameClock();

	void start();

	void setFrameRate(double fps);
	double getFrameRate();

	unsigned long getElapsedTimeMicros();
	unsigned long getLastFrameMicros();
	unsigned long getSystemTimeMicros();

	void reset();
	void update();

private:
	unsigned long startTime, lastFrameTime, prevFrameTime, systemTimeMicros;
	double fps;

};

class ofSoundStreamClock: public ofBaseClock{
public:
	ofSoundStreamClock();

	void start();
	void setSoundStream(ofSoundStream & soundStream);

	unsigned long getElapsedTimeMicros();
	unsigned long getLastFrameMicros();
	unsigned long getSystemTimeMicros();

	void reset();
	void update();

private:
	unsigned long startTime, lastFrameTime, prevFrameTime;
	ofSoundStream * soundStream;
};
#endif /* OFTIMEUTILS_H_ */

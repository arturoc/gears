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
#include "ofSoundPlayer.h"

class ofSystemClock: public ofBaseClock{
public:
	ofSystemClock();

	void start();

	long unsigned long getElapsedTimeMicros();
	long unsigned long getLastFrameMicros();
	long unsigned long getSystemTimeMicros();

	void reset();
	void update();

private:
	long unsigned long startTime, lastFrameTime, prevFrameTime;
};

class ofFrameClock: public ofBaseClock{
public:
	ofFrameClock();

	void start();

	void setFrameRate(double fps);
	double getFrameRate();

	long unsigned long getElapsedTimeMicros();
	long unsigned long getLastFrameMicros();
	long unsigned long getSystemTimeMicros();

	void reset();
	void update();
	void setFrame(u_long frame); // manually update the clock for testing;
	u_long getFrame();

private:
	long unsigned long startTime, lastFrameTime, prevFrameTime, systemTimeMicros, frame;
	double fps;

};

class ofSoundStreamClock: public ofBaseClock{
public:
	ofSoundStreamClock();

	void start();
	void setSoundStream(ofPtr<ofBaseSoundStream> soundStream);

	long unsigned long getElapsedTimeMicros();
	long unsigned long getLastFrameMicros();
	long unsigned long getSystemTimeMicros();

	void reset();
	void update();

private:
	long unsigned long startTime, lastFrameTime, prevFrameTime;
	ofPtr<ofBaseSoundStream> soundStream;
};

class ofSoundPlayerClock: public ofBaseClock{
public:
	ofSoundPlayerClock();

	void start();
	void setSoundPlayer(ofBaseSoundPlayer & soundPlayer, int offsetMicros=0);

	long unsigned long getElapsedTimeMicros();
	long unsigned long getLastFrameMicros();
	long unsigned long getSystemTimeMicros();

	void update();
	void reset();

private:
	long long startTime, lastFrameTime, prevFrameTime;
	ofBaseSoundPlayer * soundPlayer;
	int offsetMicros;
};
#endif /* OFTIMEUTILS_H_ */

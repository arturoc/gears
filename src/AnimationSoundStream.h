/*
 * AnimationSoundStream.h
 *
 *  Created on: Sep 16, 2012
 *      Author: arturo
 */

#ifndef ANIMATIONSOUNDSTREAM_H_
#define ANIMATIONSOUNDSTREAM_H_

#include "ofBaseClock.h"
#include "ofBaseApp.h"
#include "ofBaseTypes.h"
#include "ofConstants.h"
#include "ofBaseSoundStream.h"

class AnimationSoundStream: public ofBaseSoundStream {
public:
	AnimationSoundStream();

	void listDevices();

	void setDeviceID(int deviceID);

	void update();

	bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
	void setInput(ofBaseSoundInput * soundInput);
	void setOutput(ofBaseSoundOutput * soundOutput);
	bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
	void setClock(ofBaseClock & _clock);

	void start();
	void stop();
	void close();

	long unsigned long getTickCount();
	int getNumInputChannels();
	int getNumOutputChannels();
	int getSampleRate();
	int getBufferSize();

private:
	int outChannels;
	int inChannels;
	int sampleRate;
	int bufferSize;
	int nBuffers;
	long unsigned long tickCount;
	ofBaseClock * clock;
	ofBaseSoundInput * soundInput;
	ofBaseSoundOutput * soundOutput;
	u_int timeRemainder;
	vector<float> bufferOut, bufferIn;
};

#endif /* ANIMATIONSOUNDSTREAM_H_ */

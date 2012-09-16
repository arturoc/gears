/*
 * AnimationSoundStream.cpp
 *
 *  Created on: Sep 16, 2012
 *      Author: arturo
 */

#include "AnimationSoundStream.h"

AnimationSoundStream::AnimationSoundStream() {
	outChannels = 2;
	inChannels = 0;
	sampleRate = 44100;
	bufferSize = 256;
	nBuffers = 4;
	tickCount = 0;
	timeRemainder = 0;
	soundInput = NULL;
	soundOutput = NULL;
	clock = NULL;
}

void AnimationSoundStream::listDevices(){

}

void AnimationSoundStream::setDeviceID(int deviceID){

}

void AnimationSoundStream::setClock(ofBaseClock & _clock){
	clock = &_clock;
}

void AnimationSoundStream::update(){
	unsigned long frameTime = clock->getLastFrameMicros() + timeRemainder;
	int oneBufferTime = double(bufferSize)*ofBaseClock::MicrosPerSecond/sampleRate;
	int numBuffers = frameTime/oneBufferTime;
	timeRemainder = frameTime - numBuffers*oneBufferTime;
	for(int i=0;i<numBuffers;i++){
		if(outChannels && soundOutput) soundOutput->audioOut(&bufferOut[0],bufferSize,outChannels,0,tickCount);
		if(inChannels && soundInput) soundInput->audioIn(&bufferIn[0],bufferSize,inChannels,0,tickCount);
	}
}


bool AnimationSoundStream::setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers){
	soundOutput = app;
	soundInput = app;
	return setup(outChannels,inChannels,sampleRate,bufferSize,nBuffers);
}

void AnimationSoundStream::setInput(ofBaseSoundInput * _soundInput){
	soundInput = _soundInput;
}

void AnimationSoundStream::setOutput(ofBaseSoundOutput * _soundOutput){
	soundOutput = _soundOutput;
}

bool AnimationSoundStream::setup(int _outChannels, int _inChannels, int _sampleRate, int _bufferSize, int _nBuffers){
	outChannels = _outChannels;
	inChannels = _inChannels;
	sampleRate = _sampleRate;
	bufferSize = _bufferSize;
	nBuffers = _nBuffers;
	tickCount = 0;
	bufferOut.resize(outChannels*bufferSize);
	bufferIn.resize(inChannels*bufferSize);
	return true;
}


void AnimationSoundStream::start(){
	tickCount = 0;
}

void AnimationSoundStream::stop(){

}

void AnimationSoundStream::close(){

}


long unsigned long AnimationSoundStream::getTickCount(){
	return tickCount;
}

int AnimationSoundStream::getNumInputChannels(){
	return inChannels;
}

int AnimationSoundStream::getNumOutputChannels(){
	return outChannels;
}

int AnimationSoundStream::getSampleRate(){
	return sampleRate;
}

int AnimationSoundStream::getBufferSize(){
	return bufferSize;
}

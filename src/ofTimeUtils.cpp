/*
 * ofTimeUtils.cpp
 *
 *  Created on: 05/09/2012
 *      Author: arturo
 */

#include "ofTimeUtils.h"
#include "ofConstants.h"

#if defined(TARGET_OF_IPHONE) || defined(TARGET_OSX ) || defined(TARGET_LINUX)
	#include <sys/time.h>
#endif

#ifdef TARGET_WIN32
    #include <mmsystem.h>
	#ifdef _MSC_VER
		#include <direct.h>
	#endif

#endif


ofSystemClock::ofSystemClock() {
	startTime = 0;
	prevFrameTime = 0;
	lastFrameTime = 0;
}

void ofSystemClock::start(){
	reset();
}

long unsigned long ofSystemClock::getElapsedTimeMicros(){
	return getSystemTimeMicros() - startTime;
}

long unsigned long ofSystemClock::getLastFrameMicros(){
	return lastFrameTime;
}

long unsigned long ofSystemClock::getSystemTimeMicros(){
	#ifndef TARGET_WIN32
		struct timeval now;
		gettimeofday( &now, NULL );
		return now.tv_usec + now.tv_sec*1000000;
	#else
		#if defined(_WIN32_WCE)
			return GetTickCount()*1000;
		#else
			return timeGetTime()*1000;
		#endif
	#endif
}

void ofSystemClock::reset(){
	startTime = getSystemTimeMicros();
	prevFrameTime = startTime;
	lastFrameTime = 0;
}

void ofSystemClock::update(){
	unsigned long now = getSystemTimeMicros();
	lastFrameTime = now-prevFrameTime;
	prevFrameTime = now;
}







ofFrameClock::ofFrameClock() {
	startTime = 0;
	prevFrameTime = 0;
	lastFrameTime = 0;
	systemTimeMicros = 0;
	fps = 60;
}

void ofFrameClock::start(){
	reset();
}

long unsigned long ofFrameClock::getElapsedTimeMicros(){
	return getSystemTimeMicros() - startTime;
}

long unsigned long ofFrameClock::getLastFrameMicros(){
	return lastFrameTime;
}

long unsigned long ofFrameClock::getSystemTimeMicros(){
	return systemTimeMicros;
}

void ofFrameClock::reset(){
	startTime = 0;
	prevFrameTime = startTime;
	lastFrameTime = startTime;
	systemTimeMicros = startTime;
}

void ofFrameClock::update(){
	frame++;
	setFrame(frame);
}

void ofFrameClock::setFrame(u_long frame){
	systemTimeMicros = double(ofBaseClock::MicrosPerSecond)/fps*frame;
	lastFrameTime = systemTimeMicros-prevFrameTime;
	prevFrameTime = systemTimeMicros;
}

u_long ofFrameClock::getFrame(){
	return frame;
}

void ofFrameClock::setFrameRate(double _fps){
	fps = _fps;
}

double ofFrameClock::getFrameRate(){
	return fps;
}







ofSoundStreamClock::ofSoundStreamClock() {
	startTime = 0;
	prevFrameTime = 0;
	lastFrameTime = 0;
}

void ofSoundStreamClock::start(){
	if(!soundStream){
		ofLogError() << "you need to set a soundStream before starting this clock";
	}
	reset();
}

void ofSoundStreamClock::setSoundStream(ofPtr<ofBaseSoundStream> _soundStream){
	soundStream = _soundStream;
}

long unsigned long ofSoundStreamClock::getElapsedTimeMicros(){
	return getSystemTimeMicros() - startTime;
}

long unsigned long ofSoundStreamClock::getLastFrameMicros(){
	return lastFrameTime;
}

long unsigned long ofSoundStreamClock::getSystemTimeMicros(){
	//cout << soundStream->getTickCount() <<"*" << soundStream->getBufferSize() << soundStream->getSampleRate() << endl;
	return double(soundStream->getTickCount()*soundStream->getBufferSize()*ofBaseClock::MicrosPerSecond)/double(soundStream->getSampleRate());
}

void ofSoundStreamClock::reset(){
	startTime = 0;
	prevFrameTime = startTime;
	lastFrameTime = startTime;
}

void ofSoundStreamClock::update(){
	unsigned long now = getSystemTimeMicros();
	lastFrameTime = now-prevFrameTime;
	prevFrameTime = now;
}



ofSoundPlayerClock::ofSoundPlayerClock() {
	startTime = 0;
	prevFrameTime = 0;
	lastFrameTime = 0;
	offsetMicros = 0;
	soundPlayer = 0;
}

void ofSoundPlayerClock::start(){
	if(!soundPlayer){
		ofLogError() << "you need to set a soundStream before starting this clock";
	}
	reset();
}

void ofSoundPlayerClock::setSoundPlayer(ofBaseSoundPlayer & _soundPlayer, int _offsetMicros){
	soundPlayer = &_soundPlayer;
	offsetMicros = _offsetMicros;
}

long unsigned long ofSoundPlayerClock::getElapsedTimeMicros(){
	return ((long long)getSystemTimeMicros()) > startTime ? getSystemTimeMicros() - startTime : 0;
}

long unsigned long ofSoundPlayerClock::getLastFrameMicros(){
	return lastFrameTime;
}

long unsigned long ofSoundPlayerClock::getSystemTimeMicros(){
	//cout << soundStream->getTickCount() <<"*" << soundStream->getBufferSize() << soundStream->getSampleRate() << endl;
	return soundPlayer->getPositionMS()*MicrosPerMillis;
}


void ofSoundPlayerClock::update(){
	unsigned long now = getSystemTimeMicros();
	lastFrameTime = now-prevFrameTime;
	prevFrameTime = now;
}

void ofSoundPlayerClock::reset(){
	startTime = offsetMicros;
	prevFrameTime = offsetMicros;
	lastFrameTime = offsetMicros;
}


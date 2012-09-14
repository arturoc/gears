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

unsigned long ofSystemClock::getElapsedTimeMicros(){
	return getSystemTimeMicros() - startTime;
}

unsigned long ofSystemClock::getLastFrameMicros(){
	return lastFrameTime;
}

unsigned long ofSystemClock::getSystemTimeMicros(){
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

unsigned long ofFrameClock::getElapsedTimeMicros(){
	return getSystemTimeMicros() - startTime;
}

unsigned long ofFrameClock::getLastFrameMicros(){
	return lastFrameTime;
}

unsigned long ofFrameClock::getSystemTimeMicros(){
	return systemTimeMicros;
}

void ofFrameClock::reset(){
	startTime = 0;
	prevFrameTime = startTime;
	lastFrameTime = startTime;
	systemTimeMicros = startTime;
}

void ofFrameClock::update(){
	systemTimeMicros += double(ofBaseClock::MicrosPerSecond)/fps;
	lastFrameTime = systemTimeMicros-prevFrameTime;
	prevFrameTime = systemTimeMicros;
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
	soundStream = NULL;
}

void ofSoundStreamClock::start(){
	if(!soundStream){
		ofLogError() << "you need to set a soundStream before starting this clock";
	}
	reset();
}

void ofSoundStreamClock::setSoundStream(ofSoundStream & _soundStream){
	soundStream = &_soundStream;
}

unsigned long ofSoundStreamClock::getElapsedTimeMicros(){
	return getSystemTimeMicros() - startTime;
}

unsigned long ofSoundStreamClock::getLastFrameMicros(){
	return lastFrameTime;
}

unsigned long ofSoundStreamClock::getSystemTimeMicros(){
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


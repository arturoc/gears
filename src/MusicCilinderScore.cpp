/*
 * MusicCilinderScore.cpp
 *
 *  Created on: Sep 7, 2012
 *      Author: arturo
 */

#include "MusicCilinderScore.h"
#include "ofFileUtils.h"

MusicCilinderScore::MusicCilinderScore() {
	clock = NULL;
	currentEvent = 0;
}

MusicCilinderScore::~MusicCilinderScore() {
	// TODO Auto-generated destructor stub
}

void MusicCilinderScore::setup(ofBaseClock & _clock){
	clock = &_clock;
	currentEvent = 0;
}

void MusicCilinderScore::update(){
	if(currentEvent==audioEvents.size()-1) return;
	if(clock->getElapsedTimeMillis()>audioEvents[currentEvent].timeMillis){
		currentEvent++;
	}
}

MusicCilinderScore::AudioEvent & MusicCilinderScore::getNextEvent(){
	return audioEvents[currentEvent];
}

void MusicCilinderScore::load(string path){
	ofFile file(path);
	while(file.good()){
		AudioEvent event;
		file >> event.timeMillis;
		file >> event.freq;
		audioEvents.push_back(event);
	}
}

void MusicCilinderScore::save(string path){
	ofFile file(path,ofFile::WriteOnly);
	for(u_int i=0;i<audioEvents.size();i++){
		file << audioEvents[i].timeMillis;
		file << " ";
		file << audioEvents[i].freq;
		file << "\n";
	}
}

/*
 * MusicCilinder.cpp
 *
 *  Created on: 05/09/2012
 *      Author: arturo
 */

#include "MusicCilinder.h"

ofBaseClock * MusicCilinder::clock=0;
ofSoundStreamClock * MusicCilinder::audioClock=0;

MusicCilinder::MusicCilinder() {
	mom = NULL;
	lastEvent = 0;
}

MusicCilinder::~MusicCilinder() {
	// TODO Auto-generated destructor stub
}


void MusicCilinder::setup(string path, string name){
	Model::setup(path,name);
	parameters.add(radius.set("radius",0.055,0.04,0.2));
	parameters.add(rotZVel.set("rotZVel",0,-360,360));
	parameters.add(boxSize.set("boxSize",0.01,0,0.01));
	rot = ofVec3f(0,90,0);
	pos = ofVec3f(0,0,300);
	score.setup(*audioClock);
	score.load("score.sc");
}

void MusicCilinder::connectTo(Gear & gear){
	mom = &gear;
}

void MusicCilinder::update(){
	if(mom){
		pos = ofVec3f(mom->pos->x ,mom->pos->y ,pos->z);
		rotZVel = mom->rotZVel * mom->rotZFactor;
	}
	rot = ofVec3f(rot->x,rot->y, rot->z + rotZVel * clock->getLastFrameSeconds());
	if(rot->z>360) rot = ofVec3f(rot->x,rot->y,rot->z - 360);

	score.update();
	float now = clock->getElapsedTimeSeconds();
	MusicCilinderScore::AudioEvent event = score.getNextEvent();
	if(event.timeMillis>lastEvent){
		notes.push_back(Note(ofMap(event.freq,0,127,-0.085,0.085),rot->z,now));
		lastEvent = event.timeMillis;
	}

	for(u_int i=0;i<notes.size();i++){
		float time_diff = now - notes[i].startTime;
		notes[i].pct = ofClamp(time_diff,0,1);
	}
}

void MusicCilinder::renderContour(){
	Model::renderContour();
	ofNoFill();
	for(u_int i=0;i<notes.size();i++){
		ofPushMatrix();
		ofRotateX(notes[i].rot);
		ofBox(ofPoint(notes[i].pos,radius-(1-notes[i].pct)*boxSize),boxSize);
		ofPopMatrix();
	}
}

void MusicCilinder::renderFill(){
	Model::renderFill();
	ofFill();
	for(u_int i=0;i<notes.size();i++){
		ofPushMatrix();
		ofRotateX(notes[i].rot);
		ofBox(ofPoint(notes[i].pos,radius-(1-notes[i].pct)*boxSize),boxSize);
		ofPopMatrix();
	}
}

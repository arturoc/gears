/*
 * MusicCilinder.cpp
 *
 *  Created on: 05/09/2012
 *      Author: arturo
 */

#include "MusicCilinder.h"

ofBaseClock * MusicCilinder::audioClock=0;
ofBasicSoundPlayer * MusicCilinder::soundPlayer=0;

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
	parameters.add(length.set("length",0.085*2,0.001,0.085));
	rot = ofVec3f(0,90,0);
	pos = ofVec3f(0,0,300);
	score.setup(*audioClock);
	score.load("score.sc");
}

void MusicCilinder::setup(ofxFBXMesh & mesh){
	Shaft::setup(mesh);
	parameters.add(radius.set("radius",5,0.04,10));
	parameters.add(boxSize.set("boxSize",.75,0,2));
	parameters.add(length.set("length",8.5*2,0.1,8.5));
	score.setup(*audioClock);
	score.load("score.sc");
}

void MusicCilinder::update(){
	Shaft::update();

	float zrot = 0;
	if(fbxMesh){
		ofQuaternion zrotq;
		zrotq.makeRotate(rot->z,ofVec3f(1,0,0));
		ofQuaternion q = fbxMesh->getNode().getOrientationQuat();
		zrotq *= q;
		fbxMesh->getNode().setOrientation(zrotq);
		zrot  = -zrotq.getEuler().z+50;


		score.update();
		float now = clock->getElapsedTimeSeconds();
		MusicCilinderScore::AudioEvent event = score.getNextEvent();

		if(event.timeMillis>lastEvent){
			float pos = ofMap(event.freq,50,127,-length*.5,length*.5);
			notes.push_back(Note(pos,zrot,now));
			lastEvent = event.timeMillis;
		}

		for(u_int i=0;i<notes.size();i++){
			float time_diff = now - notes[i].startTime;
			notes[i].pct = ofClamp(time_diff,0,3);
		}

		if(audioClock->getElapsedTimeSeconds()<24) wave.push_back(Amplitude(soundPlayer->getCurrentBuffer().getRMSAmplitude()*30,zrot,now));
	}


}

void MusicCilinder::renderContour(){
	float now = clock->getElapsedTimeSeconds();
	Model::renderContour();
	ofNoFill();
	for(u_int i=0;i<notes.size();i++){
		if(notes[i].pct<=1) ofSetColor(200,150+105*notes[i].pct*notes[i].pct);
		else if(notes[i].pct<=3) ofSetColor(200,255*(3-(notes[i].pct))*(3-(notes[i].pct)));
		else continue;
		ofPushMatrix();
		ofRotateX(notes[i].rot);
		ofBox(ofPoint(notes[i].pos,radius-(1-ofClamp(notes[i].pct,0,1))*boxSize),boxSize);
		ofPopMatrix();
	}
	ofSetColor(color);
}

void MusicCilinder::renderFill(){
	Model::renderFill();
	ofFill();
	for(u_int i=0;i<notes.size();i++){
		ofPushMatrix();
		if(notes[i].pct<=1) ofSetColor(200,255*notes[i].pct*notes[i].pct);
		else if(notes[i].pct<=3) ofSetColor(200,255*(3-(notes[i].pct))*(3-(notes[i].pct)));
		else continue;
		ofRotateX(notes[i].rot);
		ofBox(ofPoint(notes[i].pos,radius-(1-ofClamp(notes[i].pct,0,1))*boxSize),boxSize);
		ofPopMatrix();
	}
}

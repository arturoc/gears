/*
 * MusicCilinder.cpp
 *
 *  Created on: 05/09/2012
 *      Author: arturo
 */

#include "MusicCilinder.h"

ofBaseClock * MusicCilinder::clock=0;
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
	Model::setup(mesh);
	parameters.add(radius.set("radius",5,0.04,10));
	parameters.add(rotZVel.set("rotZVel",0,-360,360));
	parameters.add(boxSize.set("boxSize",.75,0,2));
	parameters.add(length.set("length",8.5*2,0.1,8.5));
	score.setup(*audioClock);
	score.load("score.sc");
}

void MusicCilinder::connectTo(Gear & gear){
	mom = &gear;
}

void MusicCilinder::update(){
	if(mom){
		pos = ofVec3f(mom->pos->x ,mom->pos->y ,pos->z);
		rotZVel = -mom->rotZVel * mom->rotZFactor;
		//cout << getName() << " " << rotZVel << endl;
	}
	rot = ofVec3f(rot->x,rot->y, rot->z + rotZVel * clock->getLastFrameSeconds());
	if(rot->z>360) rot = ofVec3f(rot->x,rot->y,rot->z - 360);
	if(fbxMesh){
		fbxMesh->setTime(clock->getElapsedTimeMillis());
	}
	ofQuaternion zrot;
	zrot.makeRotate(rot->z,ofVec3f(1,0,0));
	ofQuaternion q = fbxMesh->getNode().getOrientationQuat();
	zrot *= q;
	fbxMesh->getNode().setOrientation(zrot);


	score.update();
	float now = clock->getElapsedTimeSeconds();
	MusicCilinderScore::AudioEvent event = score.getNextEvent();
	float rot = -zrot.getEuler().z+90;
	if(event.timeMillis>lastEvent){
		float pos = ofMap(event.freq,50,127,-length*.5,length*.5);
		//if(notes.empty() || abs(notes.back().rot-(-zrot.getEuler().z+90))>10 || abs(notes.back().pos-pos)>10){
			notes.push_back(Note(pos,rot,now));
		//}
		lastEvent = event.timeMillis;
	}

	for(u_int i=0;i<notes.size();i++){
		float time_diff = now - notes[i].startTime;
		notes[i].pct = ofClamp(time_diff,0,3);
	}

	if(audioClock->getElapsedTimeSeconds()<24) wave.push_back(Amplitude(soundPlayer->getCurrentBuffer().getRMSAmplitude()*30,rot,now));
}

void MusicCilinder::renderContour(){
	float now = clock->getElapsedTimeSeconds();
	Model::renderContour();
	ofNoFill();
	for(u_int i=0;i<notes.size();i++){
		if(notes[i].pct<=1) ofSetColor(200,150+105*notes[i].pct*notes[i].pct);
		else ofSetColor(200,255*(3-(notes[i].pct))*(3-(notes[i].pct)));
		ofPushMatrix();
		ofRotateX(notes[i].rot);
		ofBox(ofPoint(notes[i].pos,radius-(1-ofClamp(notes[i].pct,0,1))*boxSize),boxSize);
		ofPopMatrix();
	}
	/*ofPoint prev;
	for(u_int i=0;i<wave.size();i++){
		float life = now-wave[i].timeCreated;
		if(life>3) continue;
		ofPushMatrix();
		ofSetColor(200,255.*(1-life/3.));
		ofPoint p(length*.5-wave[i].amp*length*.1,radius,0);
		p.rotate(wave[i].rot,ofVec3f(1,0,0));
		if(i>0){
			ofLine(prev,p);
		}
		prev=p;
		ofPopMatrix();
	}*/
	ofSetColor(color);
}

void MusicCilinder::renderFill(){
	Model::renderFill();
	ofFill();
	for(u_int i=0;i<notes.size();i++){
		ofPushMatrix();
		if(notes[i].pct<=1) ofSetColor(200,255*notes[i].pct*notes[i].pct);
		else ofSetColor(200,255*(3-(notes[i].pct))*(3-(notes[i].pct)));
		ofRotateX(notes[i].rot);
		ofBox(ofPoint(notes[i].pos,radius-(1-ofClamp(notes[i].pct,0,1))*boxSize),boxSize);
		ofPopMatrix();
	}
}

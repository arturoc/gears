/*
 * Shaft.cpp
 *
 *  Created on: Sep 17, 2012
 *      Author: arturo
 */

#include "Shaft.h"

Shaft::Shaft() {
	mom = NULL;
}

void Shaft::setup(ofxFBXMesh & mesh){
	Model::setup(mesh);
	parameters.add(rotZVel.set("rotZVel",0,-360,360));
}

void Shaft::connectTo(Gear & gear){
	mom = &gear;
	ofLogNotice() << "connected " << getName() << " to " << gear.getName();
}

void Shaft::update(){
	Model::update();
	if(mom){
		pos = ofVec3f(mom->pos->x ,mom->pos->y ,pos->z);
		rotZVel = -mom->rotZVel * mom->rotZFactor;
	}

	rot = ofVec3f(rot->x,rot->y, rot->z + rotZVel * clock->getLastFrameSeconds());
	if(rot->z>360) rot = ofVec3f(rot->x,rot->y,rot->z - 360);

}



/*
 * Gear.cpp
 *
 *  Created on: 12/08/2012
 *      Author: arturo
 */

#include "Gear.h"

ofBaseClock * Gear::clock;

Gear::Gear() {
	mom = NULL;
	connectionAngle = 0;
}

void Gear::setup(string path, string name){
	Model::setup(path,name);
	parameters.add(rotZVel.set("rotZVel",0,-360,360));
	parameters.add(offsetx.set("offset_x",-200,-500,-400));
	parameters.add(offsetRotz.set("offsetRotz",0,-180,180));
	parameters.add(rotZFactor.set("rotZFactor",1,-10,10));
}

void Gear::update(){
	if(mom){
		pos = ofVec3f(mom->pos->x + offsetx*sin(connectionAngle),mom->pos->y + offsetx*cos(connectionAngle),mom->pos->z);
		rotZVel = mom->rotZVel;
	}
	rot = ofVec3f(rot->x,rot->y, rot->z + rotZVel * rotZFactor * clock->getLastFrameSeconds());
	if(rot->z>360) rot = ofVec3f(rot->x,rot->y,rot->z - 360);
}

void Gear::connectTo(Gear & gear, float angle){
	mom = &gear;
	connectionAngle = ofDegToRad(angle);
}

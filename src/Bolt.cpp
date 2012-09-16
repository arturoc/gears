/*
 * Bolt.cpp
 *
 *  Created on: Sep 15, 2012
 *      Author: arturo
 */

#include "Bolt.h"
ofBaseClock * Bolt::clock;

Bolt::Bolt() {
	mom = NULL;
	yPos = 0;
	yVel = 0;
}

void Bolt::setup(ofxFBXMesh & mesh){
	Model::setup(mesh);
	parameters.add(yVel.set("yVel",0,0,1));
	parameters.add(yPos.set("yPos",0,0,100));
}

void Bolt::update(){
	if(mom){
		yVel = mom->rotZVel/360.*5;
	}
	yPos -= yVel*clock->getLastFrameSeconds();
	if(fbxMesh){
		fbxMesh->setTime(clock->getElapsedTimeMillis());
	}
	fbxMesh->getNode().move(0,yPos,0);
}

void Bolt::connectTo(Gear & gear){
	mom = &gear;
}

/*
 * Bolt.cpp
 *
 *  Created on: Sep 15, 2012
 *      Author: arturo
 */

#include "Bolt.h"
#include "scene.h"

Bolt::Bolt() {
	mom = NULL;
	yPos = 0;
	yVel = 0;
}

void Bolt::setup(ofxFBXMesh & mesh){
	Model::setup(mesh);
	parameters.add(yVel.set("yVel",0,0,1));
	parameters.add(yPos.set("yPos",0,-10,0));
}

void Bolt::update(){
	Model::update();
	/*if(mom){
		yVel = mom->rotZVel/360.*5;
	}
	yPos -= yVel*clock->getLastFrameSeconds();*/

#ifdef SCENE_1
	if(audioClock->getElapsedTimeMillis()>68000){
		yPos = ofMap(audioClock->getElapsedTimeMillis(),68000,70000,0,1)*ofMap(audioClock->getElapsedTimeMillis(),68000,70000,0,1)*-10;
		fbxMesh->getNode().move(0,yPos,0);
	}
#endif
}

void Bolt::connectTo(Gear & gear){
	mom = &gear;

	ofLogNotice() << "connected " << getName() << " to " << gear.getName();
}

/*
 * Gear.cpp
 *
 *  Created on: 12/08/2012
 *      Author: arturo
 */

#include "Gear.h"
#include "Shaft.h"

#include "scene.h"

ofBasicSoundPlayer * Gear::soundPlayer;

Gear::Gear() {
	mom = NULL;
	shaft = NULL;
	connectionAngle = 0;
}

void Gear::setup(string path, string name){
	Model::setup(path,name);
	parameters.add(rotZVel.set("rotZVel",0,-360,360));
	parameters.add(offsetx.set("offset_x",-200,-500,-400));
	parameters.add(offsetRotz.set("offsetRotz",0,-180,180));
	parameters.add(rotZFactor.set("rotZFactor",1,-10,10));
}

void Gear::setup(ofxFBXMesh & mesh){
	Model::setup(mesh);
	parameters.add(rotZVel.set("rotZVel",0,-360,360));
	parameters.add(offsetx.set("offset_x",-200,-500,-400));
	parameters.add(offsetRotz.set("offsetRotz",0,-180,180));
	parameters.add(rotZFactor.set("rotZFactor",1,-10,10));
	originalVertices = fbxMesh->getVertices();
	originalIndices = fbxMesh->getIndices();
}

void Gear::update(){
	Model::update();
	if(mom){
		pos = ofVec3f(mom->pos->x + offsetx*sin(connectionAngle),mom->pos->y + offsetx*cos(connectionAngle),mom->pos->z);
		rotZVel = mom->rotZVel;
		rotZFactor = -mom->rotZFactor * double(mom->reduction)/double(reduction);
	}
	if(shaft){
		pos = ofVec3f(shaft->pos->x,shaft->pos->y,shaft->pos->z);
		rotZVel = shaft->rotZVel;
		rotZFactor = 1;
	}

	rot = ofVec3f(rot->x,rot->y, rot->z + rotZVel * rotZFactor * clock->getLastFrameSeconds());
	if(rot->z>360) rot = ofVec3f(rot->x,rot->y,rot->z - 360);
	ofQuaternion zrot;
	if(rotAxis==0){
		zrot.makeRotate(rot->z+offsetRotz,ofVec3f(1,0,0));
	}else if(rotAxis==1){
		zrot.makeRotate(rot->z+offsetRotz,ofVec3f(0,1,0));
	}else if(rotAxis==-1){
		zrot.makeRotate(rot->z+offsetRotz,ofVec3f(0,-1,0));
	}else if(rotAxis==2){
		zrot.makeRotate(rot->z+offsetRotz,ofVec3f(0,0,1));
	}else if(rotAxis==-2){
		zrot.makeRotate(rot->z+offsetRotz,ofVec3f(0,0,-1));
	}
	ofQuaternion q = fbxMesh->getNode().getOrientationQuat();
	zrot *= q;
	fbxMesh->getNode().setOrientation(zrot);

	if(soundPlayer->getPositionMS()>25500){
		float amp = soundPlayer->getCurrentBuffer().getRMSAmplitude();
		/*for(int i=0;i<fbxMesh->getIndices().size();i++){
			if(ofRandom(1)>1-amp) fbxMesh->getIndices()[i]=fbxMesh->getIndices()[ofRandom(0,fbxMesh->getIndices().size()-1)];//max((int)(i-amp*100),0),min((int)(i+amp*100),(int)fbxMesh->getIndices().size()-1))];
			else fbxMesh->getIndices()[i]=fbxMesh->getIndices()[i]*.8+originalIndices[i]*.2;
		}*/

#ifdef SCENE_0
		if(ofRandom(1)>.8){
			for(u_int i=0;i<fbxMesh->getVertices().size();i++){
				if(ofRandom(1)>.6) fbxMesh->getVertices()[i]= originalVertices[i]+amp*40*ofVec3f(0,0,(int)ofRandom(-2,2));
				else fbxMesh->getVertices()[i]= originalVertices[i];
			}
		}else{
			fbxMesh->getVertices() = originalVertices;
			fbxMesh->getNode().move(ofVec3f(amp*10*ofSignedNoise(clock->getElapsedTimeSeconds()+getNode().getGlobalPosition().x,clock->getElapsedTimeSeconds()+fbxMesh->getNode().getGlobalPosition().y)
						,amp*5*ofSignedNoise(clock->getElapsedTimeSeconds()+getNode().getGlobalPosition().y,clock->getElapsedTimeSeconds()+fbxMesh->getNode().getGlobalPosition().x),
						0));
		}
#endif


		/*ofQuaternion zrot;
		if(rotAxis==0){
			zrot.makeRotate(amp*100,ofVec3f(0,1,0));
		}else if(rotAxis==1){
			zrot.makeRotate(amp*100,ofVec3f(0,0,1));
		}else{
			zrot.makeRotate(amp*100,ofVec3f(1,0,0));
		}
		ofQuaternion q = fbxMesh->getNode().getOrientationQuat();
		zrot *= q;
		fbxMesh->getNode().setOrientation(zrot);*/
	}
}

void Gear::connectTo(Gear & gear, float angle){
	mom = &gear;
	if(shaft) shaft=NULL;
	connectionAngle = ofDegToRad(angle);

	ofLogNotice() << "connected " << getName() << " to " << gear.getName();
}

void Gear::connectTo(Shaft & _shaft){
	shaft = &_shaft;
	if(mom) mom=NULL;
	ofLogNotice() << "connected " << getName() << " to " << _shaft.getName();
}

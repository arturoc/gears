/*
 * WaveStripe.cpp
 *
 *  Created on: Sep 7, 2012
 *      Author: arturo
 */

#include "WaveStripe.h"

ofBaseClock * WaveStripe::clock=NULL;

WaveStripe::WaveStripe() {
	// TODO Auto-generated constructor stub

}

void WaveStripe::setup(){
	Model::setup("","wave");
	mesh.setUsage(GL_STREAM_DRAW);
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	scale = 1;
	lineArt = false;
	wireFacesAlpha = 0;
	mesh.getVertices().resize(1024*2);
	parameters.add(thickness.set("thickness",2,1,30));
	parameters.add(ampFactor.set("ampFactor",100,50,1000));
	for(int i=0; i<1024;i++){
		ofVec3f current(i*15,0,0);
		ofVec3f next((i+1)*15,0,0);
		ofVec3f up(0, 1, 0);
		ofVec3f &p0 = current;
		ofVec3f &p1 = next;

		ofVec3f dir = (p1 - p0).normalize();			// normalized direction vector from p0 to p1
		ofVec3f right = dir.cross(up).normalize();	// right vector
		right *= thickness;//thisThickness;
		mesh.getVertices()[i*2] = current - right;
		mesh.getVertices()[i*2+1] = current + right;
	}
}

void WaveStripe::update(){
	mutex.lock();
	amplitudesLeftFront.insert(amplitudesLeftFront.end(),amplitudesLeftBack.begin(),amplitudesLeftBack.end());
	amplitudesRightFront.insert(amplitudesRightFront.end(),amplitudesRightBack.begin(),amplitudesRightBack.end());
	amplitudesLeftBack.clear();
	amplitudesRightBack.clear();
	mutex.unlock();
	if(amplitudesLeftFront.size()>1024){
		amplitudesLeftFront.erase(amplitudesLeftFront.begin(),amplitudesLeftFront.begin()+(amplitudesLeftFront.size()-1024));
		amplitudesRightFront.erase(amplitudesRightFront.begin(),amplitudesRightFront.begin()+(amplitudesRightFront.size()-1024));
	}
	for(u_int i = 0; i<amplitudesLeftFront.size();i++){
		amplitudesLeftFront[i]*=.95;
		amplitudesRightFront[i]*=.95;
	}
	for(int i=0; i<((int)amplitudesLeftFront.size())-1;i++){
		ofVec3f current(i*thickness,amplitudesLeftFront[amplitudesLeftFront.size()-i]*ampFactor,0);
		ofVec3f currentR(i*thickness,amplitudesRightFront[amplitudesRightFront.size()-i]*ampFactor,0);
		ofVec3f next((i+1)*thickness,amplitudesLeftFront[amplitudesLeftFront.size()-(i+1)]*ampFactor,0);
		ofVec3f nextR((i+1)*thickness,amplitudesRightFront[amplitudesRightFront.size()-(i+1)]*ampFactor,0);
		ofVec3f up(0, 1, 0);

		ofVec3f dir = (next - current).normalize();			// normalized direction vector from p0 to p1
		ofVec3f right = dir.cross(up).normalize();	// right vector
		right *= thickness;//thisThickness;
		dir = (nextR - currentR).normalize();
		ofVec3f rightR = dir.cross(up).normalize();	// right vector
		rightR *= thickness;//thisThickness;


		mesh.getVertices()[i*2] = mesh.getVertices()[i*2]*.8 + (current - right)*.2;
		mesh.getVertices()[i*2+1] = mesh.getVertices()[i*2+1]*.8 + (currentR + rightR)*.2;
	}
	if(mesh.getVertices().size()>2){
		*(mesh.getVertices().end()-2) = *(mesh.getVertices().end()-4);
		*(mesh.getVertices().end()-1) = *(mesh.getVertices().end()-3);
	}

}


void WaveStripe::newSoundBuffer(ofSoundBuffer & buffer){
	float ampL = buffer.getRMSAmplitudeChannel(0);
	float ampR = buffer.getRMSAmplitudeChannel(1);
	mutex.lock();
	amplitudesLeftBack.push_back(ampL);
	amplitudesRightBack.push_back(ampR);
	mutex.unlock();
}

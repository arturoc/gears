/*
 * Counter.cpp
 *
 *  Created on: Sep 20, 2012
 *      Author: arturo
 */

#include "Counter.h"
#include "Poco/Timespan.h"
#include "Poco/DateTimeFormatter.h"

Counter::Counter() {
	// TODO Auto-generated constructor stub

}

Counter::~Counter() {
	// TODO Auto-generated destructor stub
}


void Counter::setup(ofxFBXMesh & fbxMesh){
	Model::setup(fbxMesh);
	sevenSegments.loadFont("MyBlueRoom.ttf",106,true,false,true);
	ofFbo::Settings settings;
	settings.width = 185*5;
	settings.height = 100*5;
	settings.numSamples = 8;
	fbo.allocate(settings);

	fbxMesh.addTexCoord(fbo.getTextureReference().getCoordFromPercent(0,0));//2
	fbxMesh.addTexCoord(fbo.getTextureReference().getCoordFromPercent(0,1));//3
	fbxMesh.addTexCoord(fbo.getTextureReference().getCoordFromPercent(1,0));//0
	fbxMesh.addTexCoord(fbo.getTextureReference().getCoordFromPercent(1,1));//1
}

void Counter::update(){
	Model::update();
	Poco::Timespan span(ofClamp(70000000-(long)audioClock->getElapsedTimeMicros(),0,70000000));
	now = Poco::DateTimeFormatter::format(span,"%M %S %i");
}

void Counter::renderContour(){
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glCullFace(GL_BACK);
	fbo.begin();
	ofClear(0,0);
	ofSetColor(200);
	ofNoFill();
	sevenSegments.drawStringAsShapes(now,90,220);
	fbo.end();
	fbo.getTextureReference().bind();
	fbxMesh->drawFaces();
	fbo.getTextureReference().unbind();
	glPopAttrib();
}

void Counter::renderFill(){
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glCullFace(GL_BACK);
	fbo.begin();
	ofClear(0,0);
	ofSetColor(180);
	sevenSegments.drawString(now,90,220);
	fbo.end();
	fbo.getTextureReference().bind();
	fbxMesh->drawFaces();
	fbo.getTextureReference().unbind();
	glPopAttrib();
}

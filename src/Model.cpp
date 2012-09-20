/*
 * Model.cpp
 *
 *  Created on: 05/09/2012
 *      Author: arturo
 */

#include "Model.h"

ofxAssimpModelLoader Model::modelLoader;
ofBaseClock * Model::clock;

Model::Model() {
	fbxMesh = NULL;

}

Model::~Model() {
	// TODO Auto-generated destructor stub
}



void Model::setup(string path, string name){
	if(path!=""){
		modelLoader.setScaleNomalization(false);
		modelLoader.loadModel(path);
		mesh = modelLoader.getMesh(0);
		mesh.getColors().clear();
		mesh.getTexCoords().clear();
	}
	parameters.setName(name);
	parameters.add(pos.set("pos",ofVec3f(0,0,0),ofVec3f(-ofGetWidth(),-ofGetHeight(),-10000),ofVec3f(+ofGetWidth(),+ofGetHeight(),10000)));
	parameters.add(rot.set("rot",ofVec3f(0,0,0),ofVec3f(-180,-180,0),ofVec3f(180,180,360)));
	parameters.add(color.set("color",ofColor(100,100,100),ofColor(ofColor::black,0),ofColor(ofColor::white,255)));
	parameters.add(lineWeight.set("lineWeight",3,1,10));
	parameters.add(wireFacesAlpha.set("wireFacesAlpha",1,0,1));
	parameters.add(lineArt.set("lineArt",true));
	parameters.add(scale.set("scale",1000,0.1,5000));
}

void Model::setup(ofxFBXMesh & _fbxMesh){
	fbxMesh = &_fbxMesh;
	parameters.setName(fbxMesh->getName());
	/*parameters.add(pos.set("pos",ofVec3f(0,0,0),ofVec3f(-ofGetWidth(),-ofGetHeight(),-10000),ofVec3f(+ofGetWidth(),+ofGetHeight(),10000)));*/
	parameters.add(rot.set("rot",ofVec3f(0,0,0),ofVec3f(-180,-180,0),ofVec3f(180,180,360)));
	parameters.add(color.set("color",ofColor(100,100,100),ofColor(ofColor::black,0),ofColor(ofColor::white,255)));
	parameters.add(lineWeight.set("lineWeight",3,1,10));
	parameters.add(wireFacesAlpha.set("wireFacesAlpha",1,0,1));
	parameters.add(drawWireframe.set("drawWireframe",true));
	parameters.add(lineArt.set("lineArt",true));
	parameters.add(scale.set("scale",1000,0.1,5000));
}

void Model::update(){
	/*if(fbxMesh){
		fbxMesh->setTime(clock->getElapsedTimeMillis());
	}*/
}

void Model::drawContour(){
	drawModel(lineArt);

}

void Model::drawFill(){
	drawModel(false);

}

void Model::drawModel(bool lineArt){
	//ofEnableLighting();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	if(lineArt) glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(OF_MESH_WIREFRAME));
	if(fbxMesh){
		fbxMesh->getNode().transformGL();
	}else{
		ofPushMatrix();
		ofTranslate(pos->x,pos->y,pos->z);
		ofScale(scale,scale,scale);
		ofRotateZ(rot->z);
		ofRotateY(rot->y);
		ofRotateX(rot->x);
	}
	if(lineArt){
		drawLineArt(lineWeight,ofColor(0,0),ofColor(color));
	}else{
		ofSetColor(color,wireFacesAlpha*255.);
		renderFill();
		ofSetColor(color,(1-wireFacesAlpha)*255.);
		renderContour();
	}
	if(fbxMesh){
		fbxMesh->getNode().restoreTransformGL();
	}else{
		ofPopMatrix();
	}
	glPopClientAttrib();
	glPopAttrib();
	//ofDisableLighting();
}


void Model::drawLineArt(float weight, ofColor fill, ofColor stroke) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	if(ofGetCoordHandedness()==OF_RIGHT_HANDED){
		//glPolygonMode(GL_FRONT,GL_FILL);
		glDepthFunc(GL_LESS);
		glCullFace(GL_BACK);
	}else{
		//glPolygonMode(GL_BACK,GL_FILL);
		glDepthFunc(GL_LESS);
		glCullFace(GL_FRONT);
	}
	ofSetColor(fill);
	renderFill();

	glLineWidth(weight);
	if(ofGetCoordHandedness()==OF_RIGHT_HANDED){
		//glPolygonMode(GL_BACK,GL_LINE);
		glDepthFunc(GL_LEQUAL);
		glCullFace(GL_FRONT);
	}else{
		//glPolygonMode(GL_FRONT,GL_LINE);
		glDepthFunc(GL_LEQUAL);
		glCullFace(GL_BACK);
	}
	ofSetColor(stroke);
	renderContour();
	glPopAttrib();
}


void Model::renderContour(){
	if(!drawWireframe) return;
	if(fbxMesh!=NULL){
		fbxMesh->drawWireframe();
	}
	else mesh.drawWireframe();
}

void Model::renderFill(){
	if(fbxMesh!=NULL){
		fbxMesh->drawFaces();
	}
	else mesh.drawFaces();
}

ofNode Model::getNode(){
	if(fbxMesh) return fbxMesh->getNode();
	else return ofNode();
}

string Model::getName(){
	if(fbxMesh) return ofSplitString(ofToLower(fbxMesh->getName()),"_")[0];
	else return "";
}

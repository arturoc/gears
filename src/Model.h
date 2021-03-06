/*
 * Model.h
 *
 *  Created on: 05/09/2012
 *      Author: arturo
 */

#ifndef MODEL_H_
#define MODEL_H_

#include "ofxFBX.h"
#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"
#include "ofConstants.h"
#include "ofTimeUtils.h"

class Model {
public:
	Model();
	virtual ~Model();

	virtual void setup(string path, string name);
	virtual void setup(ofxFBXMesh & fbxMesh);
	virtual void update();

	void drawContour();
	void drawFill();

	virtual void renderContour();
	virtual void renderFill();

	ofNode getNode();
	string getName();

	ofParameter<ofVec3f> rot;
	ofParameter<ofVec3f> pos;
	ofParameter<ofColor> color;
	ofParameter<float> lineWeight;
	ofParameter<float> wireFacesAlpha;
	ofParameter<bool> lineArt;
	ofParameter<float> scale;
	ofParameter<bool> drawWireframe;
	ofParameterGroup parameters;
	static ofBaseClock * clock;

protected:
	void drawLineArt(float weight = 3, ofColor fill = ofColor(255), ofColor stroke = ofColor(0));
	void drawModel(bool lineArt);
	static ofxAssimpModelLoader modelLoader;
	ofVboMesh mesh;
	ofxFBXMesh * fbxMesh;
};

#endif /* MODEL_H_ */

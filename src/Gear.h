/*
 * Gear.h
 *
 *  Created on: 12/08/2012
 *      Author: arturo
 */

#ifndef GEAR_H_
#define GEAR_H_

#include "Model.h"
#include "ofxAssimpModelLoader.h"
#include "ofParameter.h"
#include "ofBaseClock.h"

class Gear:public Model {
public:
	Gear();

	void setup(string path, string name);
	void setup(ofxFBXMesh & mesh);
	void update();

	void connectTo(Gear & gear, float angle);

	static ofBaseClock * clock;

	ofParameter<float> rotZVel;
	ofParameter<float> offsetRotz;
	ofParameter<float> offsetx;
	ofParameter<float> rotZFactor;
	ofParameter<int> rotAxis;

private:
	Gear * mom;
	float connectionAngle;
};

#endif /* GEAR_H_ */

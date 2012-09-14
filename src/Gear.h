/*
 * Gear.h
 *
 *  Created on: 12/08/2012
 *      Author: arturo
 */

#ifndef GEAR_H_
#define GEAR_H_

#include "ofxAssimpModelLoader.h"
#include "ofParameter.h"
#include "ofBaseClock.h"
#include "Model.h"

class Gear:public Model {
public:
	Gear();

	void setup(string path, string name);
	void update();

	void connectTo(Gear & gear, float angle);

	static ofBaseClock * clock;

	ofParameter<float> rotZVel;
	ofParameter<float> offsetRotz;
	ofParameter<float> offsetx;
	ofParameter<float> rotZFactor;

private:
	Gear * mom;
	float connectionAngle;
};

#endif /* GEAR_H_ */

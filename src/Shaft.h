/*
 * Shaft.h
 *
 *  Created on: Sep 17, 2012
 *      Author: arturo
 */

#ifndef SHAFT_H_
#define SHAFT_H_

#include "Model.h"
#include "Gear.h"
#include "ofParameter.h"

class Shaft: public Model {
public:
	Shaft();

	virtual void setup(ofxFBXMesh & mesh);
	virtual void update();

	virtual void connectTo(Gear & gear);


	ofParameter<float> rotZVel;

private:
	Gear * mom;
};

#endif /* SHAFT_H_ */

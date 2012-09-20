/*
 * Bolt.h
 *
 *  Created on: Sep 15, 2012
 *      Author: arturo
 */

#ifndef BOLT_H_
#define BOLT_H_


#include "Model.h"
#include "Gear.h"
#include "ofxAssimpModelLoader.h"
#include "ofParameter.h"
#include "ofBaseClock.h"

class Bolt: public Model {
public:
	Bolt();

	void setup(ofxFBXMesh & mesh);
	void update();

	void connectTo(Gear & gear);


	ofParameter<float> yVel;
	ofParameter<float> yPos;
	ofParameter<int> rotAxis;

	ofBaseClock * audioClock;


private:
	Gear * mom;
};

#endif /* BOLT_H_ */

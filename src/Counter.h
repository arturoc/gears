/*
 * Counter.h
 *
 *  Created on: Sep 20, 2012
 *      Author: arturo
 */

#ifndef COUNTER_H_
#define COUNTER_H_

#include "Model.h"
#include "ofTrueTypeFont.h"

class Counter: public Model {
public:
	Counter();
	virtual ~Counter();

	void setup(ofxFBXMesh & fbxMesh);
	void update();

	void renderContour();
	void renderFill();

	ofBaseClock * audioClock;

private:
	ofTrueTypeFont sevenSegments;
	string now;
	ofFbo fbo;
};

#endif /* COUNTER_H_ */

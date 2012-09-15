/*
 * WaveStripe.h
 *
 *  Created on: Sep 7, 2012
 *      Author: arturo
 */

#ifndef WAVESTRIPE_H_
#define WAVESTRIPE_H_

#include "Model.h"
#include "ofSoundBuffer.h"
#include "ofVboMesh.h"
#include "ofBaseClock.h"

class WaveStripe: public Model {
public:
	WaveStripe();

	void setup();

	void update();

	void newSoundBuffer(ofSoundBuffer & buffer);

	static ofBaseClock * clock;

	ofParameter<float> thickness;
	ofParameter<float> ampFactor;
private:
	vector<float> amplitudesLeftFront, amplitudesRightFront, amplitudesLeftBack, amplitudesRightBack;
	ofMutex mutex;
};

#endif /* WAVESTRIPE_H_ */

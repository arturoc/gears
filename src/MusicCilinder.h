/*
 * MusicCilinder.h
 *
 *  Created on: 05/09/2012
 *      Author: arturo
 */

#ifndef MUSICCILINDER_H_
#define MUSICCILINDER_H_

#include "Shaft.h"
#include "Gear.h"
#include "MusicCilinderScore.h"
#include "ofTimeUtils.h"

class MusicCilinder: public Shaft {
public:
	MusicCilinder();
	virtual ~MusicCilinder();

	void setup(string path, string name);
	void setup(ofxFBXMesh & mesh);
	void update();

	void renderContour();
	void renderFill();

	static ofBaseClock * audioClock;
	static ofBasicSoundPlayer * soundPlayer;

	ofParameter<float> radius;
	ofParameter<float> boxSize;
	ofParameter<float> length;

private:
	Gear * mom;
	MusicCilinderScore score;
	unsigned long lastEvent;
	struct Note{
		Note(float pos, float rot, float startTime):pos(pos),rot(rot),pct(0),startTime(startTime){}
		float pos;
		float rot;
		float pct;
		float startTime;
	};
	struct Amplitude{
		Amplitude(float amp, float rot, float timeCreated):amp(amp),rot(rot),timeCreated(timeCreated){}
		float amp;
		float rot;
		float timeCreated;
	};
	vector<Note> notes;
	vector<Amplitude> wave;
};

#endif /* MUSICCILINDER_H_ */

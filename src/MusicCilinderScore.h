/*
 * MusicCilinderScore.h
 *
 *  Created on: Sep 7, 2012
 *      Author: arturo
 */

#ifndef MUSICCILINDERSCORE_H_
#define MUSICCILINDERSCORE_H_

#include "ofConstants.h"
#include "ofBaseClock.h"

class MusicCilinderScore {
public:
	MusicCilinderScore();
	virtual ~MusicCilinderScore();

	void setup(ofBaseClock & clock);
	void update();

	void load(string path);
	void save(string path);

	struct AudioEvent{
		AudioEvent(){}
		AudioEvent(unsigned long time, int freq):timeMillis(time),freq(freq){}
		unsigned long timeMillis;
		int freq;
	};
	vector<AudioEvent> audioEvents;
	AudioEvent & getNextEvent();

private:
	ofBaseClock * clock;
	u_int currentEvent;
};

#endif /* MUSICCILINDERSCORE_H_ */

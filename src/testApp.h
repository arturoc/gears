#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"
#include "ofParameter.h"
#include "Glow.h"
#include "ofBasicSoundPlayer.h"
#include "ofxTimeline.h"
#include "Gear.h"
#include "ofBaseClock.h"
#include "ofTimeUtils.h"
#include "MusicCilinder.h"
#include "WaveStripe.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void connectionAngleChanged(float & angle);
		void pausePressed(bool & pressed);


		ofxPanel panel;
		ofParameter<float> lightx, lighty, lightz;
		ofParameter<int> fps;
		ofParameter<float> connectionAngle;
		ofParameter<float> cameraLatitude, cameraLongitude, orbitRadius;
		ofParameter<float> cameraTopY;
		ofParameter<float> cameraFov;
		ofParameter<bool> pause;

		ofLight light;

		Glow glow;
		ofBasicSoundPlayer player;
		ofxTimeline timeline;

		Gear gear1, gear2, gear3, gear4;
		Gear gearCinta;
		Gear gearDir;
		MusicCilinder musicCilinder;
		ofCamera camera,cameraTop,cameraFront,cameraLeft,cameraRight;
		ofSystemClock clock;
		ofSoundStreamClock audioClock;
		WaveStripe wave;
};

#pragma once

#include "ofxFBX.h"
#include "MusicCilinder.h"
#include "WaveStripe.h"
#include "Gear.h"
#include "ofMain.h"
#include "ofxGui.h"
#include "ofParameter.h"
#include "Glow.h"
#include "ofBasicSoundPlayer.h"
#include "ofBaseClock.h"
#include "ofTimeUtils.h"
#include "Bolt.h"
#include "AnimationSoundStream.h"

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
		
		Gear * getGear(string name);

		void pausePressed(bool & pressed);
		void resetTimePressed(bool & pressed);

		void facesWireframeAlphaChanged(float & wireframeAlpha);
		void lineWidthChanged(float & lineWeight);
		void soundPositionChanged(float & soundPosition);


		ofxPanel panel;
		ofParameter<float> lightx, lighty, lightz;
		ofParameter<int> fps;
		ofParameter<float> cameraLatitude, cameraLongitude, orbitRadius;
		ofParameter<float> cameraTopY;
		ofParameter<float> cameraFov;
		ofParameter<bool> pause;
		ofParameter<bool> autoAnimation;
		ofParameter<int> frame;
		ofParameter<float> globalFacesWireframeAlpha;
		ofParameter<float> globalLineWidth;
		ofParameter<int> blendMode;
		ofParameter<bool> drawAdditionalViews;
		ofParameter<bool> lightsOn;
		ofParameter<float> soundPosition;

		enum BlendMode{
			WireADDFacesALPHA,
			WireALPHAFacesADD,
			AllADD,
			AllAlpha
		};

		ofxButton resetTime;

		ofLight light;

		Glow glow;
		ofBasicSoundPlayer player;

		vector<Gear*> gears;
		vector<Model*> models;

		MusicCilinder musicCilinder;
		Bolt bolt;
		ofCamera cameraTop,cameraFront,cameraLeft,cameraRight;
		ofxFBXCamera * camera;
		ofFrameClock clock, cameraClock;
		ofSoundPlayerClock audioClock;
		WaveStripe wave;

		ofxFBXScene fbxScene;
		ofFbo renderFbo;
		ofPixels pixels;
		bool updatingSoundPos;
		ofPtr<AnimationSoundStream> animationSoundStream;
};

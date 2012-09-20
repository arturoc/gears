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
#include "Shaft.h"
#include "AnimationSoundStream.h"
#include "Counter.h"

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
		Shaft * getShaft(string name);
		Model * getModel(string name);

		void pausePressed(bool & pressed);
		void resetTimePressed(bool & pressed);

		void facesWireframeAlphaChanged(float & wireframeAlpha);
		void lineWidthChanged(float & lineWeight);
		void soundPositionChanged(float & soundPosition);


		ofxPanel panel;
		ofParameterGroup parameters;
		ofParameter<float> lightx, lighty, lightz;
		ofParameter<int> fps;
		ofParameter<float> cameraLatitude, cameraLongitude, orbitRadius;
		ofParameter<float> cameraTopY;
		ofParameter<float> cameraFov;
		ofParameter<float> farPlane, nearPlane;
		ofParameter<bool> pause;
		ofParameter<bool> autoAnimation;
		ofParameter<bool> fbxCamera;
		ofParameter<int> frame;
		ofParameter<float> globalFacesWireframeAlpha;
		ofParameter<float> globalLineWidth;
		ofParameter<int> blendMode;
		ofParameter<bool> drawAdditionalViews;
		ofParameter<bool> lightsOn;
		ofParameter<float> soundPosition;
		ofParameter<bool> saveOneFrame, record;
		ofParameter<ofVec3f> cameraUp, cameraOffset;

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
		vector<Shaft*> shafts;
		vector<Model*> models;

		MusicCilinder musicCilinder;
		Bolt bolt;
		Counter counter;

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

		ofShortPixels depthPixels;
		ofImage screen;

		ofNode counterCameraNode;
		ofVec3f cameraPositionOffsetOriginal;
};

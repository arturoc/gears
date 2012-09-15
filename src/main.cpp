#include "testApp.h"
#include "ofAppRunner.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
    window.setGlutDisplayString("double depth rgba samples>=8");
	ofSetupOpenGL(&window, 1600, 900, OF_FULLSCREEN);			// <-------- setup the GL context
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}

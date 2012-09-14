#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	Gear::clock = &clock;
	MusicCilinder::clock = &clock;
	MusicCilinder::audioClock = &audioClock;
	WaveStripe::clock = &clock;

	gear1.setup("engranajeGrande.obj","gear1");
	gear2.setup("engranajeChico.obj","gear2");
	gear3.setup("engranajeChico.obj","gear3");
	gear4.setup("engranajeGrande.obj","gear4");
	gearCinta.setup("gearCintas.obj","gearCinta");
	gearDir.setup("gearDir.obj","gearDir");
	musicCilinder.setup("musicalBox_.obj","musicCilinder");
	wave.setup();

	panel.setup("gui");
	panel.add(lightx.set("x_light",ofGetWidth()*.5,-ofGetWidth(),ofGetWidth()));
	panel.add(lighty.set("y_light",ofGetHeight()*.5,-ofGetHeight(),ofGetHeight()));
	panel.add(lightz.set("z_light",ofGetWidth()*.5,-ofGetWidth(),ofGetWidth()));
	panel.add(connectionAngle.set("connectionAngle",0,0,360));
	panel.add(fps.set("fps",60,0,300));
	panel.add(cameraLatitude.set("cameraLatitude",0,-180,180));
	panel.add(cameraLongitude.set("cameraLongitude",0,-180,180));
	panel.add(orbitRadius.set("orbitRadius",1000,-3000,3000));
	panel.add(cameraTopY.set("cameraTopY",3000,0,6000));
	panel.add(cameraFov.set("cameraFov",camera.getFov(),8,150));
	panel.add(glow.passes);
	panel.add(pause.set("pause",false));
	panel.add(gearCinta.parameters);
	panel.add(gearDir.parameters);
	panel.add(gear1.parameters);
	panel.add(gear2.parameters);
	panel.add(gear3.parameters);
	panel.add(gear4.parameters);
	panel.add(musicCilinder.parameters);
	panel.add(wave.parameters);
	panel.loadFromFile("settings.xml");

	connectionAngle.addListener(this,&testApp::connectionAngleChanged);
	pause.addListener(this,&testApp::pausePressed);

	gear1.rotZFactor = 1;
	gear1.offsetx = 0;

	gear2.rotZFactor = -15./13.;
	gear2.connectTo(gear1,connectionAngle);

	gear3.rotZFactor = -15./13.;
	gear3.offsetx = (float)gear2.offsetx;
	gear3.offsetRotz = (float)gear2.offsetRotz;
	gear3.connectTo(gear1,0);

	gear4.rotZFactor = 1;
	gear4.offsetx = (float)gear2.offsetx;
	gear4.connectTo(gear3,-30);

	camera.setFarClip(-15000);
	cameraTop.setFarClip(-15000);
	cameraTop.setPosition(0,cameraTopY,-1500);
	cameraTop.lookAt(gear1.pos);

	musicCilinder.connectTo(gear1);

	ofBackground(0);
	light.setup();
	light.enable();
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	glow.setup(1280,720);

	timeline.setup();
	timeline.setDurationInSeconds(10);
	timeline.setLoopType(OF_LOOP_NORMAL);
	timeline.addKeyframes("Rotate Z vel", "rotatez.xml", ofRange(-360, 360));
	timeline.addKeyframes("z", "z.xml", ofRange(0, -10000));


	//timeline.setOffset(ofVec2f(0,740));

	player.loadSound("07.Microseq.mp3",true);
	ofAddListener(player.newBufferE,&wave,&WaveStripe::newSoundBuffer);
	audioClock.setSoundStream(ofBasicSoundPlayer::getSoundStream());
	clock.start();
	audioClock.start();
	player.play();
	//ofSetFrameRate(60);
}

void testApp::connectionAngleChanged(float & angle){
	gear2.connectTo(gear1,connectionAngle);
}

void testApp::pausePressed(bool & pressed){
	if(pause) player.setPaused(pause);
	else player.play();
}

//--------------------------------------------------------------
void testApp::update(){
	clock.update();

	light.setPosition(lightx,lighty,lightz);
	const ofSoundBuffer & buffer = player.getCurrentBuffer();
	float avg=0;
	for(int i=0;i<(int)buffer.bufferSize();i++){
		avg += abs(buffer[i*buffer.getNumChannels()]);
	}
	avg/=buffer.bufferSize();
	gear1.rotZVel = avg*2000;
	gear1.update();
	gear2.update();
	gear3.update();
	gear4.update();
	gearCinta.update();
	gearDir.update();
	musicCilinder.update();

	wave.update();

	//rotVelZ = timeline.getKeyframeValue("Rotate Z vel");
	//z = timeline.getKeyframeValue("z");
	fps = ofGetFrameRate();

	camera.setFov(cameraFov);
	camera.lookAt(gear1.pos,ofVec3f(0,1,0));
	//cameraLongitude = gear1.rot->z*5;
	//cameraLatitude = gear1.rot->z*5;
	//while (cameraLatitude>90) cameraLatitude-=180;
	camera.orbit(cameraLongitude,cameraLatitude,orbitRadius,gear1.pos);

	cameraTop.resetTransform();
	cameraTop.setPosition(0,cameraTopY,gear1.pos->z);//gear1.pos.get().z);
	cameraTop.lookAt(ofVec3f(0,0,gear1.pos->z),ofVec3f(0,0,1));

	cameraFront.resetTransform();
	cameraFront.setPosition(0,0,gear1.pos->z-1000);//gear1.pos.get().z);
	cameraFront.lookAt(ofVec3f(0,0,gear1.pos->z),ofVec3f(0,1,0));

	cameraLeft.resetTransform();
	cameraLeft.setPosition(-1000,0,gear1.pos->z);//gear1.pos.get().z);
	cameraLeft.lookAt(ofVec3f(gear1.pos),ofVec3f(0,1,0));

	cameraRight.resetTransform();
	cameraRight.setPosition(1000,0,gear1.pos.get().z);//gear1.pos.get().z);
	cameraRight.lookAt(ofVec3f(gear1.pos),ofVec3f(0,1,0));
}

//--------------------------------------------------------------
void testApp::draw(){
	ofFill();
	ofSetColor(255);
	ofPushView();
	ofViewport(290,10,1280,720);
	ofBackgroundGradient(ofColor::gray,ofColor::black,OF_GRADIENT_CIRCULAR);
	ofPopView();

	glow.begin(false);
	camera.begin();
	//ofBackgroundGradient(ofColor(50),ofColor::black,OF_GRADIENT_CIRCULAR);
	gear1.drawFill();
	gear2.drawFill();
	gear3.drawFill();
	gear4.drawFill();
	musicCilinder.drawFill();
	wave.drawFill();
	gearCinta.drawFill();
	gearDir.drawFill();
	camera.end();
	glow.end();

	ofSetColor(255);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	glow.draw(290,720+10,1280,-720);

	camera.begin(ofRectangle(290,10,1280,720));
	gear1.drawContour();
	gear2.drawContour();
	gear3.drawContour();
	gear4.drawContour();
	musicCilinder.drawContour();
	wave.drawContour();
	gearCinta.drawContour();
	gearDir.drawContour();
	camera.end();


	ofEnableAlphaBlending();

	ofSetColor(0);
	ofFill();
	ofRect(290,10,1280/4,720/4);
	cameraTop.begin(ofRectangle(290,10,1280/4,720/4));
	gear1.drawContour();
	gear2.drawContour();
	gear3.drawContour();
	gear4.drawContour();
	musicCilinder.drawContour();
	wave.drawContour();
	gearCinta.drawContour();
	gearDir.drawContour();
	camera.draw();
	cameraTop.end();

	ofSetColor(0);
	ofFill();
	ofRect(290,10+720/4,1280/4,720/4);
	cameraFront.begin(ofRectangle(290,(720/4)+10,1280/4,720/4));
	gear1.drawContour();
	gear2.drawContour();
	gear3.drawContour();
	gear4.drawContour();
	musicCilinder.drawContour();
	wave.drawContour();
	gearCinta.drawContour();
	gearDir.drawContour();
	camera.draw();
	cameraFront.end();

	ofSetColor(0);
	ofFill();
	ofRect(290,10+(720/4)*2,1280/4,720/4);
	cameraLeft.begin(ofRectangle(290,(720/4)*2+10,1280/4,720/4));
	gear1.drawContour();
	gear2.drawContour();
	gear3.drawContour();
	gear4.drawContour();
	musicCilinder.drawContour();
	wave.drawContour();
	gearCinta.drawContour();
	gearDir.drawContour();
	camera.draw();
	cameraLeft.end();

	ofSetColor(0);
	ofFill();
	ofRect(290,10+(720/4)*3,1280/4,720/4);
	cameraRight.begin(ofRectangle(290,(720/4)*3+10,1280/4,720/4));
	gear1.drawContour();
	gear2.drawContour();
	gear3.drawContour();
	gear4.drawContour();
	musicCilinder.drawContour();
	wave.drawContour();
	gearCinta.drawContour();
	gearDir.drawContour();
	camera.draw();
	cameraRight.end();


	ofNoFill();
	ofSetLineWidth(1);
	ofSetColor(255);
	ofRect(290,10,1280,720);
	ofRect(290,10,1280/4,720/4);
	ofRect(290,10+720/4,1280/4,720/4);
	ofRect(290,10+(720/4)*2,1280/4,720/4);
	ofRect(290,10+(720/4)*3,1280/4,720/4);
	panel.draw();
	//timeline.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key==' '){
		timeline.play();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

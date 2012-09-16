#include "testApp.h"

Gear * testApp::getGear(string name){
	for(int i=0;i<gears.size();i++){
		if(gears[i]->getName()==name){
			return gears[i];
		}
	}
	return NULL;
}

//--------------------------------------------------------------
void testApp::setup(){
	clock.setFrameRate(30);
	cameraClock.setFrameRate(30);
	Gear::clock = &clock;
	MusicCilinder::clock = &clock;
	MusicCilinder::audioClock = &audioClock;
	MusicCilinder::soundPlayer = &player;
	WaveStripe::clock = &clock;
	Bolt::clock = &clock;

	fbxScene.load("A_camara_03_sintesis.fbx");

	wave.setup();
	globalFacesWireframeAlpha.addListener(this,&testApp::facesWireframeAlphaChanged);
	globalLineWidth.addListener(this,&testApp::lineWidthChanged);

	panel.setup("gui");
	panel.add(lightx.set("x_light",ofGetWidth()*.5,-ofGetWidth(),ofGetWidth()));
	panel.add(lighty.set("y_light",ofGetHeight()*.5,-ofGetHeight(),ofGetHeight()));
	panel.add(lightz.set("z_light",ofGetWidth()*.5,-ofGetWidth(),ofGetWidth()));
	panel.add(fps.set("fps",60,0,300));
	panel.add(cameraLatitude.set("cameraLatitude",0,-180,180));
	panel.add(cameraLongitude.set("cameraLongitude",0,-180,180));
	panel.add(orbitRadius.set("orbitRadius",1000,0,400));
	panel.add(globalFacesWireframeAlpha.set("globalFacesWireframeAlpha",1,0,1));
	panel.add(globalLineWidth.set("globalLineWidth",3,0,10));;
	panel.add(blendMode.set("blendMode",0,0,3));
	panel.add(cameraTopY.set("cameraTopY",3000,0,6000));
	panel.add(glow.passes);
	panel.add(pause.set("pause",false));
	panel.add(autoAnimation.set("autoAnimation",true));
	panel.add(frame.set("frame",0,0,300));
	panel.add(resetTime.setup("resetTime"));
	panel.add(soundPosition.set("soundPosition",0,0,225));
	panel.add(drawAdditionalViews.set("drawAdditionalViews",false));
	panel.add(lightsOn.set("lightsOn",false));


	panel.add(wave.parameters);
	for(u_int i=0;i<fbxScene.getMeshes().size();++i){
		vector<string> parsed = ofSplitString(ofToLower(fbxScene.getMeshes()[i]->getName()),"_");
		string name = parsed[0];
		string connection;
		int reduction=1;
		if(parsed.size()>1 && parsed[1].substr(0,2)=="to"){
			connection = parsed[1].substr(2);
		}else if(parsed.size()>2 && parsed[2].substr(0,2)=="to"){
			connection = parsed[2].substr(2);
			reduction = ofToInt(parsed[1]);
		}else if(parsed.size()>1){
			reduction = ofToInt(parsed[1]);
		}
		cout << name << " to " << connection << " reduction " << reduction;
		if(ofIsStringInString(name,"gear") ||
		   ofIsStringInString(name,"screw") ){
			Gear * gear = new Gear;
			gear->setup(*fbxScene.getMeshes()[i]);
			gear->reduction = reduction;
			gear->rotZFactor = 1;
			cout << " at " << gear->rotZFactor << endl;
			if(ofIsStringInString(name,"geardv")
				|| ofIsStringInString(name,"geardh")
				|| ofIsStringInString(name,"screw")){
				gear->rotAxis = 1;
			}else{
				gear->rotAxis = 2;
			}
			gears.push_back(gear);
			panel.add(gear->parameters);
		}else if(ofIsStringInString(name,"cilinder")){
			musicCilinder.setup(*fbxScene.getMeshes()[i]);
			cout << " at " << 1 << endl;
			panel.add(musicCilinder.parameters);
		}else if(ofIsStringInString(name,"tuerca")){
			bolt.setup(*fbxScene.getMeshes()[i]);
			bolt.rotAxis = 1;
			panel.add(bolt.parameters);

		}else if( ofIsStringInString(name,"tubo") ){
			Model * model = new Model;
			model->setup(*fbxScene.getMeshes()[i]);
			models.push_back(model);
			panel.add(model->parameters);
		}

	}

	for(u_int i=0;i<fbxScene.getMeshes().size();i++){
		vector<string> parsed = ofSplitString(ofToLower(fbxScene.getMeshes()[i]->getName()),"_");
		string name = parsed[0];
		string connection;
		int reduction=1;
		if(parsed.size()>1 && parsed[1].substr(0,2)=="to"){
			connection = parsed[1].substr(2);
		}else if(parsed.size()>2 && parsed[2].substr(0,2)=="to"){
			connection = parsed[2].substr(2);
		}else if(parsed.size()>1 && ofIsStringInString(name,"gear")){
			reduction = ofToInt(parsed[1]);
		}
		Gear * from = getGear(name);
		Gear * to = getGear(connection);
		if(from && !to){
			if(connection=="cilinder"){
				from->connectTo(musicCilinder);
			}
		}
		if(!from && name=="tuerca"){
			bolt.connectTo(*to);
		}

		if(from && to){
			from->connectTo(*to,0);
		}
	}
	musicCilinder.connectTo(*getGear("gear1"));
	getGear("screw")->reduction = -1;

	panel.loadFromFile("settings.xml");

	pause.addListener(this,&testApp::pausePressed);
	resetTime.addListener(this,&testApp::resetTimePressed);
	soundPosition.addListener(this,&testApp::soundPositionChanged);

	//camera.setFarClip(-15000);
	camera = fbxScene.getCameras()[0];
	camera->setNearClip(40);
	camera->setFarClip(15000);

	cameraTop.setFarClip(-15000);
	cameraTop.setPosition(0,cameraTopY,-1500);
	cameraTop.lookAt(ofVec3f(0,0,0));


	ofBackground(0);
	light.setup();
	//light.enable();
	//ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	glow.setup(1280,720);

	ofFbo::Settings settings;
	settings.depthStencilAsTexture = false;
	settings.useDepth = true;
	settings.width = 1280;
	settings.height = 720;
	settings.internalformat = GL_RGBA;
	settings.depthStencilInternalFormat = GL_DEPTH_COMPONENT32;
	settings.useStencil = false;
	settings.numSamples = 8;
	renderFbo.allocate(settings);

	animationSoundStream = ofPtr<AnimationSoundStream>(new AnimationSoundStream);
	player.loadSound("07.Microseq.mp3");
	animationSoundStream->setup(2,0,44100,256,1);
	animationSoundStream->setClock(clock);
	//player.setSoundStream(animationSoundStream);
	audioClock.setSoundPlayer(player,-200000);
	ofAddListener(player.newBufferE,&wave,&WaveStripe::newSoundBuffer);
	clock.start();
	audioClock.start();
	player.play();

}

void testApp::soundPositionChanged(float & soundPosition){
	if(updatingSoundPos) return;
	player.setPositionMS(soundPosition*1000);
}

void testApp::facesWireframeAlphaChanged(float & wireframeAlpha){
	for(u_int i=0;i<gears.size();i++){
		gears[i]->wireFacesAlpha = wireframeAlpha;
	}
	for(u_int i=0;i<models.size();i++){
		models[i]->wireFacesAlpha = wireframeAlpha;
	}
	musicCilinder.wireFacesAlpha = wireframeAlpha;
	wave.wireFacesAlpha = wireframeAlpha;
	bolt.wireFacesAlpha = wireframeAlpha;
}

void testApp::lineWidthChanged(float & lineWeight){
	for(u_int i=0;i<gears.size();i++){
		gears[i]->lineWeight = lineWeight;
	}
	for(u_int i=0;i<models.size();i++){
		models[i]->lineWeight = lineWeight;
	}
	musicCilinder.lineWeight = lineWeight;
	wave.lineWeight = lineWeight;
}

void testApp::pausePressed(bool & pressed){
	if(pause) player.setPaused(pause);
	else player.play();
}

void testApp::resetTimePressed(bool & pressed){
	if(!pressed) clock.reset();
}

//--------------------------------------------------------------
void testApp::update(){
	if(autoAnimation){
		cameraClock.update();
		frame = cameraClock.getFrame();
	}else{
		cameraClock.setFrame(frame);
	}

	clock.update();
	audioClock.update();
	animationSoundStream->update();

	updatingSoundPos = true;
	soundPosition = double(player.getPositionMS())/1000.;
	updatingSoundPos = false;

	light.setPosition(lightx,lighty,lightz);
	const ofSoundBuffer & buffer = player.getCurrentBuffer();
	float avg=0;
	for(int i=0;i<(int)buffer.bufferSize();i++){
		avg += abs(buffer[i*buffer.getNumChannels()]);
	}
	//cout << "avg " << avg << "/" << buffer.bufferSize() << endl;
	if(buffer.bufferSize()){
		avg/=buffer.bufferSize();

		getGear("gear2")->rotZVel = avg*1800;
		for(u_int i=0;i<gears.size();i++){
			gears[i]->update();
		}
		musicCilinder.update();
		bolt.update();
		wave.update();
	}

	camera->setTime(cameraClock.getElapsedTimeMillis());



	fps = ofGetFrameRate();

	if(!autoAnimation){
		//camera->setFov(cameraFov);
		camera->lookAt(musicCilinder.getNode().getGlobalPosition(),ofVec3f(0,1,0));
		while (cameraLatitude>90) cameraLatitude-=180;
		camera->orbit(cameraLongitude,cameraLatitude,orbitRadius,musicCilinder.getNode().getGlobalPosition());
	}

	cameraTop.resetTransform();
	cameraTop.setPosition(0,cameraTopY,0);//gear1.pos.get().z);
	cameraTop.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));

	cameraFront.resetTransform();
	cameraFront.setPosition(0,0,-1000);//gear1.pos.get().z);
	cameraFront.lookAt(ofVec3f(0,0,0),ofVec3f(0,1,0));

	cameraLeft.resetTransform();
	cameraLeft.setPosition(-1000,0,0);//gear1.pos.get().z);
	cameraLeft.lookAt(ofVec3f(0,0,0),ofVec3f(0,1,0));

	cameraRight.resetTransform();
	cameraRight.setPosition(1000,0,0);//gear1.pos.get().z);
	cameraRight.lookAt(ofVec3f(0,0,0),ofVec3f(0,1,0));
}

//--------------------------------------------------------------
void testApp::draw(){
	if(lightsOn){
		light.enable();
	}
	glEnable(GL_DEPTH_TEST);
	ofFill();
	ofSetColor(255);
	ofPushView();
	ofViewport(290,10,1280,720);
	ofBackgroundGradient(ofColor::gray,ofColor::black,OF_GRADIENT_CIRCULAR);
	ofPopView();

	glow.begin(false);
	if(blendMode==WireALPHAFacesADD || blendMode==AllADD) ofEnableBlendMode(OF_BLENDMODE_ADD);
	else ofEnableAlphaBlending();

	camera->begin();
	//ofBackgroundGradient(ofColor(50),ofColor::black,OF_GRADIENT_CIRCULAR);

	for(u_int i=0;i<gears.size();i++){
		gears[i]->drawFill();
	}
	for(u_int i=0;i<models.size();i++){
		models[i]->drawFill();
	}
	musicCilinder.drawFill();
	bolt.drawFill();
	wave.drawFill();
	camera->end();
	glow.end();

	ofSetColor(255);
	glow.draw(290,720+10,1280,-720);

	camera->begin(ofRectangle(290,10,1280,720));
	if(blendMode==WireADDFacesALPHA) ofEnableBlendMode(OF_BLENDMODE_ADD);
	else ofEnableAlphaBlending();
	for(u_int i=0;i<gears.size();i++){
		gears[i]->drawContour();
	}
	for(u_int i=0;i<models.size();i++){
		models[i]->drawContour();
	}

	musicCilinder.drawContour();
	bolt.drawContour();
	wave.drawContour();
	camera->end();
	glDisable(GL_DEPTH_TEST);

	light.disable();
	ofDisableLighting();

	/*ofPushView();
	ofViewport(290,10,1280,720);
	ofSaveFrame(true);
	ofPopView();*/

	if(drawAdditionalViews){
		ofSetColor(0);
		ofFill();
		ofRect(290,10,1280/4,720/4);
		cameraTop.begin(ofRectangle(290,10,1280/4,720/4));

		for(u_int i=0;i<gears.size();i++){
			gears[i]->drawContour();
		}
		for(u_int i=0;i<models.size();i++){
			models[i]->drawContour();
		}

		musicCilinder.drawContour();
		bolt.drawContour();
		wave.drawContour();
		camera->draw();
		cameraTop.end();

		ofSetColor(0);
		ofFill();
		ofRect(290,10+720/4,1280/4,720/4);
		cameraFront.begin(ofRectangle(290,(720/4)+10,1280/4,720/4));

		for(u_int i=0;i<gears.size();i++){
			gears[i]->drawContour();
		}
		for(u_int i=0;i<models.size();i++){
			models[i]->drawContour();
		}

		musicCilinder.drawContour();
		bolt.drawContour();
		wave.drawContour();
		camera->draw();
		cameraFront.end();

		ofSetColor(0);
		ofFill();
		ofRect(290,10+(720/4)*2,1280/4,720/4);
		cameraLeft.begin(ofRectangle(290,(720/4)*2+10,1280/4,720/4));

		for(u_int i=0;i<gears.size();i++){
			gears[i]->drawContour();
		}
		for(u_int i=0;i<models.size();i++){
			models[i]->drawContour();
		}

		musicCilinder.drawContour();
		bolt.drawContour();
		wave.drawContour();
		camera->draw();
		cameraLeft.end();

		ofSetColor(0);
		ofFill();
		ofRect(290,10+(720/4)*3,1280/4,720/4);
		cameraRight.begin(ofRectangle(290,(720/4)*3+10,1280/4,720/4));

		for(u_int i=0;i<gears.size();i++){
			gears[i]->drawContour();
		}
		for(u_int i=0;i<models.size();i++){
			models[i]->drawContour();
		}

		musicCilinder.drawContour();
		bolt.drawContour();
		wave.drawContour();
		camera->draw();
		cameraRight.end();

		ofNoFill();
		ofSetLineWidth(1);
		ofSetColor(255);
		ofRect(290,10,1280,720);
		ofRect(290,10,1280/4,720/4);
		ofRect(290,10+720/4,1280/4,720/4);
		ofRect(290,10+(720/4)*2,1280/4,720/4);
		ofRect(290,10+(720/4)*3,1280/4,720/4);
	}


	panel.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
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

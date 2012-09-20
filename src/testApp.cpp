#include "testApp.h"

#include "scene.h"

Gear * testApp::getGear(string name){
	for(u_int i=0;i<gears.size();i++){
		if(gears[i]->getName()==name){
			return gears[i];
		}
	}
	return NULL;
}

Shaft * testApp::getShaft(string name){
	for(u_int i=0;i<shafts.size();i++){
		if(shafts[i]->getName()==name){
			return shafts[i];
		}
	}
	return NULL;
}

Model * testApp::getModel(string name){
	for(u_int i=0;i<models.size();i++){
		if(models[i]->getName()==name){
			return models[i];
		}
	}
	for(u_int i=0;i<shafts.size();i++){
		if(shafts[i]->getName()==name){
			return shafts[i];
		}
	}
	for(u_int i=0;i<gears.size();i++){
		if(gears[i]->getName()==name){
			return gears[i];
		}
	}
	return NULL;
}



//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel("ofxFBXScene",OF_LOG_SILENT);

	clock.setFrameRate(30);
	cameraClock.setFrameRate(30);
	Model::clock = &clock;
	MusicCilinder::audioClock = &audioClock;
	MusicCilinder::soundPlayer = &player;
	Gear::soundPlayer = &player;
	counter.audioClock = &audioClock;
	bolt.audioClock = &audioClock;

	//fbxScene.load("A_camara_17sep1511_testsept1343_MOVcurvo.fbx");
#ifdef SCENE_0
	fbxScene.load("movCurvo02.fbx");
#endif
#ifdef SCENE_1
	fbxScene.load("camaraSirena10.fbx");
#endif
#ifdef SCENE_2
	fbxScene.load("camaraSirena10.fbx");
#endif
	camera = fbxScene.getCameras()[0];
	ofLogNotice() << "using camera " << camera->getName() << endl;

	wave.setup();
	globalFacesWireframeAlpha.addListener(this,&testApp::facesWireframeAlphaChanged);
	globalLineWidth.addListener(this,&testApp::lineWidthChanged);

	panel.setup("gui");
	parameters.setName("generalParameters");
	parameters.add(lightx.set("x_light",ofGetWidth()*.5,-ofGetWidth(),ofGetWidth()));
	parameters.add(lighty.set("y_light",ofGetHeight()*.5,-ofGetHeight(),ofGetHeight()));
	parameters.add(lightz.set("z_light",ofGetWidth()*.5,-ofGetWidth(),ofGetWidth()));
	parameters.add(fps.set("fps",60,0,300));
	parameters.add(cameraLatitude.set("cameraLatitude",0,-180,180));
	parameters.add(cameraLongitude.set("cameraLongitude",0,-180,180));
	parameters.add(nearPlane.set("nearPlane",1,-1,1));
	parameters.add(farPlane.set("farPlane",-2,0,-50));

	parameters.add(orbitRadius.set("orbitRadius",1000,0,400));
	parameters.add(globalFacesWireframeAlpha.set("globalFacesWireframeAlpha",1,0,1));
	parameters.add(globalLineWidth.set("globalLineWidth",3,0,10));;
	parameters.add(blendMode.set("blendMode",0,0,3));
	parameters.add(cameraTopY.set("cameraTopY",3000,0,6000));
	parameters.add(cameraFov.set("cameraFov",camera->getFov(),1,200));
	parameters.add(cameraUp.set("cameraUp",ofVec3f(0,1,0),ofVec3f(0,0,0),ofVec3f(1,1,1)));
	parameters.add(cameraOffset.set("cameraOffset",ofVec3f(0,0,0),ofVec3f(-10,-10,-10),ofVec3f(10,10,10)));
	parameters.add(glow.passes);
	parameters.add(pause.set("pause",false));
	parameters.add(autoAnimation.set("autoAnimation",true));
	parameters.add(fbxCamera.set("fbxCamera",true));
	parameters.add(frame.set("frame",0,500,1100));
	parameters.add(soundPosition.set("soundPosition",0,0,225));
	parameters.add(drawAdditionalViews.set("drawAdditionalViews",false));
	parameters.add(saveOneFrame.set("saveOneFrame",false));
	parameters.add(record.set("record",false));
	parameters.add(lightsOn.set("lightsOn",false));

	panel.add(parameters);
	panel.add(resetTime.setup("resetTime"));


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
		if(ofIsStringInString(name,"gear") ||
		   name == "screw" ){
			Gear * gear = new Gear;
			gear->setup(*fbxScene.getMeshes()[i]);
			gear->reduction = reduction;
			gear->rotZFactor = 1;
			if(name=="geardv2" || name=="geardh2"){
				gear->rotAxis = -1;
			}else if(ofIsStringInString(name,"geardv")
				|| ofIsStringInString(name,"geardh")
				|| ofIsStringInString(name,"screw")){
				gear->rotAxis = 1;
			}else{
				gear->rotAxis = 2;
			}
			gears.push_back(gear);
			panel.add(gear->parameters);
		}else if(ofIsStringInString(name,"shaft")){
			Shaft * shaft = new Shaft;
			shaft->setup(*fbxScene.getMeshes()[i]);
			shafts.push_back(shaft);
			panel.add(shaft->parameters);
		}else if(ofIsStringInString(name,"cillinder")){
			musicCilinder.setup(*fbxScene.getMeshes()[i]);
			panel.add(musicCilinder.parameters);
		}else if(ofIsStringInString(name,"tuerca")){
			bolt.setup(*fbxScene.getMeshes()[i]);
			bolt.rotAxis = 1;
			panel.add(bolt.parameters);
		}else if(ofIsStringInString(name,"plane")){
			counter.setup(*fbxScene.getMeshes()[i]);
			panel.add(counter.parameters);
		}else{
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
		ofLogNotice() << "trying to connect " << name << " to " << connection;
		Gear * from = getGear(name);
		Gear * to = getGear(connection);
		if(!from && to){
			if(ofIsStringInString(name,"shaft")){
				getShaft(name)->connectTo(*to);
			}
		}else if(from && !to){
			if(connection=="cillinder"){
				from->connectTo(musicCilinder);
			}
			if(ofIsStringInString(connection,"shaft")){
				from->connectTo(*getShaft(connection));
			}
		}else if(!from && name=="tuerca"){
			bolt.connectTo(*to);
		}else if(from && to){
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


	ofRectangle view(290,10,1280,720);
	screen.allocate(view.width, view.height, OF_IMAGE_COLOR);


#ifdef SCENE_0
	blendMode = 3;
#endif
#ifdef SCENE_1
	blendMode = 3;
#endif
#ifdef SCENE_2
	blendMode = 3;
#endif

	fbxCamera = true;
	animationSoundStream = ofPtr<AnimationSoundStream>(new AnimationSoundStream);
	player.loadSound("07.Microseq.mp3");
	animationSoundStream->setup(2,0,44100,256,1);
	animationSoundStream->setClock(clock);
	autoAnimation = true;
	//record = true;
	if(record) player.setSoundStream(animationSoundStream);
	audioClock.setSoundPlayer(player,-200000);
	ofAddListener(player.newBufferE,&wave,&WaveStripe::newSoundBuffer);
	clock.start();
	audioClock.start();
	cameraClock.start();
#ifdef SCENE_1
	clock.setFrame(500);
	cameraClock.setFrame(500);
#endif
	player.play();
#ifdef SCENE_1
	player.setPositionMS(52000);
#endif

}

void testApp::soundPositionChanged(float & soundPosition){
	if(updatingSoundPos) return;
	player.setPositionMS(soundPosition*1000);
}

void testApp::facesWireframeAlphaChanged(float & wireframeAlpha){
	for(u_int i=0;i<gears.size();i++){
		gears[i]->wireFacesAlpha = wireframeAlpha;
	}
	for(u_int i=0;i<shafts.size();i++){
		shafts[i]->wireFacesAlpha = wireframeAlpha;
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
	for(u_int i=0;i<shafts.size();i++){
		shafts[i]->lineWeight = lineWeight;
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
	if(!pressed){
		clock.reset();
		audioClock.reset();
		cameraClock.reset();
		frame = 0;
	}
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

#ifdef SCENE_1
	if(frame<1153)fbxScene.setTime(cameraClock.getElapsedTimeMillis());
#endif

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
		Gear * mainGear = getGear("gear2");
		if(mainGear)
			mainGear->rotZVel = avg*1800;
		for(u_int i=0;i<gears.size();i++){
			gears[i]->update();
		}
		for(u_int i=0;i<models.size();i++){
			models[i]->update();
		}
		for(u_int i=0;i<shafts.size();i++){
			shafts[i]->update();
		}
		musicCilinder.update();
		bolt.update();
		wave.update();
#ifndef SCENE_0
		counter.update();
#endif
	}

	/*if(frame<800){
		camera->setTime(cameraClock.getElapsedTimeMillis());
	}else if(frame==799){
		counterCameraNode = camera->getNode();
	}else if(player.getPositionMS()>65000){
		((ofNode&)*camera) = counterCameraNode;
		float pct = ofClamp(double(66000-player.getPositionMS())/1000.,0,1);
		//counter.getNode().getGlobalPosition();
		ofVec3f interpLookAt = bolt.getNode().getGlobalPosition().interpolate(counter.getNode().getGlobalPosition(),pct);
		camera->lookAt(interpLookAt,camera->getUpDir());
	}else{
		((ofNode&)*camera) = counterCameraNode;
	}*/



	fps = ofGetFrameRate();

	if(!fbxCamera){
		//camera->setFov(cameraFov);
		camera->lookAt(musicCilinder.getNode().getGlobalPosition(),ofVec3f(0,1,0));
		while (cameraLatitude>90) cameraLatitude-=180;
		camera->orbit(cameraLongitude,cameraLatitude,orbitRadius,musicCilinder.getNode().getGlobalPosition());
	}else{
		/*if(frame>762){
			ofVec3f rotationVector(0,1.0,0);
			ofVec3f up = camera->getGlobalOrientation() * rotationVector;

			ofVec3f forward = (getModel("counter")->getNode().getGlobalPosition() - camera->getGlobalPosition()).getNormalized();
			forward.normalize();
			ofVec3f right = up.cross(forward);
			right.normalize();
			up = forward.cross(right);
			up.normalize();
			camera->lookAt(getModel("counter")->getNode(),up);
		}*/
		//camera->setFov(cameraFov);
		//camera->move(cameraOffset);
	}

	camera->setNearClip(nearPlane);
	camera->setFarClip(farPlane);

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
	for(u_int i=0;i<shafts.size();i++){
		shafts[i]->drawFill();
	}
	musicCilinder.drawFill();
	bolt.drawFill();
	wave.drawFill();
#ifndef SCENE_0
	counter.drawFill();
#endif
	camera->end();
	if(saveOneFrame || record){
		glow.getDepthTexture().readToPixels(depthPixels);
		ofSaveImage(depthPixels,"depth_"+ofToString(ofGetFrameNum())+".png");
	}
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
	for(u_int i=0;i<shafts.size();i++){
		shafts[i]->drawContour();
	}

	musicCilinder.drawContour();
	bolt.drawContour();
	wave.drawContour();

#ifndef SCENE_0
	counter.drawContour();
#endif
	camera->end();
	glDisable(GL_DEPTH_TEST);

	light.disable();
	ofDisableLighting();

	if(saveOneFrame || record){
		/*ofPushView();
		ofViewport(290,10,1280,720);
		ofSaveViewport(ofToString(ofGetFrameNum())+".png");
		ofPopView();*/
		ofRectangle view(290,10,1280,720);
		screen.grabScreen(view.x, view.y, view.width, view.height);
		screen.saveImage(ofToString(ofGetFrameNum())+".png");
		saveOneFrame = false;
	}

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
		for(u_int i=0;i<shafts.size();i++){
			shafts[i]->drawContour();
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
		for(u_int i=0;i<shafts.size();i++){
			shafts[i]->drawContour();
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
		for(u_int i=0;i<shafts.size();i++){
			shafts[i]->drawContour();
		}

		musicCilinder.drawContour();
		bolt.drawContour();
		wave.drawContour();
		camera->draw();
		cameraRight.end();

		ofNoFill();
		ofSetLineWidth(1);
		ofSetColor(255);
		ofRect(290,10,1280/4,720/4);
		ofRect(290,10+720/4,1280/4,720/4);
		ofRect(290,10+(720/4)*2,1280/4,720/4);
		ofRect(290,10+(720/4)*3,1280/4,720/4);
	}

	ofNoFill();
	ofSetLineWidth(1);
	ofSetColor(255);
	ofRect(290,10,1280,720);


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

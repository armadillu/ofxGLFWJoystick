#include "ofApp.h"

void ofApp::setup(){
	if (!ofxGLFWJoystick::one().setProvider(ofxGLFWJoystick::Provider::BOTH)) {
		ofLogError() << "Could not start SDL controller input";
	}
}

void ofApp::update(){ ofxGLFWJoystick::one().update(); }

void ofApp::draw(){
	ofBackground(25);
	ofDrawBitmapStringHighlight("GLFW and SDL providers: move sticks and press buttons", 20, 25);
	ofxGLFWJoystick::one().drawDebug(20, 80);
}

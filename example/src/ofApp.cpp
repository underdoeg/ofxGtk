#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofLog() << "SETUP";

	drawFilled = true;
	drawScale = 5;
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(0);

	ofSetColor(255);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
	
	if(drawFilled)
		ofFill();
	else
		ofNoFill();
	ofSetColor(color);
	ofDrawCircle(ofGetMouseX(), ofGetMouseY(), drawScale);

}

//--------------------------------------------------------------

void ofApp::exit() {
	ofLogNotice() << "Exit";
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if(key == 'f')
		ofToggleFullscreen();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

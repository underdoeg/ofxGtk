#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp(){
	params.setName("Circle Parameters");
	params.add(radius.set("Radius", 10, 1, 150));
	params.add(color.set("Color", ofColor::red));
	//params.add(show.set(true));
	params.add(drawFilled.set("Fill", true));
}

void ofApp::setup() {
	ofLog() << "SETUP";

	drawFilled = true;
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
	ofDrawCircle(ofGetMouseX(), ofGetMouseY(), radius);
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

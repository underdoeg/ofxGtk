#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		ofApp();

		void setup();
		void update();
		void draw();
		
		void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofParameterGroup params;
		ofParameter<float> radius;
		ofParameter<ofColor> color;
		ofParameter<bool> drawFilled;
		ofParameter<bool> show;
        ofParameter<ofVec2f> pos;
};

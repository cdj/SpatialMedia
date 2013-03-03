#pragma once

#include "ofMain.h"

#include "ofxTSPSReceiver.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void draw();
    
        ofxTSPS::Receiver tspsReceiver;
        map<int, ofColor> colors;
};

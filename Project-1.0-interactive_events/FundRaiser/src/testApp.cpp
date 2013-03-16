#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    // connect to tsps
    tspsReceiver.connect(12000);
    
    ofSetFrameRate(60);
    ofSetCircleResolution(60);
    ofEnableSmoothing();
    //ofSetWindowShape(roomSize * makeBigger, roomSize * makeBigger);
    
    ofClear(0);
    mapper.initialize(ofGetWidth(), ofGetHeight() - 2 * stringsEdge);
    mapper.load("mapsettings.txt");
    bDrawBounds = false;
    
    // Initiate Tables
    for (int i = 0; i < 18; i++) {
        // Upper
        if (i < 9) {
            tables.push_back( StandingTable::StandingTable( 150 + i * tableDist / 2, margin + ( i % 2) * tableDist, tableSize, stringsEdge) );
        }
        // Lower
        else {
            tables.push_back( StandingTable::StandingTable( 150 + (i - 9) * tableDist / 2, ofGetHeight() - margin - ( (i - 9) % 2) * tableDist, tableSize, stringsEdge) );
        }
    }
    
    maxLength = ofDist(tables[0].dot.x, tables[0].dot.y, tables[17].dot.x, tables[17].dot.y);
    minLength = ofDist(tables[0].dot.x, tables[0].dot.y, tables[9].dot.x, tables[9].dot.y);
    
    projectionMode = false;
    
    // Audio setup
    
	// 2 output channels,
	// 0 input channels
	// 22050 samples per second
	// 512 samples per buffer
	// 4 num buffers (latency)
	
	int bufferSize		= 512;
	sampleRate 			= 44100;
	phase 				= 0;
	phaseAdder 			= 0.0f;
	phaseAdderTarget 	= 0.0f;
	volumeMax				= 0.1f;
    
	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);
    
	soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);

}

//--------------------------------------------------------------
void testApp::update(){
    // Add new strings
    if (index1 != -1 && index2 != -1) {
        strings.push_back( GuitarString(index1, index2, stringsEdge, tables) );
        index1 = -1;
        index2 = -1;
    }

    // Clean up old strings
    list<GuitarString>::iterator i;
    for (i = strings.begin(); i != strings.end(); ) {
        if (!i->checkLife()) {
            tables[i->index1].isSelected = false;
            tables[i->index2].isSelected = false;
            i = strings.erase(i);
        }
        else ++i;
    }
    
    //list<GuitarString>::iterator i;
    for (i = strings.begin(); i != strings.end(); i++) {

        i->checkPluck(ofGetMouseX(), ofGetMouseY());
        
        // get TSPS people
        vector<ofxTSPS::Person *> people = tspsReceiver.getPeople();
        
        // check if people are plucking the string
        for (int j=0; j<people.size(); j++){
            i->checkPluck(people[j]->centroid.x*(ofGetWidth() - 150*2)+150,
                          people[j]->centroid.y*(ofGetHeight() - stringsEdge*2)+stringsEdge);
        }
        i->update();
    }
    
    mapper.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    if (projectionMode) {
        ofTranslate(0, -stringsEdge);
    }
    
    
    ofBackground(0);
    
    // Draw strings
    list<GuitarString>::iterator i;
    for (i = strings.begin(); i != strings.end(); i++) {
        i->draw();
    }

    
    // Draw tables
    for (int i = 0; i < tables.size(); i++) {
        
        if ((ofGetMouseX() < tables[i].x + tableSize && ofGetMouseX() > tables[i].x - tableSize)
            && (ofGetMouseY() < tables[i].y + tableSize && ofGetMouseY() > tables[i].y - tableSize)) {
            tables[i].isHovered = true;
        } else {
            tables[i].isHovered = false;
        }
        tables[i].draw();
        

    }
    
    if (projectionMode) {
        //ofTranslate(0, stringsEdge);
        mapper.startMapping();
        mapper.stopMapping();
    }
    
    if ( bDrawBounds ){
        mapper.drawBoundingBox();
    }
    
    // get TSPS people
    vector<ofxTSPS::Person *> people = tspsReceiver.getPeople();
    
    // draw boxes where the people are
    for (int i=0; i<people.size(); i++){
        ofSetColor( ofColor( ofRandom(255), ofRandom(255), ofRandom(255)));
        ofRect(people[i]->boundingRect.x*(ofGetWidth() - 150*2)+150, people[i]->boundingRect.y*(ofGetHeight() - stringsEdge*2)+stringsEdge, people[i]->boundingRect.width*(ofGetWidth() - 150*2), people[i]->boundingRect.height*(ofGetHeight() - stringsEdge*2));
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case 'p':
            projectionMode = !projectionMode;
            break;
        case 's':
            // save to a file
            mapper.save("mapsettings.txt");
            break;
        case 'd':
            bDrawBounds = !bDrawBounds;
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        // Audio related keypresses
        case '-':
        case '_':
            volumeMax -= 0.05;
            volumeMax = MAX(volumeMax, 0);
            break;
        case '+':
        case '=':
            volumeMax += 0.05;
            volumeMax = MIN(volumeMax, 1);
            break;
        case 'a':
            soundStream.start();
            break;
        case 'z':
            soundStream.stop();
            break;
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
    mapper.mouseDragged(x, y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    // Figure out if they have clicked on a table/circle
    for (int i = 0; i < tables.size(); i++) {
        if ((ofGetMouseX() < tables[i].x + tableSize && ofGetMouseX() > tables[i].x - tableSize)
            && (ofGetMouseY() < tables[i].y + tableSize && ofGetMouseY() > tables[i].y - tableSize)) {
            if (i < 9) {
//                if (index1 != -1) {
//                    tables[index1].isSelected = false;
//                }
                index1 = i;
            } else {
//                if (index2 != -1) {
//                    tables[index2].isSelected = false;
//                }
                index2 = i;
            }
            tables[i].isSelected = true;
            //tables[i].isSelected = !tables[i].isSelected;
            
            break;

            
        }
    }
    
    mapper.mousePressed(x, y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    mapper.mouseReleased(x, y);
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

//--------------------------------------------------------------
// Do the audio output thing
void testApp::audioOut(float * output, int bufferSize, int nChannels){
	//pan = 0.5f;
	float leftScale = 1 - pan;
	float rightScale = pan;
    list<GuitarString>::iterator iString;
    
	// sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-TWO_PI like this:
	while (phase > TWO_PI){
		phase -= TWO_PI;
	}
    //for (iString = strings.begin(); iString != strings.end(); iString++) {
    //    while (iString->phase > TWO_PI){
    //        iString->phase -= TWO_PI;
    //    }
    //}
    
    //targetFrequency = 2000.0f * 0.5f;
    //phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
    
    //phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;

    for (int i = 0; i < bufferSize; ){
        for (iString = strings.begin(); iString != strings.end(); iString++) {
            float baseLength = ofDist(tables[iString->index1].dot.x, tables[iString->index1].dot.y, tables[iString->index2].dot.x, tables[iString->index2].dot.y);
            targetFrequency = 1600.0f * (1 - (baseLength - minLength)) / (maxLength - minLength) + 400.0f;
            phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
            //phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
            iString->phaseAdder = 0.95f * iString->phaseAdder + 0.05f * phaseAdderTarget;
            //phase += phaseAdder;
            phase += iString->phaseAdder;
            //iString->phase += iString->phaseAdder;
            float sample = sin(phase);
            //float sample = sin(iString->phase);
            float volPer = iString->spring.b.velocity.length()/10;
            lAudio[i] = output[i*nChannels    ] = sample * volumeMax * volPer;// * leftScale;
            rAudio[i] = output[i*nChannels + 1] = sample * volumeMax * volPer;// * rightScale;
            i++;
        }
    }
}
#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetCircleResolution(60);
    ofEnableSmoothing();
    //ofSetWindowShape(roomSize * makeBigger, roomSize * makeBigger);
    
    canvas.allocate(ofGetWidth(), ofGetHeight() - 2 * stringsEdge);
    canvas.begin();
    ofClear(0);
    canvas.end();
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
        i->update();
    }
    
    mapper.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    if (projectionMode) {
        canvas.begin();
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
        canvas.end();
        //ofTranslate(0, stringsEdge);
        mapper.startMapping();
        canvas.draw(0,0);
        mapper.stopMapping();
    }
    
    if ( bDrawBounds ){
        mapper.drawBoundingBox();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'p') {
        projectionMode = !projectionMode;
    } else if ( key == 's' ){
        // save to a file
        mapper.save("mapsettings.txt");
    } else if ( key == 'd' ){
        bDrawBounds = !bDrawBounds;
    } else if ( key == 'f' ) {
        ofToggleFullscreen();
    }

    // Audio related keypresses
	if (key == '-' || key == '_' ){
		volumeMax -= 0.05;
		volumeMax = MAX(volumeMax, 0);
	} else if (key == '+' || key == '=' ){
		volumeMax += 0.05;
		volumeMax = MIN(volumeMax, 1);
	}
	
	if( key == 's' ){
		soundStream.start();
	}
	
	if( key == 'e' ){
		soundStream.stop();
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
    //float maxLength = ofDist(tables[0].dot.x, tables[0].dot.y, tables[tables.size()].dot.x, tables[tables.size()].dot.y);
    
	// sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-TWO_PI like this:
	while (phase > TWO_PI){
		phase -= TWO_PI;
	}
    
    targetFrequency = 2000.0f * 0.5f;
    phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
    
    phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;

    for (int i = 0; i < bufferSize; ){
        for (iString = strings.begin(); iString != strings.end(); iString++) {
            phase += phaseAdder;
            float sample = sin(phase);
            float volPer = iString->spring.b.velocity.length()/10;
            lAudio[i] = output[i*nChannels    ] = sample * volumeMax * volPer;// * leftScale;
            rAudio[i] = output[i*nChannels + 1] = sample * volumeMax * volPer;// * rightScale;
            i++;
        }
    }
}
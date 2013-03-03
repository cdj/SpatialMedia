#include "testApp.h"

using namespace ofxTSPS;

//--------------------------------------------------------------
void testApp::setup(){
    tspsReceiver.connect(12000);
    
    ofSetBackgroundAuto(false);
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetCircleResolution(60);

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    int maxRadius = 200;
    float radius;
    
    ofSetColor(0,10);
    ofRect(0,0,ofGetWidth(), ofGetHeight());
    
    vector<ofxTSPS::Person *> people = tspsReceiver.getPeople();
    for (int i=0; i<people.size(); i++){
        if ( colors.count( people[i]->pid ) == 0){
            colors[ people[i]->pid ] = ofColor( ofRandom(255), ofRandom(255), ofRandom(255), 50);
        }
        
        ofSetColor( colors[ people[i]->pid ]);
        
        ofPoint centroid = people[i]->centroid;
        centroid.x *= ofGetWidth();
        centroid.y *= ofGetHeight();
        
        radius = maxRadius * people[i]->velocity.length() / 100;
        
        ofCircle(centroid, radius);
    }
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
void testApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}

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
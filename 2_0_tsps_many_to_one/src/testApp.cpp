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
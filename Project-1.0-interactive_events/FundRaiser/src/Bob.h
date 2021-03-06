//
//  Bob.h
//  FundRaiser
//
//  Created by Xinran Wang on 3/11/13.
//   Modified by Carl Jamilkowski
//
//

#ifndef __FundRaiser__Bob__
#define __FundRaiser__Bob__

#include <iostream>
#include "ofMain.h"

class Bob {
public:
    Bob();
    Bob(float x, float y);
    void update();
    void applyForce(ofVec2f force);
    void display();
    void clicked(int mx, int my);
    void stopDragging();
    void drag(int mx, int my);
    
    ofVec2f location;
    ofVec2f velocity;
    ofVec2f acceleration;
    float mass = 24;
    float damping = 0.98;
    
    ofVec2f dragOffset;
    bool dragging = false;
};

#endif /* defined(__FundRaiser__Bob__) */

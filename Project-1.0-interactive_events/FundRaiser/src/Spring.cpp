//
//  Spring.cpp
//  FundRaiser
//
//  Created by Xinran Wang on 3/11/13.
//   Modified by Carl Jamilkowski
//
//

#include "Spring.h"

Spring::Spring() {
    
}

Spring::Spring(float x, float y, int l, float _minlen, float _maxlen, Bob _b) {
    anchor.set(x, y);
    len = l;
    minlen = _minlen;
    maxlen = _maxlen;
    b = _b;
}

void Spring::update(float mx, float my) {
    ofVec2f force = b.location - anchor;
    float fl = force.length();
    float stretch = fl - len;
    
    force.normalize();
    force *= -1 * k * stretch;
    b.applyForce(force);
    
    ofVec2f dir = b.location - anchor;
    float d = dir.length();
    if (d < minlen) {
        dir.normalize();
        dir *= minlen;
        b.location = anchor + dir;
        b.velocity *= 0;
    }
    else if (d > maxlen) {
        dir.normalize();
        dir *= maxlen;
        b.location = anchor + dir;
        b.velocity *= 0;
    }
    b.update();
    b.drag(mx, my);
}

void Spring::display() {
    ofSetColor(175);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofRect(anchor.x, anchor.y, 10, 10);
}

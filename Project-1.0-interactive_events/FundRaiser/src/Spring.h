//
//  Spring.h
//  FundRaiser
//
//  Created by Xinran Wang on 3/11/13.
//   Modified by Carl Jamilkowski
//
//

#ifndef __FundRaiser__Spring__
#define __FundRaiser__Spring__

#include <iostream>
#include "ofMain.h"
#include "Bob.h"

class Spring {
public:
    Spring();
    Spring(float x, float y, int l, float _minlen, float _maxlen, Bob _b);
    void display();
    
    void update(float mx, float my);
    
    ofVec2f anchor;
    float len;
    float k = 10;
    
    float minlen;
    float maxlen;
    Bob b;

};
#endif /* defined(__FundRaiser__Spring__) */

#pragma once

#include "ofMain.h"
#include "ofxTonic.h"
using namespace Tonic;


class EventParticle{
    
public:
    EventParticle();
    void setup(ofVec3f, ofPolyline);
    void linkSample(float*, float, bool, int, int);
    
    float* bufferPtr;
    int sampleSize;
    
    SampleTable grain;
    BufferPlayer player;
    Generator toneOut;
    
    ControlTrigger trigger;
    
    
    ofVec3f pos;
    ofVec3f dir;
    int polyVecPos;
    float vecPercent;
    ofPolyline polyLine;
    
    ofSpherePrimitive sphere;
    
    bool startAnim;
    
    void draw();
    void updateParticle();
    void triggerSound();
    
    float mInc;
    ofColor farbe;
    
};

#pragma once

#include "ofMain.h"
#include "ofxTonic.h"
using namespace Tonic;


class EventParticle{
    
public:
    EventParticle();
    void setup(ofVec3f, ofPolyline);
    void update();
    void setStereo(float);
    void setVolume(float);
    
    float* bufferPtr;
    int sampleSize;
    
    ofxTonicSynth synth;
    Generator toneOut;
    
    RampedValue rampPan;
    RampedValue rampVol;

    
    float ranFreq;
    float baseFreq;
    float stereo;
    float modulator;
    
    ofVec3f pos;
    ofVec3f dir;
    int polyVecPos;
    float vecPercent;
    ofPolyline polyLine;
    
    ofSpherePrimitive sphere;
    
    bool startAnim;
    
    void draw();
    void updateParticle();
    
    float mInc;
    ofColor farbe;
    
    float distToCenter;
    
};

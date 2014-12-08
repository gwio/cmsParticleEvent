#pragma once

#include "ofMain.h"
#include "ofxTonic.h"
using namespace Tonic;


class EventParticle{
    
public:
    EventParticle();
    void setup(ofVec3f, ofPolyline);
    void update();
    void linkSample(float*, float, bool, int, int);
    void setStereo(float);
    
    float* bufferPtr;
    int sampleSize;
    
    SampleTable grain;
    BufferPlayer player;
    ofxTonicSynth synth;
    Generator toneOut;
    ControlTrigger trigger;
    
    SineWave sine;
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
    void triggerSound();
    
    float mInc;
    ofColor farbe;
    
    float distToCenter;
    
};

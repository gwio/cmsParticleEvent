#include "EventParticle.h"


EventParticle::EventParticle() {
    startAnim = false;
    sphere.set(4, 10);
}

void EventParticle::setup(ofVec3f _p, ofPolyline pl) {
    pos = _p;
    polyLine = pl;
    startAnim = false;
    mInc = 0.030;
    polyVecPos = 0;
    vecPercent = 0.0;
    dir = pl.getVertices().at(1)-pos;
    sphere.set(4, 10);
    farbe = ofColor::mediumAquaMarine;
    
    if (polyLine.size() == 2) {
        mInc/=30;
        sphere.set(2,10);
        farbe = ofColor::gold;
        
    }
    
}

void EventParticle::linkSample(float* bufferPtr_, float bp_, bool slow, int grainSize, int ss_) {
    
    sampleSize = ss_;
    bufferPtr = bufferPtr_;
    
    grain = SampleTable(grainSize*2);
    TonicFloat* temp = grain.dataPointer();
    
    int bufferStart = ofRandom(sampleSize-grainSize);
    cout << "buffer start position" << bufferStart << endl;
    
    
    for (int i = bufferStart; i < (bufferStart)+grainSize; i++) {
        for (int j= 0; j < 2; j++) {
            *temp++ = bufferPtr[i];
            if( (bufferPtr[i] > 1.0) || (bufferPtr[i] < -1.0) ){
                cout << "pointer error" << bufferPtr[i] << endl;
            }
        }
    }
    
    
    player.setBuffer(grain).loop(false).trigger(trigger);
    
    //setting für kleines event < 2mb
    
    toneOut = player*0.08;
    toneOut = BPF12().Q(8).cutoff(bp_).input(toneOut);
    toneOut = StereoDelay(0.22,0.22).input(toneOut).wetLevel(0.8).delayTimeLeft(0.3).delayTimeRight(0.32);

    //setting für grosses event > 3mb
    
  //  toneOut = toneOut*0.008;

    
    
}

void EventParticle::triggerSound() {
    trigger.trigger();
}

void EventParticle::updateParticle() {
    if (startAnim) {
        pos = polyLine.getVertices().at(polyVecPos) + (dir*vecPercent);
        vecPercent+=mInc;
        
        if( (vecPercent >= 1.0) && (polyVecPos != polyLine.getVertices().size()-2) )  {
            vecPercent = 0.0;
            polyVecPos++;
            dir = polyLine.getVertices().at(polyVecPos+1)-polyLine.getVertices().at(polyVecPos);
        }
        
        if  ( (vecPercent >= 1.0) && (polyVecPos == polyLine.getVertices().size()-2) )  {
            startAnim = false;
        }
    }
    
    
}


void EventParticle::draw() {
    
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(pos);
    
    ofSetColor(farbe);
    sphere.draw();
    ofPopStyle();
    ofPopMatrix();
}
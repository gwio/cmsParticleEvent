#include "EventParticle.h"


EventParticle::EventParticle() {
    startAnim = false;
    sphere.set(4, 10);
    
    int twoOctavePentatonicScale[10] = {0, 2, 4, 7, 9, 12, 14, 16, 19, 21};
     ranFreq =  twoOctavePentatonicScale[int(ofRandom(9))]+1;
    baseFreq = ranFreq;
    stereo = 0.0;
    modulator = 0.0;
   // ranFreq = 33;
    
    
    ControlParameter triggerPitch = synth.addParameter("triggerPitch");
    ControlParameter triggerPan = synth.addParameter("stereo");
    ControlParameter modulator = synth.addParameter("_modulator");

    
    
    Generator mainFq = ControlMidiToFreq().input(triggerPitch).smoothed();
    
    synth.setParameter("stereo", stereo);
    synth.setParameter("triggerPitch", ranFreq);
    synth.setParameter("_modulator", stereo);


    toneOut = SineWave().freq(mainFq) * 0.4;
    toneOut = toneOut + (SineWave().freq((mainFq*2))*0.3) * SineWave().freq(modulator);
    toneOut = toneOut + (SineWave().freq((mainFq*3))*0.21);
    
     toneOut = MonoToStereoPanner().pan(triggerPan).input(toneOut*0.6);
    
    
    
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

    if (pos.x > 0) {
        synth.setParameter("stereo", 1.0);
    } else {
       synth.setParameter("stereo",-1.0);
    }
}

void EventParticle::update() {
    synth.setParameter("triggerPitch", ranFreq);
    synth.setParameter("_modulator", modulator);

    distToCenter = (pos-ofVec3f(0,0,0)).length()/40;
    modulator = distToCenter*2;
    ranFreq = distToCenter + baseFreq;
    
    
   // cout << distToCenter << endl;

    
}

void EventParticle::setStereo(float pan_) {
    synth.setParameter("stereo", ofClamp(pan_, -1.0,1.0));
}

void EventParticle::linkSample(float* bufferPtr_, float bp_, bool slow, int grainSize, int ss_) {
    
    
    sampleSize = ss_;
    bufferPtr = bufferPtr_;
    
    grain = SampleTable(grainSize*2);
    TonicFloat* temp = grain.dataPointer();
    
    int bufferStart = ofRandom(sampleSize-grainSize);
   // cout << "buffer start position" << bufferStart << endl;
    
    
    for (int i = bufferStart; i < (bufferStart)+grainSize; i++) {
        for (int j= 0; j < 2; j++) {
            *temp++ = bufferPtr[i];
            if( (bufferPtr[i] > 1.0) || (bufferPtr[i] < -1.0) ){
                cout << "pointer error" << bufferPtr[i] << endl;
            }
        }
    }
    
    
    //player.setBuffer(grain).loop(false).trigger(trigger);
    
    //setting für kleines event < 2mb
    
  //  toneOut = player*0.08;
  //  toneOut = BPF12().Q(8).cutoff(bp_).input(toneOut);
   // toneOut = StereoDelay(0.22,0.22).input(toneOut).wetLevel(0.8).delayTimeLeft(0.3).delayTimeRight(0.32);

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
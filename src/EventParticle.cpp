#include "EventParticle.h"


EventParticle::EventParticle() {
    startAnim = false;
    sphere.set(4, 10);
    

}

void EventParticle::setup(ofVec3f _p, ofPolyline pl) {
    pos = _p;
    polyLine = pl;
    startAnim = false;
    mInc = 0.160;
    polyVecPos = 0;
    vecPercent = 0.0;
    dir = pl.getVertices().at(1)-pos;
    sphere.set(5, 10);
    farbe = ofColor::mediumAquaMarine;
    
    
    
    int twoOctavePentatonicScale[10] = {0,3,5,7,10, 12,15,17,19,22};
    ranFreq =  twoOctavePentatonicScale[int(ofRandom(10))]+1;
    baseFreq = ranFreq+24;
    stereo = 0.0;
    modulator = 0.0;
    // ranFreq = 33;
    
    
    int tableSize = 2049;
    
    SampleTable bassTable = SampleTable(tableSize,1);
    TonicFloat* bassTableData = bassTable.dataPointer();
    
    
    SampleTable highTable = SampleTable(tableSize,1);
    TonicFloat* highTableData = highTable.dataPointer();
    
    TonicFloat norm = 1.0f / tableSize;

    for (unsigned long i= 0; i < tableSize; i++) {
        TonicFloat temp = 0;
        TonicFloat phase = TWO_PI*i*norm;

        int sums = 50;
        
        for (int j = 0; j < sums; j++) {
            temp += sinf(phase*(float(j/2)))* powf((sums-float(j))/sums,4);
        }
        
        
        *bassTableData++ = temp *0.13;
        
        
        sums = 22;
        /*
        for (int j = 0; j < sums; j++) {
            temp += sinf(phase*(float(j/4)))* powf((sums-float(j))/sums,4);
        }
         */
        temp =         ((sinf(phase)*0.1)+ (sinf(phase*0.5)*0.002) +(sinf(phase)*0.02) + (sinf(phase*0.7)*0.001))*2;

        *highTableData++ = temp *4.8;

    }
    
    
    ControlParameter triggerPitch = synth.addParameter("triggerPitch");
    ControlParameter triggerPan = synth.addParameter("stereo").max(1.0).min(-1.0);
    ControlParameter modulator = synth.addParameter("_modulator");
    ControlParameter volume = synth.addParameter("volume").max(1.0).min(0.0);
    
    synth.setParameter("stereo", stereo);
    synth.setParameter("triggerPitch", baseFreq);
    synth.setParameter("_modulator", stereo);
    synth.setParameter("volume", 0.0);
    
    //rampPan = RampedValue().value(0.0).length(0.06).target(triggerPan);
    rampVol = RampedValue().value(0.0).length(0.16).target(volume);
    
    
   // Generator mainFq = ControlMidiToFreq().input(triggerPitch).smoothed();
    
    
    if (polyLine.size() == 2) {
        mInc/=30;
        sphere.set(2,10);
        farbe = ofColor::gold;

       TableLookupOsc high = TableLookupOsc().setLookupTable(highTable).freq( ControlMidiToFreq().input(triggerPitch+12));
    toneOut = MonoToStereoPanner().pan(triggerPan).input( (high * volume) );
        
    } else {
        
       TableLookupOsc bass = TableLookupOsc().setLookupTable(bassTable).freq(ControlMidiToFreq().input(triggerPitch));
        toneOut = MonoToStereoPanner().pan(triggerPan).input( (bass * volume) );
    }
    
  //  synth.setParameter("stereo", 0.0);
    
}

void EventParticle::update() {
    float tempFr;
    
    distToCenter = (pos-ofVec3f(0,0,0)).lengthSquared()/500;
    
    tempFr = distToCenter;
    
    if (polyLine.size() == 2){
    synth.setParameter("triggerPitch", baseFreq + (tempFr/200)+ofRandom(-1,1) );
   // synth.setParameter("_modulator", modulator);
    }
        
    if (startAnim) {
        synth.setParameter("_modulator", vecPercent*22);
    }
    
  
  
}

void EventParticle::setStereo(float pan_) {
   // synth.setParameter("stereo", ofClamp(pan_, -1.0,1.0));
}

void EventParticle::setVolume(float volume_) {
    // cout << volume_ << endl;
    synth.setParameter("volume", ofClamp(volume_, 0.0,1.0));
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
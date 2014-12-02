
#include "Event.h"
Event::Event() {
    
    vParticle.clear();
}

Event::Event(ofVboMesh mp1, ofVboMesh md1, ofVboMesh mp2, ofVboMesh md2, vector<ofPolyline> curves) {
    
    
    meshPos1 = mp1;
    meshPos2 = mp2;
    
    meshDir1 = md1;
    meshDir2 = md2;
    
    meshCurves = curves;
}


void Event::drawCurves() {
    
    ofPushStyle();
    for (int i = 0; i < meshCurves.size(); i++) {
        if(meshCurves[i].getVertices().size() == 2) {
            ofSetColor(ofColor::gold);
        } else {
            ofSetColor(ofColor::mediumAquaMarine);
        }
        meshCurves[i].draw();
        
    }
    ofPopStyle();
}

void Event::drawPoints() {
    
    meshPos1.draw();
    meshPos2.draw();
}

void Event::startAnimation() {
    
    for (int i = 0; i < vParticle.size(); i++) {
        vParticle[i].startAnim = true;
        vParticle[i].triggerSound();
    }
    
}

void Event::updateParticle() {
    
    for (int i = 0; i < vParticle.size(); i++) {
        vParticle[i].updateParticle();
    }
    
}
void Event::drawParticles() {
    for (int i = 0; i < vParticle.size(); i++) {
        vParticle[i].draw();
    }
    
}
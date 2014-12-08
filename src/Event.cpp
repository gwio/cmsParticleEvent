
#include "Event.h"
Event::Event() {
    
    vParticle.clear();
    aniEBRec = false;
    aniHERec = false;
    hePercent = 0.1;
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

void Event::updateCluster() {
    
    ofColor temp;
    for (int i = 0; i < cluster.getNumVertices(); i++) {
        temp = ofColor(255,255,255,clusterColor[i%255]);
        cluster.setColor(i, temp);
        clusterColor[i]+=2;
    }
}

void Event::drawHERectHits() {
    
    heRect.draw();
}

void Event::updateHERec() {
    
    if ( aniHERec ) {
        
        for (int i = 0; i < heRect.getNumVertices(); i+=8) {
            heRect.setVertex(i+4,  (heRect.getVertex(i) + ((heRect.getVertex(i+4)-heRect.getVertex(i)).normalize()*(heRectSize[i/8]*hePercent)) )  );
            heRect.setVertex(i+5,  (heRect.getVertex(i+1) + ((heRect.getVertex(i+5)-heRect.getVertex(i+1)).normalize()*(heRectSize[i/8]*hePercent)) )  );
            heRect.setVertex(i+6,  (heRect.getVertex(i+2) + ((heRect.getVertex(i+6)-heRect.getVertex(i+2)).normalize()*(heRectSize[i/8]*hePercent)) )  );
            heRect.setVertex(i+7,  (heRect.getVertex(i+3) + ((heRect.getVertex(i+7)-heRect.getVertex(i+3)).normalize()*(heRectSize[i/8]*hePercent)) )  );

        }
        
        hePercent*=1.1;
        if (hePercent > 100.0) {
            aniHERec = false;
        }
        
    }
}

void Event::drawEBRectHits() {
    
    ebRect.draw();
}


void Event::updateEBRec() {
    
    
}

void Event::drawCluster() {
    
    cluster.draw();
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
        vParticle[i].update();
    }
    
}
void Event::drawParticles() {
    for (int i = 0; i < vParticle.size(); i++) {
        vParticle[i].draw();
    }
    
}
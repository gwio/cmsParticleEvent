#pragma once

#include "ofMain.h"
#include "EventParticle.h"
#include "ofxTonic.h"


using namespace Tonic;


class Event{
    
public:
    
    Event();
    Event(ofVboMesh, ofVboMesh, ofVboMesh, ofVboMesh, vector<ofPolyline>);
    
    
    
    void drawCurves();
    void drawPoints();
    void drawHERectHits();
    void drawCluster();
    void updateCluster();
    void updateParticle();
    void startAnimation();
    void drawParticles();
    
    ofVboMesh meshPos1;
    ofVboMesh meshDir1;
    ofVboMesh meshPos2;
    ofVboMesh meshDir2;
    
    ofVboMesh heRect;
    ofVboMesh cluster;
    vector<float> clusterColor;
    
    vector<ofVec3f> vMeshPos1;
    vector<ofVec3f> vMeshDir1;
    vector<ofVec3f> vMeshPos2;
    vector<ofVec3f> vMeshDir2;
    
    
    vector<ofPolyline> meshCurves;
    
    vector<EventParticle> vParticle;
};
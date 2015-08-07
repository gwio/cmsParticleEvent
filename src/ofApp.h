#pragma once

#include "ofMain.h"
#include "Event.h"
#include "EventParticle.h"
#include "ofxTonic.h"



using namespace Tonic;

class ofApp : public ofBaseApp{
    
    ofxTonicSynth mainOut;
    BufferPlayer player;
    ControlTrigger mainBang;
    SampleTable recordSample, grainSample;
    RingBuffer recordBuffer;
    Generator synth;
    

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    void audioRequested (float * output, int bufferSize, int nChannels);

    void audioReceived(float* input, int bufferSize, int nChannels);
    
    ofPolyline curveFomula( ofVec3f _a, ofVec3f _b, ofVec3f _c, ofVec3f _d );
    void loadEvent(string);
   
    float pt, phi, eta;
    
    float easing(float,float);
    ofMesh tracks2;
    
    ofEasyCam cam;
    ofNode camNode;
   
    float camDist;
    
    float pointScale;
    float pointScaleDir = 50;

    bool useCurves;
    bool useSiPixelCluster;
    bool useHERecHits;
    bool useEBRecHits;
    
    ofSpherePrimitive sphere;
    ofCylinderPrimitive cylinder;
    
    vector<Event> vEventData;
    
    string last_event;
    
    ofLight light;
    
};

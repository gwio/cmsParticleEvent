#pragma once

#include "ofMain.h"
#include "Event.h"
#include "EventParticle.h"
#include "ofxTonic.h"

#define SAMPLESIZE 44100*6


using namespace Tonic;

class ofApp : public ofBaseApp{
    
    ofxTonicSynth mainOut;
    BufferPlayer player;
    ControlTrigger mainBang;
    SampleTable recordSample, grainSample;
    RingBuffer recordBuffer;
    Generator synth;
    
    float recordFloats[SAMPLESIZE];
    
    int recordPos;
    bool record;

    
    vector<TonicFloat> sampleFloats;

    float mainVol;
    
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

    void saveRecord();
   
    float pt, phi, eta;
    
    ofEasyCam cam;
    ofNode camNode;
   
    
    float pointScale;
    float pointScaleDir = 50;

    
    
    ofSpherePrimitive sphere;
    ofCylinderPrimitive cylinder;
    
    vector<Event> vEventData;
    
    string last_event;
    
    ofLight light;
    
    
};

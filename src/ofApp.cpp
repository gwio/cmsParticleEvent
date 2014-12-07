#include "ofApp.h"
#include <string>

#define EVENTSCALE 400


//--------------------------------------------------------------
void ofApp::setup(){
    
    //use ig data?
    useCurves = true;
    useSiPixelCluster = false;
    useHERecHits =true;
    useEBRecHits = true;
    
    
    //setup tonic
    ofSoundStreamSetup(2, 2, this, 44100, 512, 4);
    
    mainVol = 0.5;
    
    recordSample = SampleTable(SAMPLESIZE);
    
    player.setBuffer(recordSample).loop(false).trigger(mainBang);
    
    synth = synth + player;
    
    synth = Reverb().input(synth).roomSize(0.4).wetLevel(0.5).dryLevel(0.5);
    
    
    mainOut.setOutputGen(synth*mainVol);
    
    record = false;
    
    for (int i = 0; i < SAMPLESIZE; i++) {
        recordFloats[i] = 0.0;
    }
    
    
    //OF setup
    
    ofBackground(0);
    ofEnableAlphaBlending();
    //ofDisableAntiAliasing();
    ofSetFrameRate(60);
    
    pointScale = 100;
    pointScaleDir = 100;
    
    vEventData.clear();
    
    last_event = "empty";
    
    glPointSize(8);
    cam.setPosition(0, 0, -200);
    cam.lookAt(ofVec3f(0,0,0));
    
    light.setPosition(0,0,-200);
    // ofEnableLighting();
    ofEnableDepthTest();
    
    tracks2.clear();
    tracks2.setMode(OF_PRIMITIVE_POINTS);
    
    glPointSize(4);
}

//--------------------------------------------------------------
void ofApp::update(){
    if (vEventData.size() > 0) {
        
        if(useCurves)
            vEventData[vEventData.size()-1].updateParticle();
        
        if(useSiPixelCluster)
            vEventData[vEventData.size()-1].updateCluster();
        
    }
    
    // cam.rotate(0.04, 0, 0, 1);
    
}



//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient( ofColor::whiteSmoke,ofColor::lightCyan);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
    ofDrawBitmapString(last_event, 20,40);
    ofDrawBitmapString(ofToString(recordPos)+" / "+ofToString(SAMPLESIZE), 20,60);
    
    ofSetColor(0);
    cam.begin();
    //light.enable();
    
    
    if (vEventData.size() > 0) {
        
        if(useCurves)
            vEventData[vEventData.size()-1].drawCurves();
        //vEventData[vEventData.size()-1].drawPoints();
        
        if(useCurves)
            vEventData[vEventData.size()-1].drawParticles();
        
        if(useHERecHits)
            vEventData[vEventData.size()-1].drawHERectHits();
        
        
        if(useEBRecHits)
            vEventData[vEventData.size()-1].drawEBRectHits();
        
        if(useSiPixelCluster);
        vEventData[vEventData.size()-1].drawCluster();
        
    }
    
    //tracks2.draw();
    cam.end();
    
}

//--------------------------------------------------------------

void ofApp::loadEvent(string _path) {
    
    last_event = _path;
    Event tempE;
    
    tempE.meshPos1.clear();
    tempE.meshPos2.clear();
    tempE.vMeshPos1.clear();
    tempE.vMeshDir1.clear();
    tempE.vMeshPos2.clear();
    tempE.vMeshDir2.clear();
    
    tempE.heRect.clear();
    tempE.ebRect.clear();
    tempE.cluster.clear();
    
    tempE.cluster.setMode(OF_PRIMITIVE_POINTS);
    tempE.heRect.setMode(OF_PRIMITIVE_TRIANGLES);
    tempE.ebRect.setMode(OF_PRIMITIVE_TRIANGLES);
    tempE.meshPos1.setMode(OF_PRIMITIVE_POINTS);
    tempE.meshPos2.setMode(OF_PRIMITIVE_POINTS);
    
    
    
    ofBuffer buffer = ofBufferFromFile(_path);
    int counter = 0;
    bool extraFound = false;
    bool HERecHitsFound = false;
    bool clusterFound = false;
    bool EBRectHitsFound = false;
    
    while ( !buffer.isLastLine() && (!extraFound || !HERecHitsFound || !clusterFound) ) {
        string line = buffer.getNextLine();
        counter++;
        
        //get SiPixelClusters
        
        if(useSiPixelCluster){
            if  (  ofIsStringInString(line,"\"SiPixelClusters_V1\":") && !ofIsStringInString(line, "detid") )  {
                bool isFirstLine = true;
                clusterFound = true;
                while (line != "]") {
                    if (isFirstLine) {
                        line.erase(0,20);
                        isFirstLine = false;
                    }
                    
                    
                    int i = 0;
                    while ((i = line.find_first_of( "()[]", i)) != std::string::npos) {
                        line.erase(i, 1);
                    }
                    
                    vector<string> points = ofSplitString(line, ",");
                    
                    ofVec3f p1 = ofVec3f( ofToFloat(points[1]), ofToFloat(points[2]), ofToFloat(points[3]) );
                    tempE.cluster.addVertex(p1*EVENTSCALE);
                    tempE.cluster.addColor(ofColor::white);
                    tempE.clusterColor.push_back(ofRandom(255));
                    
                    
                    line = buffer.getNextLine();
                }
            }
        }
        
        //load EBRecHits
        
        if (useEBRecHits) {
            if  (  ofIsStringInString(line,"\"EBRecHits_V2\":") && !ofIsStringInString(line, "energy") )  {
                bool isFirstLine = true;
                EBRectHitsFound = true;
                while (line != "]") {
                    if (isFirstLine) {
                        line.erase(0,14);
                        isFirstLine = false;
                    }
                    
                    
                    int i = 0;
                    while ((i = line.find_first_of( "()[]", i)) != std::string::npos) {
                        line.erase(i, 1);
                    }
                    
                    vector<string> points = ofSplitString(line, ",");
                    
                    if( ofToFloat(points[0]) > 0.25) {
                        
                        float energy = ofToFloat(points[0]);
                        
                        ofVec3f front1 = ofVec3f( ofToFloat(points[5]), ofToFloat(points[6]), ofToFloat(points[7]) );
                        tempE.ebRect.addVertex(front1*EVENTSCALE);
                        tempE.ebRect.addColor(ofColor::tomato);
                        ofVec3f front2 = ofVec3f( ofToFloat(points[8]), ofToFloat(points[9]), ofToFloat(points[10]) );
                        tempE.ebRect.addVertex(front2*EVENTSCALE);
                        tempE.ebRect.addColor(ofColor::tomato);
                        ofVec3f front3 = ofVec3f( ofToFloat(points[11]), ofToFloat(points[12]), ofToFloat(points[13]) );
                        tempE.ebRect.addVertex(front3*EVENTSCALE);
                        tempE.ebRect.addColor(ofColor::tomato);
                        ofVec3f front4 = ofVec3f( ofToFloat(points[14]), ofToFloat(points[15]), ofToFloat(points[16]) );
                        tempE.ebRect.addVertex(front4*EVENTSCALE);
                        tempE.ebRect.addColor(ofColor::tomato);
                        ofVec3f back1 = ofVec3f( ofToFloat(points[17]), ofToFloat(points[18]), ofToFloat(points[19]) )  ;
                        ofVec3f dir1 = front1 + (back1-front1).normalize()*energy;
                        tempE.ebRect.addVertex(dir1*EVENTSCALE);
                        tempE.ebRect.addColor(ofColor::tomato);
                        ofVec3f back2 = ofVec3f( ofToFloat(points[20]), ofToFloat(points[21]), ofToFloat(points[22]) );
                        ofVec3f dir2 = front2 + (back2-front2).normalize()*energy;
                        tempE.ebRect.addVertex(dir2*EVENTSCALE);
                        tempE.ebRect.addColor(ofColor::tomato);
                        ofVec3f back3 = ofVec3f( ofToFloat(points[23]), ofToFloat(points[24]), ofToFloat(points[25]) );
                        ofVec3f dir3 = front3 + (back3-front3).normalize()*energy;
                        tempE.ebRect.addVertex(dir3*EVENTSCALE);
                        tempE.ebRect.addColor(ofColor::tomato);
                        ofVec3f back4 = ofVec3f( ofToFloat(points[26]), ofToFloat(points[27]), ofToFloat(points[28]) );
                        ofVec3f dir4 = front4 + (back4-front4).normalize()*energy;
                        tempE.ebRect.addVertex(dir4*EVENTSCALE);
                        tempE.ebRect.addColor(ofColor::tomato);
                        
                    }
                    line = buffer.getNextLine();
                }
            }
        }
        
        
        //get HCAL Endcap Rect Hits
        
        if (useHERecHits) {
            if  (  ofIsStringInString(line,"\"HERecHits_V2\":") && !ofIsStringInString(line, "energy") )  {
                bool isFirstLine = true;
                HERecHitsFound = true;
                while (line != "]") {
                    if (isFirstLine) {
                        line.erase(0,14);
                        isFirstLine = false;
                    }
                    
                    
                    int i = 0;
                    while ((i = line.find_first_of( "()[]", i)) != std::string::npos) {
                        line.erase(i, 1);
                    }
                    
                    vector<string> points = ofSplitString(line, ",");
                    
                    if( ofToFloat(points[0]) > 0.75) {
                        float energy = ofToFloat(points[0])*0.125;

                        
                        ofVec3f front1 = ofVec3f( ofToFloat(points[5]), ofToFloat(points[6]), ofToFloat(points[7]) );
                        tempE.heRect.addVertex(front1*EVENTSCALE);
                        tempE.heRect.addColor(ofColor::turquoise);
                        ofVec3f front2 = ofVec3f( ofToFloat(points[8]), ofToFloat(points[9]), ofToFloat(points[10]) );
                        tempE.heRect.addVertex(front2*EVENTSCALE);
                        tempE.heRect.addColor(ofColor::turquoise);
                        ofVec3f front3 = ofVec3f( ofToFloat(points[11]), ofToFloat(points[12]), ofToFloat(points[13]) );
                        tempE.heRect.addVertex(front3*EVENTSCALE);
                        tempE.heRect.addColor(ofColor::turquoise);
                        ofVec3f front4 = ofVec3f( ofToFloat(points[14]), ofToFloat(points[15]), ofToFloat(points[16]) );
                        tempE.heRect.addVertex(front4*EVENTSCALE);
                        tempE.heRect.addColor(ofColor::turquoise);
                        ofVec3f back1 = ofVec3f( ofToFloat(points[17]), ofToFloat(points[18]), ofToFloat(points[19]) );
                        ofVec3f dir1 = front1 + (back1-front1).normalize()*energy;
                        tempE.heRect.addVertex(dir1*EVENTSCALE);
                        tempE.heRect.addColor(ofColor::turquoise);
                        ofVec3f back2 = ofVec3f( ofToFloat(points[20]), ofToFloat(points[21]), ofToFloat(points[22]) );
                        ofVec3f dir2 = front2 + (back2-front2).normalize()*energy;
                        tempE.heRect.addVertex(dir2*EVENTSCALE);
                        tempE.heRect.addColor(ofColor::turquoise);
                        ofVec3f back3 = ofVec3f( ofToFloat(points[23]), ofToFloat(points[24]), ofToFloat(points[25]) );
                        ofVec3f dir3 = front3 + (back3-front3).normalize()*energy;
                        tempE.heRect.addVertex(dir3*EVENTSCALE);
                        tempE.heRect.addColor(ofColor::turquoise);
                        ofVec3f back4 = ofVec3f( ofToFloat(points[26]), ofToFloat(points[27]), ofToFloat(points[28]) );
                        ofVec3f dir4 = front4 + (back4-front4).normalize()*energy;
                        tempE.heRect.addVertex(dir4*EVENTSCALE);
                        tempE.heRect.addColor(ofColor::turquoise);
                        
                    }
                    
                    line = buffer.getNextLine();
                }
            }
        }
        
        
        //get extras - curves
        if (useCurves){
            if  (  ofIsStringInString(line,"\"Extras_V1\":") && !ofIsStringInString(line, "pos_1") )  {
                bool isFirstLine = true;
                extraFound = true;
                while (line != "]") {
                    if (isFirstLine) {
                        line.erase(0,12);
                        isFirstLine = false;
                    }
                    
                    
                    int i = 0;
                    while ((i = line.find_first_of( "()[]", i)) != std::string::npos) {
                        line.erase(i, 1);
                    }
                    
                    vector<string> points = ofSplitString(line, ",");
                    
                    ofVec3f pos1T = ofVec3f( ofToFloat(points[0]), ofToFloat(points[1]), ofToFloat(points[2]) );
                    tempE.vMeshPos1.push_back(pos1T);
                    ofVec3f dir1T = ofVec3f( ofToFloat(points[3]), ofToFloat(points[4]), ofToFloat(points[5]) );
                    tempE.vMeshDir1.push_back(dir1T);
                    ofVec3f pos2T = ofVec3f( ofToFloat(points[6]), ofToFloat(points[7]), ofToFloat(points[8]) );
                    tempE.vMeshPos2.push_back(pos2T);
                    ofVec3f dir2T = ofVec3f( ofToFloat(points[9]), ofToFloat(points[10]), ofToFloat(points[11]) );
                    tempE.vMeshDir2.push_back(dir2T);
                    
                    line = buffer.getNextLine();
                }
            }
        }
    }
    cout << tempE.vMeshPos1.size() << endl;
    cout << counter << endl;
    
    
    //make mesh
    for (int i = 0; i< tempE.vMeshPos1.size(); i++) {
        tempE.meshPos1.addVertex(tempE.vMeshPos1[i]*EVENTSCALE);
        tempE.meshPos1.addColor(ofColor::seaGreen);
        
        tempE.meshPos2.addVertex(tempE.vMeshPos2[i]*EVENTSCALE);
        tempE.meshPos2.addColor(ofColor::red);
    }
    
    //make index for HERect
    for (int i = 0; i < tempE.heRect.getNumVertices(); i+=8) {
        tempE.heRect.addIndex(i);
        tempE.heRect.addIndex(i+1);
        tempE.heRect.addIndex(i+2);
        
        tempE.heRect.addIndex(i+2);
        tempE.heRect.addIndex(i+3);
        tempE.heRect.addIndex(i);
        
        tempE.heRect.addIndex(i+4);
        tempE.heRect.addIndex(i+5);
        tempE.heRect.addIndex(i+6);
        
        tempE.heRect.addIndex(i+6);
        tempE.heRect.addIndex(i+7);
        tempE.heRect.addIndex(i+4);
        
        tempE.heRect.addIndex(i+1);
        tempE.heRect.addIndex(i+5);
        tempE.heRect.addIndex(i+6);
        
        tempE.heRect.addIndex(i+6);
        tempE.heRect.addIndex(i+2);
        tempE.heRect.addIndex(i+1);
        
        tempE.heRect.addIndex(i+2);
        tempE.heRect.addIndex(i+6);
        tempE.heRect.addIndex(i+7);
        
        tempE.heRect.addIndex(i+7);
        tempE.heRect.addIndex(i+3);
        tempE.heRect.addIndex(i+2);
        
        tempE.heRect.addIndex(i+3);
        tempE.heRect.addIndex(i+7);
        tempE.heRect.addIndex(i+4);
        
        tempE.heRect.addIndex(i+4);
        tempE.heRect.addIndex(i);
        tempE.heRect.addIndex(i+3);
        
        tempE.heRect.addIndex(i);
        tempE.heRect.addIndex(i+4);
        tempE.heRect.addIndex(i+5);
        
        tempE.heRect.addIndex(i+5);
        tempE.heRect.addIndex(i+1);
        tempE.heRect.addIndex(i);
        
    }
    
    //make index for HERect
    for (int i = 0; i < tempE.ebRect.getNumVertices(); i+=8) {
        tempE.ebRect.addIndex(i);
        tempE.ebRect.addIndex(i+1);
        tempE.ebRect.addIndex(i+2);
        
        tempE.ebRect.addIndex(i+2);
        tempE.ebRect.addIndex(i+3);
        tempE.ebRect.addIndex(i);
        
        tempE.ebRect.addIndex(i+4);
        tempE.ebRect.addIndex(i+5);
        tempE.ebRect.addIndex(i+6);
        
        tempE.ebRect.addIndex(i+6);
        tempE.ebRect.addIndex(i+7);
        tempE.ebRect.addIndex(i+4);
        
        tempE.ebRect.addIndex(i+1);
        tempE.ebRect.addIndex(i+5);
        tempE.ebRect.addIndex(i+6);
        
        tempE.ebRect.addIndex(i+6);
        tempE.ebRect.addIndex(i+2);
        tempE.ebRect.addIndex(i+1);
        
        tempE.ebRect.addIndex(i+2);
        tempE.ebRect.addIndex(i+6);
        tempE.ebRect.addIndex(i+7);
        
        tempE.ebRect.addIndex(i+7);
        tempE.ebRect.addIndex(i+3);
        tempE.ebRect.addIndex(i+2);
        
        tempE.ebRect.addIndex(i+3);
        tempE.ebRect.addIndex(i+7);
        tempE.ebRect.addIndex(i+4);
        
        tempE.ebRect.addIndex(i+4);
        tempE.ebRect.addIndex(i);
        tempE.ebRect.addIndex(i+3);
        
        tempE.ebRect.addIndex(i);
        tempE.ebRect.addIndex(i+4);
        tempE.ebRect.addIndex(i+5);
        
        tempE.ebRect.addIndex(i+5);
        tempE.ebRect.addIndex(i+1);
        tempE.ebRect.addIndex(i);
        
    }

    
    for (int i = 0; i < tempE.vMeshPos1.size(); i++) {
        tempE.meshCurves.push_back( curveFomula(tempE.vMeshPos1[i]*EVENTSCALE, tempE.vMeshDir1[i]*EVENTSCALE, tempE.vMeshPos2[i]*EVENTSCALE, tempE.vMeshDir2[i]*EVENTSCALE));
    }
    
    
    
    for (int i = 0; i < tempE.vMeshPos1.size(); i++) {
        tempE.vParticle.push_back(EventParticle());
        tempE.vParticle[i].setup(tempE.meshPos1.getVertices().at(i), tempE.meshCurves[i]);
        float lphp = ofMap(i, 0, tempE.vMeshPos1.size(), 30, 9000);
        
        
        int gSize =  SAMPLESIZE/2;
        
        bool isSlow;
        isSlow = true;
        
        tempE.vParticle[i].linkSample(recordFloats, lphp, isSlow, gSize, SAMPLESIZE);
    }
    
    vEventData.push_back(tempE);
    
    
    
    for (int i = 0; i < vEventData[vEventData.size()-1].vParticle.size(); i++) {
        synth = synth + vEventData[vEventData.size()-1].vParticle[i].toneOut;
    }
    
    mainOut.setOutputGen(synth*0.8);
    
}

//--------------------------------------------------------------
ofPolyline ofApp::curveFomula(ofVec3f _a, ofVec3f _b, ofVec3f _c, ofVec3f _d) {
    
    ofPolyline temp;
    
    int _stepIndex = 30;
    
    // FŸr Linie
    // Es kommt von Source Code iSPY.
    float _eps = 0.05;
    ofVec3f controlPoint1 = _b;
    ofVec3f controlPoint2 = _d;
    
    float paramA = _d.normalize().dot(_b.normalize());
    float paramC = (_c-_a).normalize().dot((_c-_a).normalize());
    float paramD = _d.normalize().dot(_b.cross((_c-_a).normalize()));
    
    ofVec3f midpt4s = (_a+_c)/2.F;
    if ( std::abs(paramC) < _eps || (std::abs(paramA-1) < _eps && std::abs(paramD/paramC) < _eps) ) {
        //        controlPoint1 = midpt4s;
        //        controlPoint2 = midpt4s;
        _stepIndex = 1;
    }
    
    ofVec3f perp = _b.cross(_d);
    if (std::abs(perp.dot(perp)) < _eps) {
        //        controlPoint1 = midpt4s;
        //        controlPoint2 = midpt4s;
        _stepIndex = 1;
    }
    
    
    
    // FŸr Linie Draw
    // Es kommt von http://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/opengl_programming.html
    vector<ofVec3f> _vtest;
    
    ofPopStyle();
    
    _vtest.resize(_stepIndex);
    for(int i=0;i<_stepIndex;i++) {
        
        float t = (float)i/(_stepIndex-1);
        
        // calculate blending functions
        float b0 =  2*t*t*t - 3*t*t + 1;
        float b1 = -2*t*t*t + 3*t*t;
        float b2 = t*t*t - 2*t*t + t;
        float b3 = t*t*t - t*t;
        
        // calculate the x,y and z of the curve point
        float x = b0*_a.x + b1*_c.x + b2*controlPoint1.x + b3*controlPoint2.x ;
        float y = b0*_a.y + b1*_c.y + b2*controlPoint1.y + b3*controlPoint2.y ;
        float z = b0*_a.z + b1*_c.z + b2*controlPoint1.z + b3*controlPoint2.z ;
        
        _vtest[i] = ofVec3f(x,y,z);
    }
    
    
    
    temp.addVertex(_a);
    for (int i=1; i<_stepIndex-1; i++) {
        temp.addVertex(_vtest[i]);
    }
    temp.addVertex(_c);
    
    
    return temp;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
    if (key == '1') {
        glPointSize(2);
        cam.setPosition(0, 0, -1400);
        cam.lookAt(ofVec3f(0,0,0));
    }
    
    if (key == '2') {
        glPointSize(6);
        cam.setPosition(0, 0, -700);
        cam.lookAt(ofVec3f(0,0,0));
        
        
        
    }
    
    if (key == '3') {
        glPointSize(8);
        cam.setPosition(0, 0, -200);
        cam.lookAt(ofVec3f(0,0,0));
        
    }
    
    if (key == 's') {
        
        if (vEventData.size() > 0) {
            vEventData[vEventData.size()-1].startAnimation();
        }
    }
    
    if (key == 'd') {
        ofImage pix;
        pix.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
        pix.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        pix.saveImage( ofGetTimestampString()+"debug.png");
    }
    
    if (key == 'e') {
        mainBang.trigger();
    }
    
    if (key == 'r') {
        recordPos =0;
        record = true;
    }
}

//_________________
void ofApp::saveRecord(){
    TonicFloat * lala = recordSample.dataPointer();
    
    for (int i =0; i < SAMPLESIZE; i++) {
        *lala++ = recordFloats[i];
    }
    
    cout << "ready"<< endl;
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
    for (int i = 0; i < dragInfo.files.size(); i++) {
        loadEvent(dragInfo.files[i]);
    }
    
}

void ofApp::audioReceived(float* input, int bufferSize, int nChannels){
    
    if (record) {
        for (int i=0; i<bufferSize*nChannels; i++) {
            if(recordPos<SAMPLESIZE) {
                recordFloats[recordPos++] = input[i];
            } else if(record){
                record = false;
                saveRecord();
            }
        }
    }
    
}

void ofApp::audioRequested (float * output, int bufferSize, int nChannels){
    mainOut.fillBufferOfFloats(output, bufferSize, nChannels);
}
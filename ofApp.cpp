#include "ofApp.h"








//--------------------------------------------------------------
void ofApp::setup(){
    
    
    ofSetFrameRate(60);
    
    
    sampleRate = 44100;
    channelOut = 0;
    numBuffers = 4;
    channelIn = 1;
    
    
    ofSoundStreamSetup(channelOut, channelIn, this, sampleRate, beat.getBufferSize(), numBuffers);
    
    
    gui.setup();
    gui.setPosition(30, 150);
    
    
    
    params.add(damping.set("Damping", 0, 0, 1.0 ));
    params.add(bandpicker.set("BandNumber", 0, 0, FFT_SUBBANDS));
    gui.add(params);
    gui.add(beat.beatParams);
}
//--------------------------------------------------------------
void ofApp::update(){
    beat.update(ofGetElapsedTimef());
    
    for (int i = 0 ; i < FFT_SUBBANDS; i++) {
        beat.setDamping(damping, i);
    }
    //cout << beat.checkBeat(2) << endl;

}

//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw();
    drawFftData(ofGetWidth()/2, ofGetHeight()/2);
    drawAverageEnergy(ofGetWidth()/2, ofGetHeight()/2);
    drawBeatDetect(ofGetWidth()/2, ofGetHeight()/2, bandpicker);
}



void ofApp::audioReceived(float *input, int bufferSize, int nChannels){
    beat.audioReceived(input, bufferSize, nChannels);
}



void ofApp::drawAverageEnergy(float width, float height){
    float widthPerBand = width / ((float)(FFT_SUBBANDS + 4.0));
    
    //Hoehen EnergyAvg
    for (int i =  (FFT_SUBBANDS/3) * 2; i < FFT_SUBBANDS ; i++) {
        ofSetColor(255, 0, 0);
        ofDrawRectangle(460 + (i *widthPerBand), (height + 250), widthPerBand, -(beat.getAverageEnergy(i) * beat.beatValueArray[i]) * beat.multiplier);
    }
  
    //Mitten EnergyAvg
    for (int i =  (FFT_SUBBANDS/3); i < (FFT_SUBBANDS/3) * 2 ; i++){
        ofSetColor(255, 0, 0);
        ofDrawRectangle(400 + (i *widthPerBand), (height + 250), widthPerBand, -(beat.getAverageEnergy(i) * beat.beatValueArray[i] * beat.multiplier));
    }
    
    //Tiefen EnergyAvg
    for (int i =  0; i < (FFT_SUBBANDS/3); i++){
        ofSetColor(255, 0, 0);
        ofDrawRectangle(320 + (i *widthPerBand), (height + 250), widthPerBand, -(beat.getAverageEnergy(i) * beat.beatValueArray[i]) * beat.multiplier);
    }
}


void ofApp::drawFftData(float width, float height){
    ofPushMatrix();
    
    float widthPerBand = width / ((float)(FFT_SUBBANDS + 4.0));
    
    int totalBands = (FFT_SUBBANDS + 4);
    
    int h = -height / 2.0f;
    
    ofSetColor(255, 255, 255, 255);
    // Hoehen
    ofDrawBitmapStringHighlight("Hoehen", 440 + height, ofGetHeight() - 90 );
    for (int i =  (FFT_SUBBANDS/3) * 2; i < FFT_SUBBANDS ; i++){
        if (bandpicker == i) {
            ofSetColor(0, 0, 200);
        }else{
            ofSetColor(0, 0, 0);
        }
        ofDrawRectangle(460 + (i * widthPerBand), 250 + height, widthPerBand, beat.getBand(i) *h );
    }
    // Mitten
    ofDrawBitmapStringHighlight("Mitten", 220 + height, ofGetHeight() - 90 );
    for (int i =  (FFT_SUBBANDS/3); i < (FFT_SUBBANDS/3) * 2 ; i++){
        if (bandpicker == i) {
            ofSetColor(0, 0, 200);
        }else{
            ofSetColor(0, 0, 0);
        }
        ofDrawRectangle(400 + (i * widthPerBand), 250 + height, widthPerBand, beat.getBand(i) *h);
    }
    // Tiefen
    ofDrawBitmapStringHighlight("Tiefen", 0 + height, ofGetHeight() - 90 );
    for (int i = 0 ; i < FFT_SUBBANDS/3; i++) {
        if (bandpicker == i) {
            ofSetColor(0, 0, 200);
        }else {
            ofSetColor(0, 0, 0);
        }
            ofDrawRectangle(320 + (i * widthPerBand), 250 + height, widthPerBand, beat.getBand(i) *h);
    }

    ofPopMatrix();
}


void ofApp::drawBeatDetect(float width, float height, int bandnumber){
    if (beat.checkBeat(bandnumber)) {
        ofSetColor(0, 255, 255);
        ofDrawRectangle(width, height, 100, 100);
    }

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

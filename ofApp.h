#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "BeatDetector.hpp"

class ofApp : public ofBaseApp{
    BeatDetector beat;
	
    public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    int sampleRate;
    int channelIn;
    int channelOut;
    int numBuffers;
    
    
    void audioReceived(float * input, int bufferSize, int nChannels);
    void drawFftData(float width, float height);
    void drawAverageEnergy(float width, float height);
    void drawBeatDetect(float width, float height, int bandpicker);
    
    
    ofxPanel gui;
    ofParameterGroup params;
    ofParameter<float>damping;
    ofParameter<float>multiplier;
    ofParameter<int>bandpicker;
};

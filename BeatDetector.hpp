//
//  BeatDetector.hpp
//  Bachelorarbeit
//
//  Created by Maximilian Deistler on 14.11.17.
//
//

#ifndef BeatDetector_hpp
#define BeatDetector_hpp



#include "ofMain.h"
#include "fft.h"

#define FFT_BINS 512
#define FFT_SUBBANDS 64
#define ENERGY_HISTORY 50


#endif /* BeatDetector_hpp */

class BeatDetector {
    fft myFFt;
public:
   
    float fftSmoothed[FFT_BINS];
    float averageEnergy[FFT_SUBBANDS];
    float fftVariance[FFT_SUBBANDS];
    float fftSubbands[FFT_SUBBANDS];
    float beatValueArray[FFT_SUBBANDS];
    float energyHistory[FFT_SUBBANDS][ENERGY_HISTORY];
    
    float * in_fft;
    
    int buffersize;
    int fftsize;
    int historyPos;
    
    bool usefft;
    bool detectbeat;
    
    
    void updateFft();

    
    BeatDetector();
    ~BeatDetector();
    void enableBeatDetect(){detectbeat = true;}
    void disableBeatDetect(){ detectbeat = false;}
    void audioReceived(float* input, int bufferSize, int nChannels);
    
    
    void update(int time);
    void setDamping(float damping, int subband);
    void findPeak(int binMin, int binMax);
    void clearFFT();

    bool checkBeat(int subband);

    int getBufferSize();
    
    float getBand(int subband);
    float getMagnitude();
    float getInFft();
    float getAverageEnergy(int subband);
    float getSmoothedValue(float smoothamount, int subband);
    
    float *magnitude;
    float *phase;
    float *power;
    float *audio_input;
    float *magnitude_average;
    
    ofParameterGroup beatParams;
    ofParameter<float>multiplier;
    ofParameter<float>smooting;
    //ofParameter<float>
    
private:
    
    float smoothedValue;
    
    


};
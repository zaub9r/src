//
//  BeatDetector.cpp
//  Bachelorarbeit
//
//  Created by Maximilian Deistler on 14.11.17.
//
//

#include "BeatDetector.hpp"



BeatDetector::BeatDetector(){

    
    beatParams.add(multiplier.set("Threshholdmultiplier", 1.0, 0, 2.0));
    
    
    
    smoothedValue = 0.0;
    
    
    usefft = true;
    int historyPos = 0;
    int fftsize = 512;
    int buffersize = 1024;
    enableBeatDetect();
    
    
    for(int i = 0; i < fftsize; i++)
        fftSmoothed[i] = 0;
    
    // bidimensional array, every subband includes 43 eHis values
    for(int i = 0; i < FFT_SUBBANDS; i++){
        for(int l = 0; l < ENERGY_HISTORY; l++){
            energyHistory[i][l] = 0;
        }
            fftSubbands[i] = 0;
            averageEnergy[i] = 0;
            fftVariance[i] = 0;
            beatValueArray[i] = 0;
    }
    // Declare FFT
    audio_input = new float[buffersize];
    magnitude = new float[fftsize];
    phase = new float[fftsize];
    power = new float[fftsize];
    magnitude_average = new float[fftsize];
    
    for (int i = 0; i < fftsize; i++) {
        magnitude[i] = 0;
        phase[i] = 0;
        power[i] = 0;
        magnitude_average[i] = 0;
    }
    
}
BeatDetector::~BeatDetector(){
    clearFFT();
}

void BeatDetector::update(int time){
    updateFft();
}



void BeatDetector::updateFft(){
    if (usefft) {
        
        in_fft = magnitude;
        for (int i = 0; i < fftsize; i++) {
            //take max, either smoothed value or the incomming fft
            if (fftSmoothed[i] < in_fft[i]) {
                fftSmoothed[i] = in_fft[i];
            }
            // multiply amplitude value of recieved frequency by 0.9 & let decrease
            fftSmoothed[i] *= 0.95f;
        }

        if (detectbeat) {
            //subband calculation
            for(int i = 0; i < FFT_SUBBANDS; i++) {
                fftSubbands[i] = 0;
                
                //16
                for(int b = 0; b < fftsize/FFT_SUBBANDS; b++) {
                    //16 fft_size_bins = 1 Subband
                    //[i * 8 +1 ~ 8 ] -> 8
                    //take the sum with 8, subband value
                    //like from 0 - 7, 8 -> 15, 16 -> 23
                    fftSubbands[i] +=  in_fft[i*(fftsize/FFT_SUBBANDS)+b];
                }
                // fftSubbands[i] = Subband Energy
                //turn added frequency division into average value 0.625
                fftSubbands[i] = fftSubbands[i]*(float)FFT_SUBBANDS/(float)fftsize;
                
                //calculation of subband value
                for(int b=0; b < fftsize/FFT_SUBBANDS; b++) {
                    fftVariance[i] += pow(in_fft[i*(fftsize/FFT_SUBBANDS)+b] - fftSubbands[i], 2);
                }
                fftVariance[i] = fftVariance[i]*(float)FFT_SUBBANDS/(float)fftsize;
                
                //change fixed number c dynamically acording to dispersion value
                //c becomes coefficient of threshhold of detection
                
                // C -> Rap/Techno 1.4, Rock 1.1
                beatValueArray[i] = (-0.0025714*fftVariance[i])+1.5142857;
            }
            
            //calculate energy average
            for(int i = 0; i < FFT_SUBBANDS; i++) {
                averageEnergy[i] = 0;
                
                for(int h = 0; h < ENERGY_HISTORY; h++) {
                    //entire energy average += each subband energy history
                    averageEnergy[i] += energyHistory[i][h];
                }
                //divide total sum with history count, change to weighted average
                averageEnergy[i] /= ENERGY_HISTORY;
            }
                for(int i = 0; i < FFT_SUBBANDS; i++) {
                    //add calculated subband energy to position of historyPos of energyHistory
                    energyHistory[i][historyPos] = fftSubbands[i];
                }
                // % = modulo -> Rest
                // representing pseudo enviroment list by looping subscript of arrangement
                historyPos = (historyPos+1) % ENERGY_HISTORY;
        }
    }
}
void BeatDetector::findPeak(int binMin, int binMax){
    float max = fftSubbands[0];
    int maxIndex = 0;
    for (int i = binMin; i < binMax; i++) {
        if (fftSubbands[i] > max) {
            max = fftSubbands[i];
            maxIndex = i;
        }
    }
    cout << maxIndex << endl;
}


void BeatDetector::clearFFT(){

}




bool BeatDetector::checkBeat(int subband){
    if(fftSubbands[subband] > averageEnergy[subband] * beatValueArray[subband] * multiplier){
        return true;
    }
}


float BeatDetector::getSmoothedValue(float smoothamount, int subband ){
    smoothedValue *= smoothamount + ( 1- smoothamount) * fftSubbands[subband];
    return smoothedValue;

}


void BeatDetector::setDamping(float damping, int subband){
    fftSubbands[subband] *= damping;
}

int BeatDetector::getBufferSize(){
    return buffersize;
}

float BeatDetector::getBand(int subband){
    return fftSubbands[subband];
}

float BeatDetector::getMagnitude(){
    return * magnitude;
}

float BeatDetector::getInFft(){
    return * in_fft;
}

float BeatDetector::getAverageEnergy(int subband){
    return averageEnergy[subband];
}

// when audio input is registrated, carry out fft analysis and change magnitude arrangement
void BeatDetector::audioReceived(float* input, int bufferSize, int nChannels) {
    
    //copy *input to audio_input
    memcpy(audio_input, input, sizeof(float) * bufferSize);
    float avg_power = 0.0f;
    //analyse spectrum with fft class, becomes fft amplitude based on magnitude
    myFFt.powerSpectrum(0, (int)fftsize, audio_input, buffersize, magnitude, phase, power, &avg_power);
    
    for (int i = 0; i < fftsize; i++) {
        // square root / magnitude was too intense so ^1/2
        magnitude[i] = powf(magnitude[i], 0.5);
    }
    //calculation of average fft amplitude value
    for (int i = 0; i < fftsize; i++) {
        float x = 0.01;
        magnitude_average[i] = (magnitude[i] * x) + (magnitude_average[i] * (1 - x));
    }
}

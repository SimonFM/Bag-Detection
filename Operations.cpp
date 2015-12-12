 #include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include "Headers/Utilities.h"
#include "Headers/Defines.h"
#include "Histograms.cpp"


// This back projection code was taken from the openCvExample given to us.
void backProjectionAndThreshold(Mat * imagesToProcesse, Mat sampleBlue, int size, Mat * &result){
    Mat hls_image,hls_imageBlue, backProjProb,binary;
	
    cvtColor(sampleBlue, hls_imageBlue, CV_BGR2HLS);
    ColourHistogram histogram3D(hls_imageBlue,NUM_OF_BINS);
 
    histogram3D.NormaliseHistogram();
    for(int i = 0; i < size; i++){
        cvtColor(imagesToProcesse[i], hls_image, CV_BGR2HLS);
        backProjProb = StretchImage( histogram3D.BackProject(hls_image) );
        threshold(backProjProb, result[i], 200, 255,  CV_THRESH_BINARY );
    }
}
 
 
// gets the Mask of an page so I can only get the page when it is applied
// result contains the masked images
void getAndApplyMask(Mat * images, int size, Mat * &result){
    Mat rgb[3], thresh, erodedImage, dilatedImage;
    Mat mask;
    vector<Mat> channels;
    for(int i = 0 ; i < size ; i++){
        split(images[i],rgb);
        threshold(rgb[0],thresh, 200, 225, CV_THRESH_OTSU);
        // perform an opening 4 times
		morphologyEx(thresh, mask, MORPH_OPEN, Mat(), Point(-1,-1),NUMBER_OF_ERODES);

        // mask all the channels
        for(int j = 0 ; j < 3 ; j++){
            Mat temp_1;
            rgb[j].copyTo(temp_1,mask);
            channels.push_back(temp_1);
        }
		
        // merge all the channels in to the result.
        merge(channels,result[i]);
        channels.clear();
    }
}
  
// returns an array of all the red channels
void getRedChannels(Mat * input, int size, Mat * &redChannel){
    Mat  channels[3];
    for(int i = 0; i < size; i++){
        split(input[i],channels);
        redChannel[i] = channels[0];
    }
}


// applies canny to an array of images
void applyCanny(Mat * input, int size, Mat * output){
	for(int i = 0; i < size; i++){
		Mat gray;
		cvtColor(input[i], gray, CV_BGR2GRAY);
		Canny( gray, gray, 50, 150, 3);
		gray.convertTo(output[i], CV_8U);
	}
	
}
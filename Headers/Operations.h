 #include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

// This back projection code was taken from the openCvExample given to us.
void backProjectionAndThreshold(Mat * imagesToProcesse, Mat sampleBlue, int size, Mat * &result);
 
 
// gets the Mask of an page so I can only get the page
void getAndApplyMask(Mat * images, int size, Mat * &result);
  
// returns an array of all the red channels
void getRedChannels(Mat * input, int size, Mat * &redChannel);

// applies canny to a single image
void applyCanny(Mat * input, int size, Mat * output);

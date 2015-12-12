#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

// taken from the sample code
void FindLocalMaxima( Mat& input_image, Mat& local_maxima, double threshold_value );
  
// Modified from the sample code given to us.
int templateMatch(Mat full_image, int size ,Mat * templates);
 

// A function that gets the matched template the for each image
void templateMatchImages(Mat * full_image,int sizeB, int sizeT, Mat * templates, int * result);
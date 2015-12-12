#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <tuple>
#include "Headers/Utilities.h"

// Modified from the sample code given to us.
int templateMatch(Mat full_image, int size ,Mat * templates){
    int i  = 0, maxIndex = 0;
    Mat display_image, correlation_image;
    double min_correlation, max_correlation;
    vector<tuple<double,int>> maxCorrelations;
 
    for(i = 0; i < size; i++){
        int result_columns =  full_image.cols - templates[0].cols + 1;
        int result_rows = full_image.rows - templates[0].rows + 1;
        correlation_image.create( result_columns, result_rows, CV_32FC1 );
        matchTemplate( full_image, templates[i], correlation_image, CV_TM_CCORR_NORMED );
        minMaxLoc( correlation_image, &min_correlation, &max_correlation );
        cout.precision(17);
        maxCorrelations.push_back(make_tuple(max_correlation,i));
    }
 
    max_correlation = 0;
    maxIndex = 0;
    for(i = 0 ; i < maxCorrelations.size(); i++){
        if(max_correlation <  get<0>(maxCorrelations[i]) ) {
            maxIndex = i;
            max_correlation =  get<0>(maxCorrelations[i]);
        }
    }
    return maxIndex;
}
 

// A function that gets the matched template the for each image
void templateMatchImages(Mat * full_image,int sizeB, int sizeT, Mat * templates, int * result){
    for(int i = 0; i < sizeB; i++)
        result[i] = templateMatch(full_image[i],sizeT,templates);
 
}
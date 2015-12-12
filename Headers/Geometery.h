#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"


// transforms any given image to a given destination.
void transformImage(Mat imageToTransform,  vector<Point> srcPoints,  vector<Point> dstPoints, Mat &result);
 
// Apply Transformation to an array of images, given the points and the destinations and store those
// transformed images in result
void transformSetOfImages(Mat * imagesToTransform,  vector<Point>* srcPoints,  vector<Point> dstPoints,int size, Mat * &result);

// A function that finds the top left, top right, bottom left and the bottom
// right points in a list of points.
 vector<Point> getTemplatePoints( vector<Point> pointsInImages);
 
// A function that gets the corner points (the blue dots) from each 
// template. 
  vector<Point> getTemplateCorners(Mat sample,Mat * templateImages, int size);
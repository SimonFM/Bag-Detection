#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

// draws lines on an image for a given list of points.
void drawLines(Mat input, vector<Point> points);
 
// draws circles on an image for a given list of points.
void drawCircles(Mat input, vector<Point> points, int size);
 
// A function that displays an array of given images using the imshow function of opencv
// Resizes each image so that it would display nicer on my screen, I then show each image passed
// in to the function
void displayImages(string windowName,int size,Mat * original,Mat * masked, Mat * backProjected, Mat * matchedImages);
void displayImages(string windowName,int size,Mat * original, Mat * matchedImages);

//draws the circles and lines on an image
void drawLocationOfPage(Mat * backProjectionImages, Mat * images, int size,  vector<Point> *  whiteDots);

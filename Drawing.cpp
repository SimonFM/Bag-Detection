#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include "Headers/Utilities.h"
#include "Headers/ImageFunctions.h"
#include "Headers/Points.h"
#include "Headers/Defines.h"


// draws lines on an image for a given list of points.
void drawLines(Mat input, vector<Point> points){
    line( input,points[0],points[1],Scalar( 0, 0, 255 ),1,8 );
    line( input,points[1],points[3],Scalar( 0, 0, 255),1,8 );
    line( input,points[2],points[3],Scalar( 0, 0, 255),1,8 );
    line( input,points[2],points[0],Scalar( 0, 0, 255),1,8 );
}
 
// draws circles on an image for a given list of points.
void drawCircles(Mat input, vector<Point> points, int size){
    circle( input, points[0], 5, Scalar( 0, 0, 0),-1);
    circle( input, points[1], 5, Scalar( 0, 255, 0 ),-1);
    circle( input, points[2], 5, Scalar( 0, 0, 255 ),-1);
    circle( input, points[3], 5, Scalar( 255, 0, 0 ),-1);
}
 
// A function that displays an array of given images using the imshow function of opencv
// Resizes each image so that it would display nicer on my screen, I then show each image passed
// in to the function
void displayImages(string windowName,int size,Mat * original,Mat * masked, Mat * backProjected, Mat * matchedImages){
    Mat * display = new Mat[size];
 
    Mat * resizedOriginal = new Mat[size];
    Mat * resizedMasked = new Mat[size];
    Mat * resizedBackProjection = new Mat[size];
    Mat * resizedMatchedImages = new Mat[size];
 
 
    Mat temp;
    resize(original,size, THIRD_SIZE, resizedOriginal);
    resize(masked,size, THIRD_SIZE, resizedMasked);
    resize(backProjected,size, THIRD_SIZE, resizedBackProjection);
    resize(matchedImages,size, HALF_SIZE, resizedMatchedImages);
 
    for(int i = 0 ; i < size; i++){
		// join them all up
        cvtColor(resizedBackProjection[i],temp, CV_GRAY2BGR);
        display[i] = JoinImagesHorizontally(resizedOriginal[i],"Original Image",resizedMasked[i],"Masked Image",2);
        display[i] = JoinImagesHorizontally(display[i],"",temp,"Back Project & closing Image",2);
        display[i] = JoinImagesHorizontally(display[i],"",resizedMatchedImages[i],"Matched Image",2);
		// display them
        imshow(windowName,display[i]);
        waitKey(0);// wait for user input (Enter pressed)
    }
}

// A function that displays an array of given images using the imshow function of opencv
// Resizes each image so that it would display nicer on my screen, I then show each image passed
// in to the function
void displayImages(string windowName,int size,Mat * original, Mat * matchedImages){
    Mat * display = new Mat[size];
    Mat * resizedOriginal = new Mat[size];
    Mat * resizedMatchedImages = new Mat[size];
 
 
    Mat temp;
    resize(original,size, THIRD_SIZE, resizedOriginal);
    resize(matchedImages,size, HALF_SIZE, resizedMatchedImages);
 
    for(int i = 0 ; i < size; i++){
		// join them all up
        display[i] = JoinImagesHorizontally(resizedOriginal[i],"Original",resizedMatchedImages[i],"Matched Image",2);
		// display them
        imshow(windowName,display[i]);
        waitKey(0);// wait for user input (Enter pressed)
    }
}

//draws the circles and lines on an image
void drawLocationOfPage(Mat * backProjectionImages, Mat * images, int size,  vector<Point> *  whiteDots){
     vector<Point> temp;
    Point * p = new Point[NUM_OF_CORNERS];
    for(int i = 0; i < size; i++){
		// get the points
        temp = getWhiteDotsLocations(backProjectionImages[i]);
        p[0] =  getTopLeftPoint(temp);
        p[1] = getTopRightPoint(backProjectionImages[i],temp);
        p[2] = getBottomLeftPoint(temp);
        p[3] = getBottomRightPoint(temp);
 
		// gather the points
        for(int j = 0; j < NUM_OF_CORNERS; j++)  whiteDots[i].push_back(p[j]);
 
        drawCircles(images[i],whiteDots[i],whiteDots[i].size());
        drawLines(images[i],whiteDots[i]);
    }
}
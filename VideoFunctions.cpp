
#include "Headers/VideoFunctions.h"
#include "Headers/Defines.h"
#include "Headers/Utilities.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
 
// load the images in from a files
void loadVideosFromFile(char * fileLocation, char ** imageFiles, int size, VideoCapture * &videos){
    std::cout << "Loading some Videos..." << std::endl;
    for(int i = 0; i < size; i++){
        string filename(fileLocation);
        filename.append(imageFiles[i]);
        VideoCapture temp(filename);
        videos[i] = temp;
        if (!videos[i].isOpened())
            std::cout << "!!! Failed to open file: " << filename << std::endl;
        else std::cout << "Loaded: " << filename << std::endl;
    }
 
}
 
void applyMask(Mat toProcess, Mat mask, Mat &result){
    toProcess.copyTo(result,mask);
	//imshow("resultOfMask ",result);

}
 
//  Gray the current frame
void grayFrame(Mat currentFrame, Mat &result){
    cvtColor(currentFrame, result, CV_BGR2GRAY);
   // imshow("Gray ", result);
}
 
// Apply Edge Detection
void cannyFrame(Mat toBeGrayed, Mat result){
    Canny(toBeGrayed, result, 50, 150, 3);
    imshow("Canny",result);
	waitKey(0);
}

// this tutorial helped me understand how to implement the background
// models in OpenCV:
// http://docs.opencv.org/master/d1/dc5/tutorial_background_subtraction.html#gsc.tab=0
void backGroundModelFrame(Mat currentFrame,Ptr<BackgroundSubtractorMOG2> MOG2, Mat &result){
	RNG rng(12345);
    Mat foreGround, canny, drawing;
	
    MOG2 -> apply(currentFrame, foreGround);
    morphologyEx(foreGround, result, MORPH_ERODE, Mat(), Point(-1,-1),NUMBER_OF_ERODES);
	morphologyEx(result, result, MORPH_DILATE, Mat(), Point(-1,-1),NUMBER_OF_DILATIONS);
	
}
 
void applyContours(Mat toContour, Mat result){
	RNG rng(12345);
    Mat foreGround, canny, drawing = Mat();
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours( toContour, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	for( int i = 0; i < contours.size(); i++ ) {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }
}
 
// Process the Videos
void processVideos(int size, VideoCapture * &videos){
    std::cout << "Processing the Frames..." << std::endl;
    Mat frameToDisplay, gray, foregroundMask, canny;
    Mat mask, frameToMask, resultOfMask;
    Mat originalBackground, contours;
    Ptr<BackgroundSubtractorMOG2> MOG2 = createBackgroundSubtractorMOG2();
    for(int i = 0; i < size; i++){
 
        while(videos[i].isOpened()){ // Process the image's frame
            resultOfMask = Mat();
            if(!videos[i].read(frameToDisplay)) break; // stop processing if the final frame.
            backGroundModelFrame(originalBackground,MOG2,mask);
            applyMask(frameToDisplay,mask,resultOfMask);
			grayFrame(resultOfMask,gray);
			cannyFrame(gray, canny);
            applyContours(canny, contours);
            imshow("Masked Image ",contours);
            waitKey(30);
        }
        videos[i].release();
    }
}
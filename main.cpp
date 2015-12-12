/*
 * Simon Markham
 * 
 */
#pragma region INCLUDES
 
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <fstream>
#include <tuple>
#include "Headers/Utilities.h"
#include "Histograms.cpp"
#include "Headers/Points.h"
#include "Headers/ImageFunctions.h"
#include "Headers/Drawing.h"
#include "Headers/Operations.h"
#include "Headers/Metrics.h"
#include "Headers/VideoFunctions.h"
 
using namespace std;
using namespace cv;
#pragma endregion INCLUDES
 
#pragma region VIDEO LOCATIONS
// Location of the images in the project
char * media = "Media/";
char * videoLocations[] = {"ObjectAbandonmentAndRemoval1.avi","ObjectAbandonmentAndRemoval2.avi"};
 
#pragma endregion VIDEO LOCATIONS


// Function to run program
int main(int argc, const char** argv){
	int numberOfVideos = sizeof(videoLocations) / sizeof(videoLocations[0]);
	cv::VideoCapture * videos = new VideoCapture[numberOfVideos];
	//loadVideos(media,);
	loadVideosFromFile(media,videoLocations,numberOfVideos,videos);
	processVideos(numberOfVideos, videos);
	return 0;
}
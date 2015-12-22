#include "Headers/VideoFunctions.h"
#include "Headers/Defines.h"
#include "Headers/Utilities.h"
#include "Histograms.cpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include "Headers/MedianBackground.h"
 
// Some global values
Point2f previousCenter = Point2f(0.0, 0.0);
Mat image;

int part1TP = 0, part1FP = 0;
int part1TN = 0, part1FN = 0;

double di = 0.0;
int numberOfDi = 0.0;
vector<Rect> overallDiImages;
vector<Rect> videoDiImages;
double overallDice = 0.0;
bool abandoned = false, removed = false;
int currentFrame = 0, frameFoundIn = 0;
MatND abanadonedHist, abanadonedComparasionHist;
MatND removedHist, removedComparasionHist;
Mat firstFrame;


//
// Calculates the average Dice Cofficient for every video
//
void calcAverageDiceCofficient(){
	double result = overallDice / overallDiImages.size();
	std::cout << "Overall Average Dice Cofficient: " << result << std::endl;
	overallDiImages.clear();
}

//
// Calculates the Dice Cofficient for an individual video
//
void calcVideoDiceCoefficient(bool isVideo1){
	double result  = 0;
	Rect groundTruth = (isVideo1)? Rect(Point(356,208),Point(390,239)): groundTruth = Rect(Point(287,261),Point(352,323));
	for(int i  = 0; i < videoDiImages.size() ; i++){
		Rect overlap =  videoDiImages[i] & groundTruth ;
		result += (float)( 2 * overlap.area() ) / (videoDiImages[i].area()+ groundTruth.area());
		overallDice += result;
		overallDiImages.push_back(overlap);
		std::cout << "Event Dice Cofficient: " << (float)( 2 * overlap.area() ) / (videoDiImages[i].area()+ groundTruth.area()) << std::endl;
	}
	std::cout << "Video Average Dice Cofficient: " << result / videoDiImages.size() << std::endl;
	videoDiImages.clear();
}

//
// Prints out the metrics
//
void genereateMetrics(int video){
	std::cout << "*****************************************" << std::endl;

	if(video == 0) std::cout << "                VIDEO 1                   " << std::endl;
	else std::cout << "                VIDEO 2                   " << std::endl;

	std::cout << "TP: " << part1TP << std::endl;
	std::cout << "FP: " << part1FP << std::endl;
	std::cout << "TN: " << part1TN << std::endl;
	std::cout << "FN: " << part1FN << std::endl;

	std::cout << "Recall: " << ((double) part1TP) / ((double) (part1TP+part1FP)) << std::endl;
	std::cout << "Precision: " << ((double) part1TP) / ((double) (part1TP+part1FN)) << std::endl;

	std::cout << "*****************************************" << std::endl;
	waitKey(0);
}

// load the images in from a files
void loadVideosFromFile(char * fileLocation, char ** imageFiles, int size, VideoCapture * &videos){
    std::cout << "Loading some Videos..." << std::endl;
    for(int i = 0; i < size; i++){
        string filename(fileLocation);
        filename.append(imageFiles[i]);
        VideoCapture temp(filename);
        videos[i] = temp;
        if (!videos[i].isOpened()) std::cout << "!!! Failed to open file: " << filename << std::endl;
        else std::cout << "Loaded: " << filename << std::endl;
    }
}

//
// Compares the area bound by the points in the largest contour inside of the 
// frame passed in.
// The comparsion is done against a rect that is bigger than the original
Rect histogramComparasion(vector<Point> largestContour, Mat original){
    Mat bagMat, toCompareAgainst;

	int height = largestContour[2].y - largestContour[0].y;
	int width = largestContour[2].x - largestContour[0].x;

	bagMat = original(Rect(largestContour[0].x, largestContour[0].y, width, height));
	toCompareAgainst = original(Rect(largestContour[0].x-20, largestContour[0].y-20, width, height));
	ColourHistogram reference_histogram(bagMat,4);
	ColourHistogram comparison_histogram(toCompareAgainst,4);

	double result = compareHist(reference_histogram.getHistogram(), comparison_histogram.getHistogram(), CV_COMP_CORREL);
	string type;
	if(result >= 0.91)	type = "Removed ";
	else type = "Abandonned ";
	
	std::cout << "Similarity - " << type << result << std::endl;
	return Rect(largestContour[0].x, largestContour[0].y, width, height);
}

//
// Applies contours to a given image, from the masked image called input
// I found out how to do contours and the largest contour from opencv's wiki &
// stack overflow respectively.
vector<Point> applyContours(Mat input, Mat original){
    RNG rng(12345);
    Mat foreGround, canny;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    cvtColor(input, foreGround, CV_BGR2GRAY);
    morphologyEx(foreGround, foreGround, MORPH_ERODE, Mat(), Point(-1,-1),NUMBER_OF_ERODES);
    morphologyEx(foreGround, foreGround, MORPH_DILATE, Mat(), Point(-1,-1),NUMBER_OF_DILATIONS);
    findContours( foreGround, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    vector<RotatedRect> minRect( contours.size() );
    vector<RotatedRect> minEllipse( contours.size() );
    vector<Point> largestContour;
    // Draw contours
    int largest_area = 0;
    int largest_contour_index = 0;
    cv::Rect bounding_rect;

    Mat drawing = Mat::zeros( original.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ ){
       minRect[i] = minAreaRect( Mat(contours[i]) );
       if( contours[i].size() > 5 ) minEllipse[i] = fitEllipse( Mat(contours[i]) ); 

       double a = contourArea( contours[i],false);  //  Find the area of contour
       if(a > largest_area){
            largest_area = a;
            largest_contour_index = i;                 //Store the index of largest contour
            bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
            largestContour = contours[i];
       }
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( original, contours,largest_contour_index, color, 2, 8, hierarchy ); // Draw the largest contour using previously stored index.  
    }
	return largestContour;
}

//
// This method was copied from Ken's sample provided to us.
//
Mat getMedianDifferenceMedianModels(MedianBackground &background1,MedianBackground &background2,Mat &frameToDisplay){
    Mat diff1, diff2, diff3;
    Mat median_foreground_image_1, median_foreground_image_2;
    background1.UpdateBackground( frameToDisplay );
    Mat background1Image = background1.GetBackgroundImage();

    Mat median_difference;
    absdiff(background1Image, frameToDisplay, diff1);
    cvtColor(diff1, diff1, CV_BGR2GRAY);
    threshold(diff1,diff1,30,255,THRESH_BINARY);
    median_foreground_image_1.setTo(Scalar(0,0,0));
    frameToDisplay.copyTo(median_foreground_image_1, diff1);

    background2.UpdateBackground( frameToDisplay );
    Mat background2Image2 = background2.GetBackgroundImage();

    absdiff(background2Image2, frameToDisplay, diff2);
    cvtColor(diff2, diff2, CV_BGR2GRAY);
    threshold(diff2,diff2,30,255,THRESH_BINARY);
    median_foreground_image_2.setTo(Scalar(0,0,0));
    frameToDisplay.copyTo(median_foreground_image_2, diff2);

    absdiff(median_foreground_image_1, median_foreground_image_2, diff3);
    return diff3;
}

//
// This method was found on stackoveflow:
// http://stackoverflow.com/questions/9074202/opencv-2-centroid
//
vector<Point2f> getCentroidOfContour(vector<Point> contourPoints){
	// Get the moments
	vector<Moments> mu(contourPoints.size() );
	for( int i = 0; i < contourPoints.size(); i++ )
		mu[i] = moments( contourPoints, false ); 

	//  Get the mass centers:
	vector<Point2f> mc( contourPoints.size() );
	for( int i = 0; i < contourPoints.size(); i++ )
		mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 

	return mc;
}

//
// Determines if two points are close to each other.
//
bool withinPreviousLocationArea(Point2f previousCentroids, Point2f centroids){
	return ( previousCentroids.x >= centroids.x - 10) && ( previousCentroids.x <= centroids.x + 10)
						 && ( previousCentroids.y >= centroids.y - 10) && ( previousCentroids.y <= centroids.y + 10) ;
}
//
// Process the Video
//
void processVideos(int size, VideoCapture * &videos){
    std::cout << "Processing the Videos..." << std::endl;
    Mat frameToDisplay, current, diff3;
    Mat contouredImage, blurredImage;
    Mat output;
    VideoCapture currentVideo;
    
    for(int i = 0; i < size ; i++){
		videos[i] >> firstFrame;
        videos[i] >> current;
        currentVideo = videos[i];
        MedianBackground background1(current, LEARNING_RATE_1, 1 );
        MedianBackground background2(current, LEARNING_RATE_2, 1 );
        int frameNum = 0;
        bool currentFrameSet = false;
        vector<Point> currentLargestContourPos;
        vector<Point> previousContourPos;
        vector<Point2f> centroids;
        
        frameNum = 0;
        int range = 0;
        frameToDisplay = current;
		image = frameToDisplay;
        currentFrame = videos[i].get(CV_CAP_PROP_POS_FRAMES);
        Rect result;
        while(!frameToDisplay.empty()){ // Process the image's frame
            currentFrame = videos[i].get(CV_CAP_PROP_POS_FRAMES); // get the current frame count

            if(frameNum % FRAME == 0){ // only process a certain frame
                // convert the current frame number to a char * 
                std::stringstream s;
                s << "Frame: " << currentFrame;
                std::string frameText = s.str();
                char * writable = new char[frameText.size() + 1];
                std::copy(frameText.begin(), frameText.end(), writable);
                writable[frameText.size()] = '\0';

                // do some processing
                blur(frameToDisplay,blurredImage, cv::Size(3,3)); // blur the image
                diff3 = getMedianDifferenceMedianModels(background1,background2,blurredImage); // get the difference in the 2 age rates
                currentLargestContourPos = applyContours(diff3,frameToDisplay); // draw the location of the bag
                centroids = getCentroidOfContour(currentLargestContourPos); // gets the centre of a centroid

				if(centroids.size() == 4 ){ // only accept contours with 4 edges

					if(!currentFrameSet){ // if want to get the current frame & we haven't got it before
						frameFoundIn = videos[i].get(CV_CAP_PROP_POS_FRAMES);
						currentFrameSet = true;
					}
					//  if the objects haven't moved in the last 40 frames within a range
					else if(withinPreviousLocationArea(previousCenter, centroids[0]) && currentFrame - frameFoundIn >= FRAMES_TO_WAIT){

						// get the histogram comparasion
						Rect result = histogramComparasion(currentLargestContourPos,frameToDisplay);
						overallDiImages.push_back(result);
						videoDiImages.push_back(result);
						currentFrameSet = false;
						part1TP++;
						std::cout <<"Event occurred at " << frameNum / FRAMES_PER_SEC  << " seconds"<< std::endl;
					}
					previousCenter = centroids[0];
					
				}
				// reset it if it does not find any commmon centroids
				else if(currentFrameSet && currentFrameSet - frameFoundIn > FRAMES_TO_WAIT){
					std::cout <<"Resetting the Frame " << std::endl;
					currentFrameSet = false;
					frameFoundIn = 0;
				}

                imshow("Original",JoinImagesHorizontally(frameToDisplay,writable,Mat(),"") );
                waitKey(SPEED);
            }
            frameNum++;
			videos[i] >> frameToDisplay;
			
        }   
		// draw metrics and reset values.
		genereateMetrics(i);
		calcVideoDiceCoefficient(i == 0);
		currentLargestContourPos.clear();
		currentFrameSet = false;
		frameFoundIn = 0;
		
    }
	calcAverageDiceCofficient(); // print out all the a
	std::cout << "Press Any Key to Exit...";
	waitKey(0);
}
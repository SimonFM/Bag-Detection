#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <fstream>
#include <tuple>
#include "Headers/Utilities.h"
#include "Headers/Points.h"
#include "Headers/Operations.h"

// transforms any given image to a given destination.
void transformImage(Mat imageToTransform,  vector<Point> srcPoints,  vector<Point> dstPoints, Mat &result){
    Point2f src[4], dst[4];
	// get the points in the src to transform
    src[0] = Point2f( srcPoints[0].x, srcPoints[0].y ); // top left
    src[1] = Point2f( srcPoints[1].x, srcPoints[1].y ); // top right
    src[2] = Point2f( srcPoints[2].x, srcPoints[2].y ); // bottom left
    src[3] = Point2f( srcPoints[3].x, srcPoints[3].y ); // bottom right
	
	// this is where the new image will go.
    dst[0] = Point2f( dstPoints[3].x, dstPoints[3].y ); // top left
    dst[1] = Point2f( dstPoints[2].x, dstPoints[2].y ); // top right
    dst[2] = Point2f( dstPoints[1].x, dstPoints[1].y ); // bottom left
    dst[3] = Point2f( dstPoints[0].x, dstPoints[0].y ); // bottom right
 
    Mat perspective_matrix( 3, 3, CV_32FC1 );
    perspective_matrix = getPerspectiveTransform( src, dst );
    warpPerspective( imageToTransform, result, perspective_matrix, result.size() ); // do the transformation
}
 
// Apply Transformation to an array of images, given the points and the destinations and store those
// transformed images in result
void transformSetOfImages(Mat * imagesToTransform,  vector<Point>* srcPoints,  vector<Point> dstPoints,int size, Mat * &result){
    for(int i = 0; i < size; i++)
        transformImage(imagesToTransform[i], srcPoints[i],dstPoints,result[i]); 
}

// A function that finds the top left, top right, bottom left and the bottom
// right points in a list of points.
 vector<Point> getTemplatePoints( vector<Point> pointsInImages){
    int minX = 0, maxX =  numeric_limits<int>::max();
    int minY = 0, maxY =  numeric_limits<int>::max();
     vector<Point> cornerPoints;
    for(int i = 0; i < pointsInImages.size(); i++){
        // max values for X and Y
        if(maxX > pointsInImages[i].x) maxX = pointsInImages[i].x;
        if(maxY > pointsInImages[i].y) maxY = pointsInImages[i].y;
 
        // min values for X and Y
        if(minX < pointsInImages[i].x) minX = pointsInImages[i].x;
        if(minY < pointsInImages[i].y) minY = pointsInImages[i].y;
 
    }
    cornerPoints.push_back(Point(minX,minY));// top left
    cornerPoints.push_back(Point(maxX,minY));// top right
    cornerPoints.push_back(Point(minX,maxY));// bottom left
    cornerPoints.push_back(Point(maxX,maxY));// bottom right
    return cornerPoints;
}
 
// A function that gets the corner points (the blue dots) from each 
// template. 
 vector<Point> getTemplateCorners(Mat sample,Mat * templateImages, int size){
    Mat * mask = new Mat[size];;
    Mat * backProjectedImages = new Mat[size];
    getAndApplyMask(templateImages,size,mask);
    backProjectionAndThreshold(mask,sample,size,backProjectedImages);
     vector<Point> temp = getWhiteDotsLocations(backProjectedImages[0]);
     vector<Point> result = getTemplatePoints(temp);
    return result;
}
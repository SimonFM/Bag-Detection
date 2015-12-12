#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

// getWhiteDotsLocations
// getBottomLeftPoint
// getTopRightPoint
// getBottomRightPoint
// getTopLeftPoint
 #pragma region POINTS
 
// gets the locations of the white dots in an image
 vector<Point> getWhiteDotsLocations(Mat image ){
     vector<Point>  nonZero ;
    findNonZero(image,nonZero);
     vector<Point> result(nonZero.size());
    //for (int i = 0; i < nonZero.size(); i++) result[i] = nonZero[i];
 
    return nonZero;
}
 
//gets the bottom left corner in an image
Point getBottomLeftPoint( vector<Point> points){
    Point bottomLeft;
    int size = points.size();
    bottomLeft = points[0];
    for(int i = 0; i < size; i++){
        if( bottomLeft.x > points[i].x && points[i].x > 173)  bottomLeft = points[i];
    }
    return bottomLeft;
}
 
//gets the bottom right circle in an image
Point getTopRightPoint(Mat image, vector<Point> points){
    Point topRight;
    int size = points.size();
    topRight = points[0];
    for(int i = 0; i < size; i++){
        if( topRight.x < points[i].x && points[i].y > 100) topRight = points[i];
    }
    return topRight;
}
 
//gets the bottom right circle in an image
Point getBottomRightPoint( vector<Point> points){
    Point bottomRight;
    int size = points.size();
    bottomRight = points[0];
    for(int i = 0; i < size; i++){
        if( bottomRight.y < points[i].y && points[i].y > 100) bottomRight = points[i];
    }
    return bottomRight;
}
 
//gets the top left circle in an image
Point getTopLeftPoint(  vector<Point> points){
    Point topLeft;
    int size = points.size();
    topLeft = points[0];
    for(int i = 0; i < size; i++){
        if( topLeft.y > points[i].y && points[i].y > 100) topLeft = points[i];
    }
    //cout<< topLeft<< endl;
    return topLeft;
}
#pragma endregion POINTS
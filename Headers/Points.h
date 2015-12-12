#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;
 
// gets the locations of the white dots in an image
vector<Point> getWhiteDotsLocations(Mat image );
 
//gets the bottom left corner in an image
Point getBottomLeftPoint( vector<Point> points);
 
//gets the bottom right circle in an image
Point getTopRightPoint(Mat image, vector<Point> points);

//gets the bottom right circle in an image
Point getBottomRightPoint( vector<Point> points);
 
//gets the top left circle in an image
Point getTopLeftPoint(  vector<Point> points);


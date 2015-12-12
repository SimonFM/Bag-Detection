#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

void loadVideosFromFile(char * fileLocation, char ** imageFiles, int size, VideoCapture * &images);

void processVideos(int size, VideoCapture * &videos);
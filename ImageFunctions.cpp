#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include "Headers/Utilities.h"

using namespace std;
using namespace cv;

// A function that loads in a file path, the file names, the number of files, and
// a pointer to where those files should be stored and then stores them at that location
void loadImages(char * fileLocation, char ** imageFiles, int size, Mat * &images){
    images = new Mat[size];
 
    // This code snippet was taken from your OpenCVExample and it loads the images
    for(int i = 0 ; i < size ; i++){
        string filename(fileLocation);
        filename.append(imageFiles[i]);
        images[i] =  imread(filename, -1);
 
        if (images[i].empty()) cout << "Could not open " << filename << endl;
        //else cout << "closing: " << filename << endl;
    }
}
 
// a function that resizes an image with a given factor
void resize(Mat * image,int size, int factor, Mat * result){
    for(int i = 0; i < size; i++)
         resize(image[i],result[i], Size(image[i].cols/factor,image[i].rows/factor));
}

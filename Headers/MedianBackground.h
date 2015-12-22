// This class was taken from ken's sample code.

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "Headers/Utilities.h"

class MedianBackground{
private:
	Mat mMedianBackground;
	float * * * * mHistogram;
	float * * * mLessThanMedian;
	float mAgingRate;
	float mCurrentAge;
	float mTotalAges;
	int mValuesPerBin;
	int mNumberOfBins;
public:
	MedianBackground( Mat initial_image, float aging_rate, int values_per_bin );
	~MedianBackground();
	Mat GetBackgroundImage();
	void UpdateBackground( Mat current_frame );
	float getAgingRate(){ return mAgingRate; }
};
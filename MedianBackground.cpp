#include "MedianBackground.h"

MedianBackground::MedianBackground( Mat initial_image, float aging_rate, int values_per_bin ){
	mCurrentAge = 1.0;
	mAgingRate = aging_rate;
	mTotalAges = 0.0;
	mValuesPerBin = values_per_bin;
	mNumberOfBins = 256/mValuesPerBin;
	mMedianBackground = Mat::zeros(initial_image.size(), initial_image.type());
	mLessThanMedian = (float***) new float**[mMedianBackground.rows];
	mHistogram = (float****) new float***[mMedianBackground.rows];
	for (int row=0; (row<mMedianBackground.rows); row++)
	{
		mHistogram[row] = (float***) new float**[mMedianBackground.cols];
		mLessThanMedian[row] = (float**) new float*[mMedianBackground.cols];
		for (int col=0; (col<mMedianBackground.cols); col++)
		{
			mHistogram[row][col] = (float**) new float*[mMedianBackground.channels()];
			mLessThanMedian[row][col] = new float[mMedianBackground.channels()];
			for (int ch=0; (ch<mMedianBackground.channels()); ch++)
			{
				mHistogram[row][col][ch] = new float[mNumberOfBins];
				mLessThanMedian[row][col][ch] = 0.0;
				for (int bin=0; (bin<mNumberOfBins); bin++)
				{
					mHistogram[row][col][ch][bin] = (float) 0.0;
				}
			}
		}
	}
}

Mat MedianBackground::GetBackgroundImage(){
	return mMedianBackground;
}

void MedianBackground::UpdateBackground( Mat current_frame ){
	mTotalAges += mCurrentAge;
	float total_divided_by_2 = mTotalAges/((float) 2.0);
	for (int row=0; (row<mMedianBackground.rows); row++)
	{
		for (int col=0; (col<mMedianBackground.cols); col++)
		{
			for (int ch=0; (ch<mMedianBackground.channels()); ch++)
			{
				int new_value = (mMedianBackground.channels()==3) ? current_frame.at<Vec3b>(row,col)[ch] : current_frame.at<uchar>(row,col);
				int median = (mMedianBackground.channels()==3) ? mMedianBackground.at<Vec3b>(row,col)[ch] : mMedianBackground.at<uchar>(row,col);
				int bin = new_value/mValuesPerBin;
				mHistogram[row][col][ch][bin] += mCurrentAge;
				if (new_value < median)
					mLessThanMedian[row][col][ch] += mCurrentAge;
				int median_bin = median/mValuesPerBin;
				while ((mLessThanMedian[row][col][ch] + mHistogram[row][col][ch][median_bin] < total_divided_by_2) && (median_bin < 255))
				{
					mLessThanMedian[row][col][ch] += mHistogram[row][col][ch][median_bin];
					median_bin++;
				}
				while ((mLessThanMedian[row][col][ch] > total_divided_by_2) && (median_bin > 0))
				{
					median_bin--;
					mLessThanMedian[row][col][ch] -= mHistogram[row][col][ch][median_bin];
				}
				if (mMedianBackground.channels()==3)
					mMedianBackground.at<Vec3b>(row,col)[ch] = median_bin*mValuesPerBin;
				else mMedianBackground.at<uchar>(row,col) = median_bin*mValuesPerBin;
			}
		}
	}
	mCurrentAge *= mAgingRate;
}
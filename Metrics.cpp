#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <fstream>
#include <tuple>
#include "Headers/Utilities.h"

int groundTruths[] = {0, 1, 2, 3, 4,
					  5, 6, 7, 8, 9,
					  10, 11, 12, 1, 2,
				      4, 3, 6, 8, 7,
					  6, 10, 12, 11, 1};

// True Positive = a page is visible and recognised correctly
// False Positive = an incorrectly recognised page, where EITHER no page  was visible OR a different page was visible
// True Negative = no page visible and no page identified
// False Negative = a page is visible but no page was found
void compareAgainstGroundTruth( Mat * images, Mat * templates, int * results, int size, Mat * result){
	int TP = 0, FP = 0;
	int TN = 0, FN = 0;
	for(int i = 0; i < size; i++){
		if(results[i] == groundTruths[i]) TP++;
		else {
			cout <<"Did not match: Matched Page "<<i<<" and Ground Truth "<<groundTruths[i]<<endl;
			FP++;
		}
		result[i] = templates[results[i]];
	}

	// Metrics stuff
	double precision = ((double) TP) / ((double) (TP+FP));
	double recall = ((double) TP) / ((double) (TP+FN));
	double accuracy = ((double) (TP+TN)) / ((double) (TP+FP+TN+FN));
	double f1 = 2.0* ((precision*recall) / (precision + recall));
	double specificity = ((double) TN) / ((double) (FP+TN));

	cout<<"---------------------------------------"<<endl;
	cout <<"True Positives: " << TP <<endl;
	cout <<"False Positives: " << FP << endl;
	cout <<"True Negatives: " << TN <<endl;
	cout <<"False Negatives: " << FN << endl;
	cout<<"---------------------------------------"<<endl;
	cout<<"Precision: "<<precision<<endl;
	cout<<"Recall: "<<recall<<endl;
	cout<<"Accuracy: "<<accuracy<<endl;
	cout<<"Specificity: "<<specificity<<endl;
	cout<<"F1 Measure: "<<f1<<endl;
	cout<<"---------------------------------------"<<endl;

}
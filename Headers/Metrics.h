#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

// True Positive = a page is visible and recognised correctly
// False Positive = an incorrectly recognised page, where EITHER no page  was visible OR a different page was visible
// True Negative = no page visible and no page identified
// False Negative = a page is visible but no page was found
void compareAgainstGroundTruth( Mat * images, Mat * templates, int * results, int size, Mat * result);
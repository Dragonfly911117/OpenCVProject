#include <iostream>
#include <opencv2/core/mat.hpp>
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;

Mat sketchpad;
int R = 0, R_sliderValue = 0;
int G = 0, G_sliderValue = 0;
int B = 0, B_sliderValue = 0;
int counter = 0;
void drawing(Point a, Point b) {
    R = R_sliderValue;
    G = G_sliderValue;
    B = B_sliderValue;

    cout << counter++ << ' ';

    a.x += 175;
    b.x += 175;
    line(sketchpad, a, b, Scalar(B, G, R), 3);
    return;
}
#include "Drawing.cpp"
#include "array_judge.cpp"
#include "findCorsurCooedinate.cpp"
#include <algorithm>
#include <opencv2/highgui.hpp>
#include "paintingBoardInitializer.cpp"
using namespace cv;
using namespace std;

int main() {
    namedWindow("sketchpad", WINDOW_AUTOSIZE);
    sketchpad = imread("../paintingBoard.png");
    paintingBoardInitializer(true);
    Mat frame;
    VideoCapture cap;
    cap.open(0);
    if (!cap.isOpened()) {
        cout <<"Error! Camera is not opened";
        return -1;
    } //Confirm if the camera has turned on

    int num_arr[5]{0};
    int i= 0;
    Point ptFar[2];
    while (true) {
        paintingBoardInitializer();
        cap.read(frame); //read
        moveWindow("cam", 960, 0);
        resize(frame,frame,Size(960, 540)); //Reconfigure the size of the frame
        rectangle(frame, Point(150, 20), Point(800, 500), Scalar(255, 0, 255)); //Draw a square
        imshow("cam", frame);

        Mat crop_image;
        crop_image = frame(Rect(150, 20, 650, 450)); //900-400 (x,y)(w,h)
        Mat blurOutput;
        blur(crop_image, blurOutput, Size(3, 3)); //Filter out noise in the image
        Mat hsv;
        cvtColor(blurOutput, hsv, COLOR_BGR2HSV); //Convert to HSV(Hue, Saturation, Value)
        Mat mask;
        inRange(hsv, Scalar(2, 0, 0), Scalar(20, 255, 255), mask); //Use inRange to control H, S, V
        Mat dilation;
        Mat kernel = Mat::ones(5,5,CV_8U);
        Mat erosion;
        Mat filtered;
        Mat thresh;
        dilate(mask,dilation,kernel);
        erode(dilation, erosion, kernel);	//Remove unnecessary elements
        blur(erosion, filtered, Size(3, 3)); //Fill the gap with the appropriate structural elements
        threshold(filtered, thresh, 100, 255, 0);//threshold operation (Only black and white left)

        Mat contours_Image;
        vector<Vec4i> hierarchy;
        vector<vector<Point>> contours;
        findContours(thresh, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //findContours
        //hierarchy(Vec4i)the next contour,the previous contour,the sub-contour,the parent contour


        //Remove oversized contours
        int int_min = 100;
        int int_max = 800;
        vector<vector<Point>>::const_iterator itc = contours.begin();
        while (itc != contours.end())
        {
            if (itc->size() < int_min || itc->size() > int_max)
                itc = contours.erase(itc);
            else
                ++itc;
        }

        Rect bounding_rect;
        int largest_area = 0;
        //Find the biggest contour and draw a rectangle
        for (i = 0; i < contours.size(); i++) {
            double a = contourArea(contours[i], false); // Find the area of contour
            if (a > largest_area) {
                largest_area = a;
                bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
            }

        }

        if(bounding_rect.height<300&&bounding_rect.width<300)
            rectangle(crop_image, bounding_rect, Scalar(0), 2);

        RNG rng; //Random num
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        vector<vector<Point>>hull(contours.size());
        vector<vector<int>> hullsI(contours.size());
        vector<vector<Vec4i>> defects(contours.size());
        for (i = 0; i < contours.size(); i++) {
            convexHull(Mat(contours[i]), hull[i], false);	//Calculate the outermost points to form a convex polygon
            convexHull(Mat(contours[i]), hullsI[i], false);	//Calculate the outermost points to form a convex polygon(Use integer type)
            convexityDefects(Mat(contours[i]), hullsI[i], defects[i]);	//Defect detection

            drawContours(crop_image, contours, -1, Scalar(0, 255, 0), 2); //draw contours
            drawContours(crop_image, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point());//draw hull
        }


        //Calculate and draw the result of the convexityDefects
        //vector<vector<Point>> contours;
        int num = 1;
        for (size_t limit = 0; limit < contours.size(); limit++) {
            paintingBoardInitializer();
            //Ignore small contour
            size_t count = contours[limit].size();
            if (count < 300) {
                continue;
            }

            vector<Vec4i>::iterator d = defects[limit].begin();
            bool times = true;
            while (d != defects[limit].end()) {
                Vec4i v = *d;
                int start = v[0];
                Point ptStart(contours[limit][start]);
                int end = v[1];
                Point ptEnd(contours[limit][end]);
                int far = v[2];
                if (times) {
                    ptFar[0] = Point(contours[limit][start]);
                    times = false;
                }else{
                    ptFar[1] = Point(contours[limit][start]);
                    times = true;
                }
                int depth = v[3] / 256;

                if (depth > 75 && depth < 180) {
                    num++;
                    circle(crop_image, ptStart, 4, Scalar(255, 0, 0), 2);
                    circle(crop_image, ptEnd, 4, Scalar(200, 0, 255), 2);
                    circle(crop_image, Point(contours[limit][far]), 4, Scalar(0, 0, 255), 2);
                   // if ()
                }
                d++;
                num_arr[num]++;
            }
        }
        imshow("cam", frame);
        imshow("sketchpad", sketchpad);
        if (array_judge(num_arr) == 2)
            drawing(ptFar[0], ptFar[1]);
        //setMouseCallback("cam", reinterpret_cast<MouseCallback>(mouse_callback));
        if (waitKey(20) == 27)
            break;
    }
    destroyAllWindows();
    return 0;
}
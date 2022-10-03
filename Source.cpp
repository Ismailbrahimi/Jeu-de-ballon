
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include "Windows.h"
#include <opencv2/core/core.hpp>
#include <random>

using namespace std;
using namespace cv;

const String video_original = "video_original";
const String video_capture = "video_capture";

cv::Scalar bluelow = cv::Scalar(110, 50, 50);
cv::Scalar bluehigh = cv::Scalar(130, 255, 255);


void createCircle(Mat img, int x2, int y2);
bool MyCollision(Point pt1, Point pt2);

const int n = 5;
int xc = rand() % 600;
int yc = 500;

Point pt(xc, yc);

int result = 0;
int failed = 0;

void CoordFunc(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_MOUSEMOVE)
    {
        //cout << "Mouvement du curseur - coordonnees (" << x << ", " << y << ")" << endl;
    }
}

int main(int argc, char** argv)
{
    VideoCapture cap(0);
    namedWindow(video_original);
    namedWindow(video_capture);
    srand(time(NULL));

    Mat video, frame_HSV, video_detection;
    while (true) {
        cap >> video;
        if (video.empty())
        {
            break;
        }
        // BGR au HSV 
        cv::cvtColor(video, frame_HSV, COLOR_BGR2HSV);
        // Detection basee sur le HSV 
        inRange(frame_HSV, bluelow, bluehigh, video_detection);

        //set the callback function for any mouse event
        setMouseCallback("video_original", CoordFunc, NULL);

        vector<vector<Point>> contours;

        vector<Vec4i> hierarchy;

        findContours(video_detection, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

        // find moments of the image
        Moments m = moments(video_detection);
        Point p(m.m10 / m.m00, m.m01 / m.m00);

        // coordinates of centroid
        //cout << Mat(p) << endl;

        // show the image with a point mark at the centroid
        circle(video_detection, p, 5, Scalar(128, 0, 0), -1);

        SetCursorPos(p.x, p.y);
        int tentative = 10;
        //xc = 200;
        for (tentative = 10;tentative > 0;tentative--) {

            createCircle(video, xc, yc);

            if (MyCollision(p, Point(xc, yc)))
            {
                cout << "collision" << endl;
                result += 1;
                yc = 520;
                xc = rand() % 600;
            }

            line(video, cv::Point(10, 20), cv::Point(1000, 20), Scalar(255, 0, 0),
                3, LINE_8);
            line(video, cv::Point(10, 60), cv::Point(1000, 60), Scalar(255, 0, 0),
                3, LINE_8);
            cv::putText(video, //target image##
                "Score = " + to_string(result), cv::Point(10, 50), cv::FONT_HERSHEY_DUPLEX, 1.0, Scalar(255, 0, 0), 2);

            if (yc < 0) {
                failed = 1;
            }

        }
        if (failed == 1) {
            cv::putText(video, "FAILED", cv::Point(260, 250), cv::FONT_HERSHEY_DUPLEX, 1.0, Scalar(0, 0, 255), 4);
            cv::putText(video, "YOUR SCORE IS : " + to_string(result), cv::Point(160, 300), cv::FONT_HERSHEY_DUPLEX, 1.0, Scalar(0, 0, 255), 4);
        }

        drawContours(video_detection, contours, -1, Scalar(0, 255, 0), 2);
        imshow(video_original, video);
        imshow(video_capture, video_detection);

        yc -= 5;


        char key = (int)waitKey(1);
        if (key == 27)
        {
            break;
        }
    }
    return 0;

}

//methode pour creer un cercle
void createCircle(Mat img, int x2, int y2) {
    //RNG rng(12345);
    //Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
    circle(img, Point(x2, y2), 40, Scalar(0, 0, 255), -1);
}

//methode pour detecter une collision
bool MyCollision(Point pt1, Point pt2)
{
    double res = cv::norm(pt1 - pt2);//Euclidian distance
    if (res <= 40)
    {  //collis
        return true;
    }
    return false;
}
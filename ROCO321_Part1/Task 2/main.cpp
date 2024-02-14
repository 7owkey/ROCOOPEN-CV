//James Rogers Nov 2023 (c) Plymouth University

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../owl.h"

using namespace std;
using namespace cv;

int main() {
    robotOwl owl(1500, 1475, 1520, 1525, 1520); // Initialize with calibration values

    namedWindow("left");
    namedWindow("Color Target");

    // HSV range for tracking
    int HueLower = 96, SatLower = 199, ValLower = 70;
    int HueUpper = 114, SatUpper = 255, ValUpper = 206;

    // Create trackbars for HSV range adjustment
    createTrackbar("Hue Lower", "left", &HueLower, 179);
    createTrackbar("Sat Lower", "left", &SatLower, 255);
    createTrackbar("Val Lower", "left", &ValLower, 255);
    createTrackbar("Hue Upper", "left", &HueUpper, 179);
    createTrackbar("Sat Upper", "left", &SatUpper, 255);
    createTrackbar("Val Upper", "left", &ValUpper, 255);


    bool trackingEnabled = false;

    while (true) {
        Mat left, right;
        owl.getCameraFrames(left, right);


        Mat left_hsv;
        cvtColor(left, left_hsv, COLOR_BGR2HSV);

        Mat frame_filtered;
        inRange(left_hsv, Scalar(HueLower, SatLower, ValLower), Scalar(HueUpper, SatUpper, ValUpper), frame_filtered);

        Moments m = moments(frame_filtered, true);
        Point center(left.cols / 2, left.rows / 2);
        Point p;

 if (trackingEnabled) {
        if (m.m00 >= 0.2) //added to stop m.m00 = 0 issues
        {
            p = Point(m.m10/m.m00, m.m01/m.m00);
            circle(left, p, 5, Scalar(0, 0, 255), -1);

            int errorX = p.x - center.x;
            int errorY = p.y - center.y;

            // Calculate proportional adjustments for head and eyes together
            int adjustmentX = errorX * 0.125; // Adjust these multipliers as needed
            int adjustmentY = -errorY * 0.075;

            // Apply combined adjustments to both head and eyes
            owl.setServoRelativePositions(adjustmentX, adjustmentY, adjustmentX, adjustmentY, adjustmentX);
        }
}
        imshow("left", left);
        imshow("Color Target", frame_filtered);

        int key = waitKey(30); // Wait for a key press for 30 ms
               if (key == 27) // ESC key to exit
                   break;
               if (key == 's')
                   trackingEnabled = !trackingEnabled; // Toggle the tracking state
           }

    destroyAllWindows();
    return 0;
}


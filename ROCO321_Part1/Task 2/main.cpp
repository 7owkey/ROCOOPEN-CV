//James Rogers Nov 2023 (c) Plymouth University

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <iostream>
#include <string>

#include "../owl.h"

using namespace std;
using namespace cv;

int main()
{
    // Connect with the owl and load calibration values
    robotOwl owl(1500, 1475, 1520, 1525, 1520);

    // Create windows to display the camera frame and the color target
    namedWindow("left");
    namedWindow("Color Target");

    // Tracking start values - set to track green card
    int HueLower = 34;
    int SatLower = 42;
    int ValLower = 100;
    int HueUpper = 077; // Maximum value for Hue
    int SatUpper = 200; // Maximum value for Saturation
    int ValUpper = 185; // Maximum value for Value

    // Create trackbars to adjust the HSV range
    createTrackbar("Hue Lower", "left", &HueLower, 179);
    createTrackbar("Sat Lower", "left", &SatLower, 255);
    createTrackbar("Val Lower", "left", &ValLower, 255);
    createTrackbar("Hue Upper", "left", &HueUpper, 179);
    createTrackbar("Sat Upper", "left", &SatUpper, 255);
    createTrackbar("Val Upper", "left", &ValUpper, 255);

    while (true)
    {
        // Read the owl's camera frames
        Mat left, right;
        owl.getCameraFrames(left, right);

        // Convert the frame from BGR to HSV
        Mat left_hsv;
        cvtColor(left, left_hsv, COLOR_BGR2HSV);

        // Apply threshold to isolate pixels of color target
        Mat frame_filtered;
        // Threshold the HSV frame based on the trackbar values
        inRange(left_hsv, Scalar(HueLower, SatLower, ValLower), Scalar(HueUpper, SatUpper, ValUpper), frame_filtered);

        // Find the moments of the thresholded frame
        Moments m = moments(frame_filtered, true);

        // Calculate the center of mass
        Point p;
        if (m.m00 != 0)
        {
            p = Point(m.m10 / m.m00, m.m01 / m.m00);
            // Draw a marker on the original frame to show tracking
            circle(left, p, 5, Scalar(0, 0, 255), -1);
        }

        // Display the camera frame
        imshow("left", left);

        // Display the color target (highlighted pixels)
        imshow("Color Target", frame_filtered);

        // Check for key press to exit the loop
        if (waitKey(10) == 27) // ESC key
            break;
    }

    // Close all windows
    destroyAllWindows();

    return 0;
}


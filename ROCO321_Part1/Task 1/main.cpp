//James Rogers Oct 2023 (c) Plymouth University

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <iostream>
#include <string>

#include "../owl.h"

using namespace std;
using namespace cv;

void RGBtoHSV(const Mat& bgr, Mat& hsv) {
    cvtColor(bgr, hsv, COLOR_BGR2HSV);
}
string detectColor(float h, float s, float v) {
    // Define thresholds for saturation and value
    float min_saturation = 0.3;
    float min_value = 0.3;

    if (v < 0.3)
          return "black";
    if (s < 0.2 && v >0.8)
        return "white";
    if (((h >= 0 && h < 20) || h > 345) && s > min_saturation && v > min_value)
        return "red";
    if (h > 30 && h < 70 && s > min_saturation && v > min_value)
        return "yellow";
    if (h > 100 && h < 175 && s > min_saturation && v > min_value)
        return "green";
    if (h > 180 && h < 200 && s > min_saturation && v > min_value)
        return "cyan";
    if (h > 200 && h < 240 && s > min_saturation && v > min_value)
        return "blue";
    if (h > 280 && h < 345 && s > min_saturation && v > min_value)
        return "magenta";
    return "n/a";
}

Scalar getColourCode(const string& colour) {
    if (colour == "red")
        return Scalar(0, 0, 255); // BGR color value for red
    if (colour == "green")
        return Scalar(0, 255, 0); // BGR color value for green
    if (colour == "blue")
        return Scalar(255, 0, 0); // BGR color value for blue
    if (colour == "yellow")
        return Scalar(0, 255, 255); // BGR color value for yellow
    if (colour == "cyan")
        return Scalar(255, 255, 0); // BGR color value for cyan
    if (colour == "magenta")
        return Scalar(255, 0, 255); // BGR color value for magenta
    if (colour == "black")
        return Scalar(0, 0, 0); // BGR color value for black
    if (colour == "white")
        return Scalar(255, 255, 255); // BGR color value for white

    // Default color (white) for unknown colors
    return Scalar(255, 255, 255);
}
int main()
{
    //connect with the owl and load calibration values
    robotOwl owl(1500, 1475, 1520, 1525, 1520, true); //starts in "quiet mode" which switches off the servos.

    while (true){
        //read the owl's camera frames
        Mat left, right;
        owl.getCameraFrames(left, right);

        //get pixel colour values
        Point centrePoint(left.size().width/2, left.size().height/2);
        Vec3b pixelValue = left.at<Vec3b>(centrePoint);
        unsigned char r  = pixelValue[2];
        unsigned char g = pixelValue[1];
        unsigned char b = pixelValue[0];

        float h = pixelValue[0];
        float s = pixelValue[1] / 255.0;
        float v = pixelValue[2] / 255.0;


        // Convert RGB to HSV
               Mat left_hsv;
               RGBtoHSV(left, left_hsv);

        // Detect color
        string colour = detectColor(h, s, v);

        // Get color code
        Scalar colourCode = getColourCode(colour);

        //drawing functions
        circle(left, centrePoint, 15, colourCode, 2); //draw a circle to show the pixel being measured
        string text_rgb = "RGB: (" + to_string(r) + ", " + to_string(g) + ", " + to_string(b) + ")";      //create a string of the RGB values
        string text_hsv = "HSV: (" + to_string(h) + ", " + to_string(s) + ", " + to_string(v) + ")"; // create a string of the HSV values
        string text_colour = "" + colour; // create a string indicating the detected color
        putText(left, text_rgb, centrePoint + Point(-300, 200), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255,255,255), 2); //draw the RGB string to the image
        putText(left, text_hsv, centrePoint + Point(-300, 180), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255,255,255), 2); // draw the HSV string to the image
        putText(left, text_colour, centrePoint + Point(-150, 40), FONT_HERSHEY_SIMPLEX, 1, colourCode, 2); // draw the detected color with the corresponding color code

        //display image
        imshow("left",left);
        waitKey(30);
    }
}













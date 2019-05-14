#include <iostream>
#include <limits.h>
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;

Mat initial_frame, hsv_frame, out_frame;
VideoCapture cap;

int thresh_l_R = 0, thresh_l_G = 0, thresh_h_R = 255;
int thresh_l_B = 0, thresh_h_B = 255, thresh_h_G = 255;

void op_image(std::string s);
void op_video(std::string s);
void img_proc_path();
void img_proc_gate();
void img_proc_buoy();
void img_proc_bins();
void img_proc_heart();
void img_proc_coffin();
void show();

int main(int argc, char **argv) {
  int n = atoi(argv[1]);
  //---- thresholding window--------------
  namedWindow("Threshold", WINDOW_NORMAL);
  createTrackbar("Low B or H", "Threshold", &thresh_l_B, 255, NULL);
  createTrackbar("High B or H", "Threshold", &thresh_h_B, 255, NULL);
  createTrackbar("Low G or S", "Threshold", &thresh_l_G, 255, NULL);
  createTrackbar("High G or S", "Threshold", &thresh_h_G, 255, NULL);
  createTrackbar("Low R or V", "Threshold", &thresh_l_R, 255, NULL);
  createTrackbar("High R or V", "Threshold", &thresh_h_R, 255, NULL);
  //------end thresholding window-------------

  if (n) {
    std::cout << "opening image" << std::endl;
    op_image(argv[2]);
  }
  if (!n) {
    std::cout << "opening image" << std::endl;
    op_video(argv[2]);
  }
  return 0;
}

void op_image(std::string s) {
  s = "/home/kookies/RoboSub 2019/robosub "
      "tasks/Path/Bottom_screenshot_13.05.2019.png";
  std::cout << "Processing Image \n" << std::endl;
  initial_frame = imread(s, CV_LOAD_IMAGE_COLOR);

  if (!initial_frame.data) {
    std::cout << "Could not open or find the image" << std::endl;
    return;
  }
  //----- Image Processing ---------
  // img_proc_gate();
  img_proc_path();
  // img_proc_buoy();
  // img_proc_bins();
  // img_proc_heart();
  // img_proc_coffin();

  //-----End Image Processing ----------
  show();
  waitKey(0);
  return;
}

void op_video(std::string s) {
  std::cout << "Processing Video " << std::endl;
  cap.open(s);
  if (!cap.isOpened()) {
    return;
  }
  for (;;) {
    cap >> initial_frame;

    //----- Image Processing ---------
    // img_proc_gate();
    // img_proc_path();
    // img_proc_buoy();
    // img_proc_bins();
    // img_proc_heart();
    // img_proc_coffin();
    //-----End IMage Processing ----------
    show();
    if (waitKey(30) >= 0)
      break;
  }
}

void img_proc_path() {
  std::cout << "Processing path " << std::endl;
  cvtColor(initial_frame, hsv_frame, CV_BGR2HSV);

  inRange(hsv_frame, Scalar(thresh_l_B, thresh_l_G, thresh_l_R),
          Scalar(thresh_h_B, thresh_h_G, thresh_h_R), out_frame);
}
void img_proc_gate() { std::cout << "Processing gate " << std::endl; }
void img_proc_buoy() { std::cout << "Processing buoy " << std::endl; }
void img_proc_bins() { std::cout << "Processing bins " << std::endl; }
void img_proc_heart() { std::cout << "Processing heart " << std::endl; }
void img_proc_coffin() { std::cout << "Processing coffin " << std::endl; }

void show() {
  imshow("Input Frame", initial_frame);
  imshow("hsv image", hsv_frame);
  imshow("Output image", out_frame);
}

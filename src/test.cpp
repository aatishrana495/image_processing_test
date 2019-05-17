#include <iostream>
#include <limits.h>
#include <opencv2/opencv.hpp>
#include <string>

#define PI 3.14159265

using namespace cv;

Mat initial_frame, hsv_frame, gray_frame, canny_frame, out_frame;
VideoCapture cap;

int thresh_l_R = 0, thresh_l_G = 0, thresh_h_R = 255;
int thresh_l_B = 0, thresh_h_B = 255, thresh_h_G = 255;
int canny_low_thresh = 0, canny_ratio = 3, canny_kernel_size = 3;
int hl_thresh_detect = 50, hl_min_line_length = 50, hl_max_line_gap = 10;

float path_angle;

std::vector<Vec4i> lines;

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
  createTrackbar("canny low threshold", "Threshold", &canny_low_thresh, 255,
                 NULL);
  createTrackbar("canny ratio", "Threshold", &canny_ratio, 20, NULL);
  createTrackbar("kernel size", "Threshold", &canny_kernel_size, 20, NULL);

  createTrackbar("HL thresh detect", "Threshold", &hl_thresh_detect, 255, NULL);
  createTrackbar("HL min Line Length ", "Threshold", &hl_min_line_length, 255,
                 NULL);
  createTrackbar("HL max line Gap", "Threshold", &hl_max_line_gap, 255, NULL);
  //------end thresholding window-------------

  if (n) {

    op_image(argv[2]);
  }
  if (!n) {
    op_video(argv[2]);
  }
  return 0;
}

void op_image(std::string s) {
  s = "/home/kookies/RoboSub 2019/robosub "
      "tasks/Path/Bottom_screenshot_13.05.2019.png";

  for (;;) {
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
    if (waitKey(30) >= 0)
      break;
  }
}

void op_video(std::string s) {

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

  cvtColor(initial_frame, hsv_frame, CV_BGR2HSV);
  inRange(hsv_frame, Scalar(thresh_l_B, thresh_l_G, thresh_l_R),
          Scalar(thresh_h_B, thresh_h_G, thresh_h_R), gray_frame);
  Canny(gray_frame, canny_frame, canny_low_thresh,
        canny_low_thresh * canny_ratio, canny_kernel_size);
  HoughLinesP(canny_frame, lines, 1, CV_PI / 180, hl_thresh_detect,
              hl_min_line_length, hl_max_line_gap);
  std::cout << lines.size() << std::endl;
  float slope[lines.size()], a, b;
  for (size_t i = 0; i < lines.size(); i++) {
    Vec4i l = lines[i];
    line(initial_frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 255, 0),
         3, CV_AA);
    if (l[4] != l[2]) {
      slope[i] = (l[3] - l[1]) / (l[4] - l[2]);
      if (i == 1) {
        a = slope[i];
      } else {
        if (slope[i] > a + 5 || slope[i] < a - 5) {
          b = slope[i];
        }
      }
    }
    if (a * b != -1) {
      path_angle = atan(abs((b - a) / (1 + a * b))) * 180 / PI;
    } else {
      path_angle = 90;
    }
    std::cout << "path angle is " << path_angle << std::endl;
  }
}
void img_proc_gate() { std::cout << "VAibhav " << std::endl; }
void img_proc_buoy() { std::cout << "Processing buoy " << std::endl; }
void img_proc_bins() { std::cout << "Processing bins " << std::endl; }
void img_proc_heart() { std::cout << "Processing heart " << std::endl; }
void img_proc_coffin() { std::cout << "Processing coffin " << std::endl; }

void show() {
  imshow("Input Frame", initial_frame);
  imshow("HSV image", hsv_frame);
  imshow("Gray image", gray_frame);
  imshow("Output image", canny_frame);
}

#include <iostream>
#include <limits.h>
#include <opencv2/opencv.hpp>
#include <string>

#define PI 3.14159265

using namespace cv;

Mat initial_frame, hsv_frame, gauss_frame, erosion_frame, dilation_frame,
    gray_frame, gray_frame_1, canny_frame, out_frame;
Mat element;
VideoCapture cap;

int thresh_l_R = 0, thresh_l_G = 112, thresh_h_R = 255;
int thresh_l_B = 0, thresh_h_B = 51, thresh_h_G = 255;
int canny_low_thresh = 0, canny_ratio = 3, canny_kernel_size = 3;
int hl_thresh_detect = 50, hl_min_line_length = 50, hl_max_line_gap = 10;

int x, y, k;

float path_angle;

std::vector<Vec4i> lines;

void op_image(std::string s, int obj);
void op_video(std::string s, int obj);
void img_proc_path();
void img_proc_gate();
void img_proc_buoy();
void img_proc_bins();
void img_proc_heart();
void img_proc_coffin();
void show();

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cout << "Usage: \n ./test a path b \n a is 1 for an image and 0 for a "
                 "video \n b is \n 1 for gate\n 2 for path\n 3 for buoy\n 4 "
                 "for bins\n 5 for heart\n 6 for coffins\n"
              << '\n';
  }
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

    op_image(argv[2], atoi(argv[3]));
  }
  if (!n) {
    op_video(argv[2], atoi(argv[3]));
  }
  return 0;
}

void op_image(std::string s, int obj) {

  for (;;) {
    initial_frame = imread(s, CV_LOAD_IMAGE_COLOR);

    if (!initial_frame.data) {
      std::cout << "Could not open or find the image" << std::endl;
      return;
    }
    //----- Image Processing ---------
    switch (obj) {
    case '1':
      img_proc_gate();
      break;
    case '2':
      img_proc_path();
    case '3':
      img_proc_buoy();
      break;
    case '4':
      img_proc_bins();
      break;
    case '5':
      img_proc_heart();
      break;
    case '6':
      img_proc_coffin();
      break;
    }

    //-----End Image Processing ----------
    show();
    if (waitKey(30) >= 0)
      break;
  }
}

void op_video(std::string s, int obj) {

  cap.open(s);
  if (!cap.isOpened()) {
    std::cout << "Could not open or find the image" << std::endl;
    return;
  }
  for (;;) {
    cap >> initial_frame;

    //----- Image Processing ---------
    switch (obj) {
    case 1:
      img_proc_gate();
      break;
    case 2:
      img_proc_path();
      break;
    case 3:
      img_proc_buoy();
      break;
    case 4:
      img_proc_bins();
      break;
    case 5:
      img_proc_heart();
      break;
    case 6:
      img_proc_coffin();
      break;
    }
    //-----End IMage Processing ----------
    show();
    if (waitKey(500) >= 0)
      break;
  }
}

void img_proc_path() {

  element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(0, 0));
  cvtColor(initial_frame, hsv_frame, CV_BGR2HSV);
  erode(hsv_frame, erosion_frame, element);
  dilate(erosion_frame, dilation_frame, element);
  GaussianBlur(dilation_frame, gauss_frame, Size(3, 3), 0, 0);
  inRange(gauss_frame, Scalar(thresh_l_B, thresh_l_G, thresh_l_R),
          Scalar(thresh_h_B, thresh_h_G, thresh_h_R), gray_frame);
  /*
    for (int i1 = 0; i1 < gray_frame.rows; i1++) {
      x = y = k = 0;
      for (int j1 = 0; j1 < gray_frame.cols; j1++) {

        if (gray_frame.at<uchar>(i1, j1) > 0) {
          y += i1;
          x += j1;
          k++;
        }
      }
      double limit = 0.01 * gray_frame.cols * gray_frame.rows;

      if (k >= limit) {
        x /= k;
        y /= k;
      }
      circle(initial_frame, Point(x, y), 3, Scalar(255, 0, 0), 1, 8);
    }*/

  Canny(gray_frame, canny_frame, canny_low_thresh,
        canny_low_thresh * canny_ratio, canny_kernel_size);

  lines.clear();
  HoughLinesP(canny_frame, lines, 1, CV_PI / 180, hl_thresh_detect,
              hl_min_line_length, hl_max_line_gap);
}

void img_proc_gate() { std::cout << "Processing gate " << std::endl; }
void img_proc_buoy() { std::cout << "Processing buoy " << std::endl; }
void img_proc_bins() { std::cout << "Processing bins " << std::endl; }
void img_proc_heart() { std::cout << "Processing heart " << std::endl; }
void img_proc_coffin() { std::cout << "Processing coffin " << std::endl; }

void show() {
  imshow("Input Frame", initial_frame);
  imshow("HSV image", hsv_frame);
  imshow("Gray image", gray_frame);
  // imshow("Output image", canny_frame);
}

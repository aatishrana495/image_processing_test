#include <iostream>
#include <limits.h>
#include <opencv2/opencv.hpp>

using namespace cv;
void op_image(std::string s);
void op_video(std::string s);
void img_proc_path();
void img_proc_gate();
void img_proc_buoy();
void img_proc_bins();
void img_proc_heart();
void img_proc_coffin();

Mat frame;

int main(int argc, char **argv) {
  if (argv[1] == "0") {
    op_image(argv[2]);
  } else if (argv[1] == "1") {
    op_video(argv[2]);
  } else {
    std::cout << "usage:" << std::endl;
  }
  return 0;
}

void op_image(std::string s) {
  std::cout << "Processing Image \n" << std::endl;
  frame = imread(s, CV_LOAD_IMAGE_COLOR);

  if (!frame.data) {
    std::cout << "Could not open or find the image" << std::endl;
    return;
  }
  //----- Image Processing ---------
  img_proc_gate();
  // img_proc_path();
  // img_proc_buoy();
  // img_proc_bins();
  // img_proc_heart();
  // img_proc_coffin();

  //-----End IMage Processing ----------

  imshow("Display window", frame);
  waitKey(0);
  return;
}

void op_video(std::string s) {
  std::cout << "Processing Video " << std::endl;
  VideoCapture cap(0);
  if (!cap.isOpened()) {
    return;
  }
  for (;;) {
    cap >> frame;

    //----- Image Processing ---------
    img_proc_gate();
    // img_proc_path();
    // img_proc_buoy();
    // img_proc_bins();
    // img_proc_heart();
    // img_proc_coffin();
    //-----End IMage Processing ----------

    if (waitKey(30) >= 0)
      break;
  }
}

void img_proc_path() { std::cout << "Processing path " << std::endl; }
void img_proc_gate() { std::cout << "Processing gate " << std::endl; }
void img_proc_buoy() { std::cout << "Processing buoy " << std::endl; }
void img_proc_bins() { std::cout << "Processing bins " << std::endl; }
void img_proc_heart() { std::cout << "Processing heart " << std::endl; }
void img_proc_coffin() { std::cout << "Processing coffin " << std::endl; }

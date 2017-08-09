#include <cassert>
#include <iostream>

#include <cube/search.hh>
#include <detect/detector.hh>


// temp
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "detect/displayer.hh"


using namespace std;
using namespace cv;

Mat src, src_gray;
detect::Displayer displayer("solvzer");
RNG rng(12345);

int alpha = 100;
int beta = 50;
int threshb = 150;

int main(int argc, char** argv)
{
  namedWindow("debug", CV_WINDOW_AUTOSIZE);
  createTrackbar("contrast", "debug", &alpha, 300, nullptr);
  createTrackbar("brightness", "debug", &beta, 100, nullptr);
  createTrackbar("threshold", "debug", &threshb, 255, nullptr);
  VideoCapture cap(1);
  if (!cap.isOpened())
    return -1;

  for (;;)
  {
    Mat src, use;
    cap >> src;
    displayer.addImage(src, "source", -1);
    use = Mat::zeros(src.size(), src.type());
    circle(use, use.size() / 2, 60, Scalar(255, 255, 255), -1, 8, 0);
    src.copyTo(use, use);
    displayer.addImage(use, "target", -1);
    cvtColor(use, src_gray, CV_BGR2GRAY);
    //displayer.addImage(src_gray, "gray", CV_GRAY2BGR);

    cvtColor(src_gray, src_gray, CV_GRAY2BGR);
    Mat contrast = Mat::zeros(src_gray.size(), src_gray.type());
    for (int y = 0; y < src_gray.rows; y++)
      for (int x = 0; x < src_gray.cols; x++)
        for (int c = 0; c < 3; c++)
          contrast.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(((double)alpha / 100.)
              * (src_gray.at<Vec3b>(y, x)[c]) + beta);

    displayer.addImage(contrast, "contrast", -1);

    Mat hsv;
    cvtColor(contrast, hsv, CV_BGR2HSV);
    inRange(hsv, Scalar(0, 0, 0, 0), Scalar(180, 255, threshb, 0), hsv);
    displayer.addImage(hsv, "hsv", CV_GRAY2BGR);
    Mat dilated;
    dilate(hsv, dilated, getStructuringElement(MORPH_ELLIPSE, Size(11, 11), Point(5, 5)));
    displayer.addImage(dilated, "dilated", CV_GRAY2BGR);

    use = Mat::zeros(dilated.size(), dilated.type());
    circle(use, use.size() / 2, 60, Scalar(255, 255, 255), -1, 8, 0);
    bitwise_and(dilated, use, dilated);

    int x = 0, y = 0;
    int total = 0;

    for (int j = 0; j < dilated.rows; j++)
      for (int i = 0; i < dilated.cols; i++)
      {
        if (dilated.at<char>(j, i) != 0)
        {
          x += i;
          y += j;
          total++;
        }
      }
    if (total != 0)
    {
      x /= total;
      y /= total;
    }

    cvtColor(dilated, dilated, CV_GRAY2BGR);

    circle(dilated, Point(x, y), 4, Scalar(0, 0, 255), -1, 8, 0);
    displayer.addImage(dilated, "center", -1);

    displayer.display();
  }
  waitKey(0);
  return 0;
  //cube::Search search;

  /*auto face = cube::Face(init_state);

  std::cout << face << std::endl;

  auto solution = search.solution(init_state, DEPTH);

  if (!search.ack_solution(init_state, solution))
    std::cout << "Solution is wrong" << std::endl; */


  //auto face = cube::Face(cube::Cube::solved_state_);
  //std::cout << face << std::endl;

  //std::string moves =
  //  "L'D2R1F1R1F2L'F'D'F2L'F'R2B'D'F1D'R1D'F'R2B'F2B2U2L1D1B2U'L'";
  //face.scramble();

  //auto sol = search.solution(face, cube::Search::DEPTH);
  //assert(search.ack_solution(face, sol));

  if (argc != 2)
  {
    std::cout << "Usage: ./solvzer path_to_cube" << std::endl;
    return 1;
  }
  detect::Detector d(displayer, detect::CameraPosition::BOTTOM);
  while (true)
  {
    d.update();
  }

  return 0;
}

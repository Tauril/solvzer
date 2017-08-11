#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <unistd.h> // for sleep
#include <iostream>
#include <vector>

#include <cube/color.hh>
#include <detect/displayer.hh>

#define DEBUG_DETECT
#define DEBUG_THICKNESS 5 // size of debug visuals

namespace detect
{
  enum class Direction
  {
    TOP,
    BOTTOM,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
  };

  enum class CameraPosition
  {
    TOP,
    BOTTOM
  };

  class Detector
  {
    public:
      Detector(Displayer& displayer, const CameraPosition position);
      ~Detector() = default;

      void update();

    private:
      void computeCenter();
      void startDetection();
      void computeColors();
      bool isInRangeMask(const cv::Scalar& low, const cv::Scalar& high,
                         const cv::Point2f& coord);
      cv::Point computeExtremity(const detect::Direction dir,
                                 const cv::Mat& mat);
      void fillDirection(const Direction dir, const cv::Point current,
                         cv::Point& res, const cv::Mat& bMask, cv::Mat& tmp);
      void updateExtreme(const Direction dir, const cv::Point& current,
                         cv::Point& res);
      cv::Point2f intersection(const cv::Point2f& o1, const cv::Point2f& o2,
                               const cv::Point2f& p1, const cv::Point2f& p2);

      Displayer& displayer_;
      cv::VideoCapture capture_;
      cv::Mat image_;
      cv::Mat image_debug_;
      cv::Point center_;
      std::vector<cv::Point2f> facelets_;
      std::vector<cube::color> colors_;
      CameraPosition cameraPosition_;
  };
}

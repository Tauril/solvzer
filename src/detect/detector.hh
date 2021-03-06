#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <unistd.h> // for sleep
#include <iostream>
#include <vector>
#include <unordered_map>

#include <cube/color.hh>
#include <detect/displayer.hh>

//#define DEBUG_DETECT
#define DEBUG_THICKNESS 5 // size of debug visuals

#ifdef _PI_
# define START_CHANNEL 0
# undef DEBUG_DETECT
#else
# define START_CHANNEL 1
#endif

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
      Detector(Displayer& displayer, const CameraPosition position, size_t channel);
      ~Detector() = default;

      std::vector<cube::color>& getColors();

      void update();

    private:
      void computeCenter();
      void startDetection();
      void computeColors();
      cube::color classifyColor(cv::Vec3b color);
      bool isInRangeMask(const cv::Scalar& low, const cv::Scalar& high,
                         const cv::Point2f& coord);
      void detectExtremities(cv::Point& p1, cv::Point& p2, cv::Point& p3);
      void setFixedExtremities(cv::Point& p1, cv::Point& p2, cv::Point& p3);
      cv::Point computeExtremity(const detect::Direction dir,
                                 const cv::Mat& mat);
      void fillDirection(const Direction dir, const cv::Point current,
                         cv::Point& res, const cv::Mat& bMask, cv::Mat& tmp);
      void updateExtreme(const Direction dir, const cv::Point& current,
                         cv::Point& res);
      cv::Point2f intersection(const cv::Point2f& o1, const cv::Point2f& o2,
                               const cv::Point2f& p1, const cv::Point2f& p2);

      int channel_;
      Displayer& displayer_;
      cv::VideoCapture capture_;
      cv::Mat image_;
      cv::Mat image_debug_;
      cv::Point center_;
      std::vector<cv::Point2f> facelets_;
      std::vector<cube::color> colors_;
      CameraPosition cameraPosition_;
      int areaRad_ = 50; // radius of interest circle in detector
  };
}

// U BLANC
// R BLEU
// F ROUGE
// D JAUNE
// L VERT
// B ORANGE

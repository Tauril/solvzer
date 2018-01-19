#pragma once

#include <string>
#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace detect
{
  class Displayer
  {
    public:
      Displayer(std::string name);

      void destroy();
      void addImage(cv::Mat img, const std::string& imgName, int convert);
      void display();

    private:
      std::string name_;
      int rows_, cols_;
      int width_, height_;
      std::vector<cv::Mat> images_;
      std::vector<std::string> names_;
  };
}

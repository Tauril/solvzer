#include <detect/displayer.hh>

namespace detect
{
  Displayer::Displayer(std::string name) : name_(name), rows_(0), cols_(0)
  {
    cv::namedWindow(name, CV_WINDOW_AUTOSIZE);
  }

  void Displayer::addImage(cv::Mat img, const std::string& imgName, int convert)
  {
    cv::Mat copy;
    copy = img.clone();
    if (convert != -1)
      cv::cvtColor(copy, copy, convert);
    images_.push_back(copy);
    names_.push_back(imgName);
  }

  void Displayer::display()
  {
    if (images_.empty())
      return;
    int cols = images_.size() < 3 ? images_.size() : (images_.size() < 5 ? 2 : 3);
    int max_width = 0;
    int max_height = 0;
    for (size_t i = 0; i < images_.size(); i++)
    {
      if (i > 0 && images_[i].type() != images_[i - 1].type())
      {
        std::cerr << "Warning: cannot display full image, images types are not"
          << "the same (" << images_[i].type() << "|" << images_[i - 1].type()
          << ")\n";
        return;
      }
      max_height = std::max(max_height, images_[i].rows);
      max_width = std::max(max_width, images_[i].cols);
    }

    int rows = std::ceil((float)images_.size() / (float)cols);
    cv::Mat result = cv::Mat::zeros(rows * max_height, cols * max_width, images_[0].type());
    size_t i = 0;
    int current_height = 0;
    int current_width = 0;
    for (int y = 0; y < rows; y++)
    {
      for (int x = 0; x < cols; x++)
      {
        if (i >= images_.size())
          break;

        cv::Mat to(result, cv::Range(current_height, current_height + images_[i].rows),
            cv::Range(current_width, current_width + images_[i].cols));
        images_[i++].copyTo(to);
        cv::putText(result, names_[i - 1], cv::Point(current_width + 10, current_height + 20),
            CV_FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 255, 0), 1, CV_AA);
        current_width += max_width;
      }

      current_width = 0;
      current_height += max_height;
    }

    cv::imshow(name_, result);
    cv::waitKey(1);
    images_.clear();
    names_.clear();
  }
}

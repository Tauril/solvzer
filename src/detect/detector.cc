#include <detect/detector.hh>

#define GREEN   cv::Scalar(0, 255, 0)
#define PINK    cv::Scalar(155, 0, 255)
#define BLUE    cv::Scalar(255, 0, 0)
#define BLACK   cv::Scalar(0, 0, 0)

namespace detect
{
  Detector::Detector(Displayer& displayer, const CameraPosition position)
    : displayer_(displayer), cameraPosition_(position)
  {
    // We try to load the image
    //image_ = cv::imread(path_to_file, CV_LOAD_IMAGE_COLOR);
    capture_ = cv::VideoCapture(1);

    // If we failed
    if (!capture_.isOpened())
    {
      std::cerr << "Could not open capture, aborting." << std::endl;
      std::abort();
    }

    // We blur the image to smooth it
    cv::blur(image_, image_, cv::Size(5, 5));

    cv::namedWindow("detect main", cv::WINDOW_NORMAL);
    cv::imshow("detect main", image_);
    cv::setMouseCallback("detect main", detect::setCenter, this);

#ifdef DEBUG_DETECT
    // image_debug_ is used to display debug info on the detection
    image_debug_ = image_.clone();

    // If the debug mode is set, we open a window to display the visual debug
    cv::namedWindow("detect debug", cv::WINDOW_AUTOSIZE);
#endif

    while (1)
    {
      cv::waitKey(0);
    }
  }

  // Sets the center of the rubik's cube (the closest edge to the camera) and
  // starts the detecting process
  void Detector::setCenter(const int x, const int y)
  {
    center_ = cv::Point(x, y);

#ifdef DEBUG_DETECT
    cv::circle(image_debug_, center_, DEBUG_THICKNESS, GREEN, -1);
    update_debug();
#endif

    startDetection();
  }

  void Detector::update()
  {
  }

  void Detector::startDetection()
  {
    // We compute the black mask used to detect extremities
    cv::Mat bMask;
    cv::Scalar lowerb = cv::Scalar(0, 0, 0);
    cv::Scalar upperb = cv::Scalar(50, 50, 50);
    cv::inRange(image_, lowerb, upperb, bMask);
    int erosion_size = 2;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS,
                                                cv::Size(2 * erosion_size + 1,
                                                         2 * erosion_size + 1),
                                                cv::Point(erosion_size,
                                                          erosion_size));
    cv::GaussianBlur(bMask, bMask, cv::Size(3, 3), 0);
    cv::dilate(bMask, bMask, element);

#ifdef DEBUG_DETECT
    cv::imshow("detect debug", bMask);
    cv::waitKey(0);
#endif

    cv::Point p1, p2, p3; // The extremities of the rubik's cube

    // If the camera is above the rubik's cube, call the appropriate functions
    if (cameraPosition_ == CameraPosition::TOP)
    {
      // TOP CAMERA
      p1 = computeExtremity(Direction::TOP_LEFT, bMask);
      p2 = computeExtremity(Direction::TOP_RIGHT, bMask);
      p3 = computeExtremity(Direction::BOTTOM, bMask);
    }
    else
    {
      // BOTTOM CAMERA
      p1 = computeExtremity(Direction::TOP, bMask);
      p2 = computeExtremity(Direction::BOTTOM_RIGHT, bMask);
      p3 = computeExtremity(Direction::BOTTOM_LEFT, bMask);
    }

    // m1, m2 and m3 are the middle points between the extremities
    cv::Point m1 = cv::Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
    cv::Point m2 = cv::Point((p2.x + p3.x) / 2, (p2.y + p3.y) / 2);
    cv::Point m3 = cv::Point((p3.x + p1.x) / 2, (p3.y + p1.y) / 2);

#ifdef DEBUG_DETECT
    cv::circle(image_debug_, m1, DEBUG_THICKNESS, BLUE, -1);
    cv::circle(image_debug_, m2, DEBUG_THICKNESS, BLUE, -1);
    cv::circle(image_debug_, m3, DEBUG_THICKNESS, BLUE, -1);
    update_debug();
#endif

    // We add the extremities in a vector to iterate easily
    std::vector<cv::Point> edges;
    edges.push_back(p1);
    edges.push_back(p2);
    edges.push_back(p3);

    // For each edge in a clockwise order starting from the most top left one,
    // we compute the lines of the facelets, and on each line we iterate on the
    // 'perpendicular' lane, computing each time the intersection of the two
    // lines, thus giving us the approximate center of the facelets near the
    // closest edge
    for (size_t i = 0; i < edges.size(); i++)
    {
      // step and step2 are the steps needed to advance 1/2 of a facelet in a
      // given direction
      cv::Point step = (edges[i] - center_) / 6;
      cv::Point step2 = (edges[(i + 1) % edges.size()] - center_) / 6;

      // stepH and stepH2 are the points tracing the line crossing the centers
      // of all the facelets of one column in the rubik's cube. We use this
      // lines to compute intersections later, same for stepV & stepV2
      cv::Point stepH(center_ + step);
      cv::Point stepH2(edges[(i + 1) % edges.size()] + step);
      cv::Point stepV(center_ + step2);
      cv::Point stepV2(edges[i] + step2);

#ifdef DEBUG_DETECT
      cv::line(image_debug_, stepH, stepH2, PINK, DEBUG_THICKNESS / 2);
      cv::line(image_debug_, stepV, stepV2, PINK, DEBUG_THICKNESS / 2);
      update_debug();
#endif

      // There are 3 facelets per column, so we iterate on the middle of each
      // facelet and compute the intersecting point with the line of the column
      // computed before, thus giving us the approximate center of each facelet
      // t1 will always be before t2 in a clockwise order
      for (int l = 0; l < 3; l++)
      {
        cv::Point2f t1 = intersection(stepV, stepV2,
                                      center_ + (step * (l * 2 + 1)),
                                      edges[(i + 1) % edges.size()]
                                        + (step * (l * 2 + 1)));
        cv::Point2f t2 = intersection(stepH, stepH2,
                                      center_ + (step2 * (l * 2 + 1)),
                                      edges[i] + (step2 * (l * 2 + 1)));

        // for the first iteration, t1 ~= t2 and we don't want duplicates
        if (l != 0)
          facelets_.push_back(t1);
        facelets_.push_back(t2);

#ifdef DEBUG_DETECT
        std::cout << "intersection " << l << ": t1 = " << t1
                  << ", t2 = " << t2 << std::endl;
        cv::circle(image_debug_, t1, DEBUG_THICKNESS, PINK, -1);
        cv::circle(image_debug_, t2, DEBUG_THICKNESS, BLACK, -1);
        update_debug();
#endif
      }
    }

    computeColors();
  }

  void Detector::computeColors()
  {
#ifdef DEBUG_DETECT
    // We draw circles around the faces spots
    image_debug_ = image_.clone();
    for (size_t i = 0; i < facelets_.size(); i++)
    {
      cv::putText(image_debug_, std::to_string(i),
                  facelets_[i] - cv::Point2f(6, 10),
                  cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, BLACK, 1, CV_AA);
      cv::circle(image_debug_, facelets_[i], DEBUG_THICKNESS, BLACK, 2);
      cv::imshow("detect debug", image_debug_);
      cv::waitKey(1);
    }

    // Histogram equalization
    std::vector<cv::Mat> channels;
    cv::Mat img_hist_equalized;

    cv::cvtColor(image_debug_, img_hist_equalized, CV_BGR2YCrCb);
    cv::split(img_hist_equalized, channels);
    cv::equalizeHist(channels[0], channels[0]);
    cv::merge(channels, img_hist_equalized);
    cv::cvtColor(img_hist_equalized, img_hist_equalized, CV_YCrCb2BGR);
    cv::imshow("detect debug", img_hist_equalized);
    cv::waitKey(0);
#endif

    // We convert our image to HSV
    cv::cvtColor(image_, image_, CV_BGR2HSV);
    for (size_t i = 0; i < facelets_.size(); i++)
    {
      std::cout << "[" << i << "] " << facelets_[i] << " = "
                << image_.at<cv::Vec3b>(facelets_[i]);

      if (isInRangeMask(cv::Scalar(160, 220, 25), cv::Scalar(180, 255, 255), facelets_[i]))
        std::cout << " RED" << std::endl;
      else if (isInRangeMask(cv::Scalar(0, 217, 50), cv::Scalar(3, 255, 255), facelets_[i]))
        std::cout << " RED" << std::endl;
      else if (isInRangeMask(cv::Scalar(0, 50, 30), cv::Scalar(18, 255, 255), facelets_[i]))
        std::cout << " ORANGE" << std::endl;
      else if (isInRangeMask(cv::Scalar(177, 50, 30), cv::Scalar(180, 255, 255), facelets_[i]))
        std::cout << " ORANGE" << std::endl;
      else if (isInRangeMask(cv::Scalar(19, 50, 30), cv::Scalar(33, 255, 255), facelets_[i]))
        std::cout << " YELLOW" << std::endl;
      else if (isInRangeMask(cv::Scalar(34, 50, 30), cv::Scalar(87, 255, 255), facelets_[i]))
        std::cout << " GREEN" << std::endl;
      else if (isInRangeMask(cv::Scalar(88, 50, 30), cv::Scalar(130, 255, 255), facelets_[i]))
        std::cout << " BLUE" << std::endl;
      else if (isInRangeMask(cv::Scalar(0, 0, 127), cv::Scalar(255, 100, 255), facelets_[i]))
        std::cout << " WHITE" << std::endl;
      else
        std::cout << " UNKNOWN" << std::endl;
    }

    while (1)
    {
      cv::waitKey(0);
    }
  }

  // Computes the mask with the given lower bound and upper bound. Returns true
  // if the pixel at coord is positive within the mask, false otherwise
  bool Detector::isInRangeMask(const cv::Scalar& low, const cv::Scalar& high,
                               const cv::Point2f& coord)
  {
    cv::inRange(image_, low, high, image_debug_);
    if (image_debug_.at<unsigned char>(coord) != 0)
      return true;

    return false;
  }

  // Clones the original image and returns the call to fillDirection on it
  cv::Point Detector::computeExtremity(const Direction dir,
                                       const cv::Mat& bMask)
  {
    cv::Point edge = center_;

    cv::Mat img_cpy = image_.clone();
    fillDirection(dir, center_, edge, bMask, img_cpy);

#ifdef DEBUG_DETECT
    cv::imshow("detect debug", img_cpy);
    cv::waitKey(0);
    cv::circle(image_debug_, edge, DEBUG_THICKNESS, GREEN, -1);
    update_debug();
#endif

    return edge;
  }

  // FIXME: Cleanup this function
  // Find the most extreme point in a given direction
  // This function takes a mask as parameters which is a black mask applied on
  // the rubik's cube. tmp is the clone of the original image
  void Detector::fillDirection(const Direction dir, const cv::Point current,
                               cv::Point& res, const cv::Mat& bMask,
                               cv::Mat& tmp)
  {
    // We update the extreme depending on the direction
    updateExtreme(dir, current, res);

    // If we already came here, no need to stack overflow
    if (tmp.at<cv::Vec3b>(current) == cv::Vec3b(0, 0, 255))
      return;

    // We set the pixel at our location to red (arbitrary)
    tmp.at<cv::Vec3b>(current) = cv::Vec3b(0, 0, 255);

    // The three next points to look at
    cv::Point p1, p2, p3;

    // TOP_RIGHT
    p1 = cv::Point(0, -1);
    p2 = cv::Point(1, 0);
    p3 = cv::Point(1, -1);

    // FIXME: Ugly solution, to be rewritten
    if (dir == Direction::BOTTOM_LEFT || dir == Direction::BOTTOM)
    {
      p1 *= -1;
      p2 *= -1;
      p3 *= -1;
    }
    if (dir == Direction::TOP_LEFT)
    {
      p2.x *= -1;
      p3.x *= -1;
    }
    if (dir == Direction::BOTTOM_RIGHT)
    {
      p1.y *= -1;
      p3.y *= -1;
    }
    if (dir == Direction::TOP)
    {
      p2 = cv::Point(-1, -1);
    }
    if (dir == Direction::BOTTOM)
    {
      p2 = cv::Point(1, 1);
    }

    // If the mask at new location is positive (black on the original image),
    // we recurse on this new location
    if (bMask.at<unsigned char>(current + p1) != 0)
      fillDirection(dir, current + p1, res, bMask, tmp);
    if (bMask.at<unsigned char>(current + p2) != 0)
      fillDirection(dir, current + p2, res, bMask, tmp);
    if (bMask.at<unsigned char>(current + p3) != 0)
      fillDirection(dir, current + p3, res, bMask, tmp);
  }

  // Updates res if current if closer to the chosen direction
  void Detector::updateExtreme(const Direction dir, const cv::Point& current,
                               cv::Point& res)
  {
    if (dir == Direction::TOP && current.y < res.y)
      res = current;
    if (dir == Direction::TOP_RIGHT
        && image_.cols - current.x + current.y < image_.cols - res.x + res.y)
      res = current;
    if (dir == Direction::TOP_LEFT
        && current.x + current.y < res.x + res.y)
      res = current;
    if (dir == Direction::BOTTOM
        && current.y > res.y)
      res = current;
    if (dir == Direction::BOTTOM_LEFT
        && image_.rows - current.y + current.x < image_.rows - res.y + res.x)
      res = current;
    if (dir == Direction::BOTTOM_RIGHT
        && (image_.rows - current.y) + (image_.cols - current.x)
          < (image_.rows - res.y) + (image_.cols - res.x))
      res = current;
  }

  // Computes the intersections between two lines A and B represented
  // respectively by the points a1,a2 and b1,b2
  cv::Point2f Detector::intersection(const cv::Point2f& a1,
                                     const cv::Point2f& a2,
                                     const cv::Point2f& b1,
                                     const cv::Point2f& b2)
  {
    // m and c are the constants such as the line equation is m * x + c
    cv::Point2f res(0, 0);
    float m1, c1, m2, c2;
    float dx, dy;

    // Computations for line A
    dx = a2.x - a1.x;
    dy = a2.y - a1.y;
    m1 = dy / dx;
    c1 = a1.y - m1 * a1.x;

    // Computations for line B
    dx = b2.x - b1.x;
    dy = b2.y - b1.y;
    m2 = dy / dx;
    c2 = b2.y - m2 * b2.x;

    // Intersection point
    res.x = (c2 - c1) / (m1 - m2);
    res.y = res.x * m1 + c1;

    // If one of the lines is perpendicular to the ordinate axis, one of the
    // slopes will be equal to 0, and thus the final coordinates will either
    // be NaN or infinite. To prevent this, if one of the lines is
    // perpendicular to the ordinate, we compute the intersection of the other
    // line at its x value
    if (a1.x == a2.x)
      res = cv::Point2f(a1.x, a1.x * m2 + c2);
    else if (b1.x == b2.x)
      res = cv::Point2f(b2.x, b2.x * m1 + c1);

    return res;
  }

  // Refreshes the debug image and wait for a key press to continue
  void Detector::update_debug() const
  {
    imshow("detect debug", image_debug_);
    cv::waitKey(0);
  }

  // This static function calls the setCenter function from the class
  // to properly set the center (workaround, because opencv is C based)
  void setCenter(int event, int x, int y, int flags, void *userdata)
  {
    if (!userdata)
      return;

    if (event == cv::EVENT_LBUTTONDOWN)
    {
      Detector* detector = reinterpret_cast<Detector*>(userdata);
      detector->setCenter(x, y);
    }
  }
}
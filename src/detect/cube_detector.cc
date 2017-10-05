#include <detect/cube_detector.hh>

/* This class handles all the Detectors to recreate the cube from the camera
 * visions. The cameras are created in this order:
 *    Camera t1: TOP 1- Left is Green, Right is Red
 *    Camera t2: TOP 2- Left is Blue, Right is Orange
 *    Camera b1: BOTTOM 1- Left is Orange, Right is Green
 *    Camera b2: BOTTOM 2- Left is Red, Right is Blue
 *
 *  The top facelet is always White, the bottom one is always Yellow.
 *
 *  The Top cameras see the facelets in this order:
 *
 *               3         4
 *              14  1   2   8
 *                 12 0  6
 *                  10 5
 *                  11 7
 *                  13 9
 *
 *  The bottom cameras see the facelets in this order:
 *
 *                  12 3
 *                  11 1
 *                   9 0
 *                10  4  2
 *                 6     5
 *              8          7
 *
 * The cameras see a total of 56 faces, the following are duplicates:
 *    b1 7  - b2 8
 *    b1 8  - b2 7
 *    t1 3  - t2 4
 *    t1 4  - t2 3
 *    b1 3  - t1 14
 *    b1 12 - t2 8
 *    b2 3  - t2 14
 *    b2 12 - t1 8
*/

namespace detect
{
  CubeDetector::CubeDetector()
    : t1_(displayer_, CameraPosition::TOP, 1)
    , t2_(displayer_, CameraPosition::TOP, 1)
    , b1_(displayer_, CameraPosition::BOTTOM, 1)
    , b2_(displayer_, CameraPosition::BOTTOM, 1)
  { }

  void CubeDetector::detect_cube()
  {
    std::string result;

    colors_.clear();
    result.clear();

    t1_.update();
    t2_.update();
    b1_.update();
    b2_.update();

    if (ack_nb_faces() && pass_sanity_duplicates() && pass_unknown_colors())
    {
      // TOP (White face)
      result += t2_.getColors()[4];
      result += t2_.getColors()[2];
      result += t2_.getColors()[0];
      result += t1_.getColors()[1];
      result += 'U';
      result += t2_.getColors()[1];
      result += t1_.getColors()[4];
      result += t1_.getColors()[2];
      result += t1_.getColors()[0];

      // RIGHT (Blue face)
      result += t2_.getColors()[14];
      result += t2_.getColors()[12];
      result += t2_.getColors()[10];
      result += b2_.getColors()[1];
      result += 'R';
      result += t2_.getColors()[11];
      result += b2_.getColors()[0];
      result += b2_.getColors()[2];
      result += t2_.getColors()[13];

      // FRONT (Red face)
      result += t1_.getColors()[5];
      result += t1_.getColors()[6];
      result += t1_.getColors()[8];
      result += t1_.getColors()[7];
      result += 'F';
      result += b2_.getColors()[11];
      result += t1_.getColors()[9];
      result += b2_.getColors()[10];
      result += b2_.getColors()[9];

      // DOWN (Yellow face)
      result += b2_.getColors()[8];
      result += b2_.getColors()[6];
      result += b2_.getColors()[4];
      result += b1_.getColors()[5];
      result += 'D';
      result += b2_.getColors()[5];
      result += b1_.getColors()[8];
      result += b1_.getColors()[6];
      result += b1_.getColors()[4];

      // LEFT (Green face)
      result += t1_.getColors()[14];
      result += t1_.getColors()[12];
      result += t1_.getColors()[10];
      result += b1_.getColors()[1];
      result += 'L';
      result += t1_.getColors()[11];
      result += b1_.getColors()[0];
      result += b1_.getColors()[2];
      result += t1_.getColors()[13];

      // BACK (Orange face)
      result += t2_.getColors()[5];
      result += t2_.getColors()[6];
      result += t2_.getColors()[8];
      result += t2_.getColors()[7];
      result += 'B';
      result += b1_.getColors()[11];
      result += t2_.getColors()[9];
      result += b1_.getColors()[10];
      result += b1_.getColors()[9];

      return result;
    }
  }

  // Check that the detectors detect a total of 56 faces
  bool CubeDetector::ack_nb_faces()
  {
    if (b1_.getColors().size() + b2_.getColors().size()
        + t1_.getColors().size() + t2_.getColors().size() == 56)
      return true;

    return false;
  }

  // Check that the duplicates detected cells hold the same values
  bool CubeDetector::pass_sanity_duplicates()
  {
    if (b1_.getColors()[7] != b2_.getColors()[8])
      return false;
    if (b1_.getColors()[8] != b2_.getColors()[7])
      return false;
    if (t1_.getColors()[3] != t2_.getColors()[4])
      return false;
    if (t1_.getColors()[4] != t2_.getColors()[3])
      return false;
    if (b1_.getColors()[3] != t1_.getColors()[14])
      return false;
    if (b1_.getColors()[12] != t2_.getColors()[8])
      return false;
    if (b2_.getColors()[3] != t2_.getColors()[14])
      return false;
    if (b2_.getColors()[12] != t1_.getColors()[8])
      return false;

    return true;
  }

  // Check that there are no unknown colors detected by the detectors
  bool CubeDetector::pass_unknown_colors()
  {
    return no_unknown_colors(b1_) && no_unknown_colors(b2_)
      && no_unknown_colors(t1_) && no_unknown_colors(t2_);
  }

  bool CubeDetector::no_unknown_colors(Detector& d) const
  {
    for (auto c : d.getColors())
    {
      if (c == cube::color::UNKNOWN)
        return false;
    }

    return true;
  }
}

#ifndef TIDAL_TIDAL_SRC_COMPUTERVISION_WEBCAMOBJECTTRACKER_H_
#define TIDAL_TIDAL_SRC_COMPUTERVISION_WEBCAMOBJECTTRACKER_H_

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

// A singleton class which collects data from the user's webcam,
// detects objects of a certain colour using colour thresholding and
// calculates and stores the position of the object.
class WebcamObjectTracker
{
 private:
  // HSV related code
  int HSV_MIN = 0;
  int HSV_MAX = 256;

  // Colour we'll track (HSV)
  glm::ivec2 H = glm::ivec2(82, 158);
  glm::ivec2 S = glm::ivec2(173, 256);
  glm::ivec2 V = glm::ivec2(100, 185);


  // Default capture width and height
  const int FRAME_WIDTH = 640;
  const int FRAME_HEIGHT = 480;
  // Max number of objects to be detected in frame
  const int MAX_NUM_OBJECTS = 50;

  // Min & max object area
  const int MIN_OBJECT_AREA = 20 * 20;
  const int MAX_OBJECT_AREA = FRAME_HEIGHT * FRAME_WIDTH / 1.5;

  // Window names
  const std::string windowName = "Original Image";
  const std::string windowName1 = "HSV Image";
  const std::string windowName2 = "Thresholded Image";
  const std::string windowName3 = "After Morphological Operations";

  // Booleans to control aspects of class.
  bool ShouldTrackObjects = true;
  bool ShouldMorph = true;
  bool ShouldDisplayWindows = true;

  glm::ivec2 PPos = glm::ivec2(0); // Previous position
  glm::ivec2 Pos = glm::ivec2(0); // Position

  // Matrices to store frames
  Mat cameraFeed;
  Mat HSV;
  Mat threshold;
  VideoCapture capture;

 public:
  static WebcamObjectTracker *instance;
  WebcamObjectTracker();
  virtual ~WebcamObjectTracker() {};

  std::string intToString(int number);
  void DrawObject();

  void MorphOperations();
  void TrackObject();

  void Update();

  glm::ivec2 Position() { return Pos; }
  glm::ivec2 Offset() { return Pos - PPos; }
  glm::ivec2 MappedPosition(glm::ivec2 &source);
  glm::ivec2 MappedOffset() { return MappedPosition(Pos) - MappedPosition(PPos); }
};

#endif //TIDAL_TIDAL_SRC_COMPUTERVISION_WEBCAMOBJECTTRACKER_H_

#include "PCHeader.h"
#include "WebcamObjectTracker.h"
#include "Core/Window/Window.h"

WebcamObjectTracker *WebcamObjectTracker::instance = nullptr;

// Constructs the object, setting the instance to itself and
// performing the setup needed to capture web cam data.
WebcamObjectTracker::WebcamObjectTracker() {
  instance = this;
  capture.open(0);
  capture.set(CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
  capture.set(CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
}

// Converts an integer to std::string.
std::string WebcamObjectTracker::intToString(int number) {
  std::stringstream ss;
  ss << number;
  return ss.str();
}

// Draw an object, as a circle with cross-hairs.
void WebcamObjectTracker::DrawObject() {
  circle(cameraFeed, Point(Pos.x, Pos.y), 20, Scalar(0, 255, 0), 2);
  if (Pos.y - 25 > 0)
	line(cameraFeed, Point(Pos.x, Pos.y), Point(Pos.x, Pos.y - 25), Scalar(0, 255, 0), 2);
  else line(cameraFeed, Point(Pos.x, Pos.y), Point(Pos.x, 0), Scalar(0, 255, 0), 2);

  if (Pos.y + 25 < FRAME_HEIGHT)
	line(cameraFeed, Point(Pos.x, Pos.y), Point(Pos.x, Pos.y + 25), Scalar(0, 255, 0), 2);
  else line(cameraFeed, Point(Pos.x, Pos.y), Point(Pos.x, FRAME_HEIGHT), Scalar(0, 255, 0), 2);

  if (Pos.x - 25 > 0)
	line(cameraFeed, Point(Pos.x, Pos.y), Point(Pos.x - 25, Pos.y), Scalar(0, 255, 0), 2);
  else line(cameraFeed, Point(Pos.x, Pos.y), Point(0, Pos.y), Scalar(0, 255, 0), 2);

  if (Pos.x + 25 < FRAME_WIDTH)
	line(cameraFeed, Point(Pos.x, Pos.y), Point(Pos.x + 25, Pos.y), Scalar(0, 255, 0), 2);
  else line(cameraFeed, Point(Pos.x, Pos.y), Point(FRAME_WIDTH, Pos.y), Scalar(0, 255, 0), 2);

  putText(cameraFeed,
		  intToString(Pos.x) + "," + intToString(Pos.y),
		  Point(Pos.x, Pos.y + 30),
		  1,
		  1,
		  Scalar(0, 255, 0),
		  2);
}

// Performs morph operations on the image. In this implementation it
// erodes small objects, and combines (dilates) close objects.
void WebcamObjectTracker::MorphOperations() {
  Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
  Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

  erode(threshold, threshold, erodeElement);
  erode(threshold, threshold, erodeElement);

  dilate(threshold, threshold, dilateElement);
  dilate(threshold, threshold, dilateElement);
}

// Performs object tracking from the thresholded image.
void WebcamObjectTracker::TrackObject() {
  Mat temp;
  threshold.copyTo(temp);

  std::vector<std::vector<Point>> contours;
  std::vector<Vec4i> hierarchy;

  findContours(temp, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

  double refArea = 0;
  bool objectFound = false;
  if (hierarchy.size() > 0) {
	int numObjects = hierarchy.size();

	if (numObjects < MAX_NUM_OBJECTS) {
	  for (int index = 0; index >= 0; index = hierarchy[index][0]) {
		Moments moment = moments((cv::Mat) contours[index]);
		double area = moment.m00;

		if (area > MIN_OBJECT_AREA && area < MAX_OBJECT_AREA && area > refArea) {
		  PPos.x = Pos.x;
		  PPos.y = Pos.y;
		  Pos.x = moment.m10 / area;
		  Pos.y = moment.m01 / area;
		  objectFound = true;
		  refArea = area;
		} else {
		  objectFound = false;
		}
	  }
	  //let user know you found an object
	  if (objectFound == true) {
		putText(cameraFeed, "Tracking Object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
		// Draw the object location on screen
		DrawObject();
	  }
	}
  }
}

// Updates the class each frame. Captures a web cam image, creates the
// threshold image and then performs the morph, tracking and display operations.
void WebcamObjectTracker::Update() {
  // Read in the capture.
  capture.read(cameraFeed);

  // Convert to HSV colour
  cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);

  // Creates a thresholded image from the range.
  inRange(HSV, Scalar(H.x, S.x, V.x), Scalar(H.y, S.y, V.y), threshold);

  // Morph, if we need to.
  if (ShouldMorph) MorphOperations();

  // Track, if we need to.
  if (ShouldTrackObjects) TrackObject();

  // Display windows
  if (ShouldDisplayWindows) {
	imshow(windowName2, threshold);
	imshow(windowName, cameraFeed);
	imshow(windowName1, HSV);
  }

  waitKey(1);
}

// Maps the input point from web cam frame coordinates to the related
// application window coordindates.
glm::ivec2 WebcamObjectTracker::MappedPosition(glm::ivec2 &source) {
  // Map lambda.
  auto map = [&](int value, int min1, int max1, int min2, int max2) {
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
  };

  int x = map(source.x, 0, FRAME_WIDTH, 0, Window::instance->GetWidth());
  int y = map(source.y, FRAME_HEIGHT, 0, 0, Window::instance->GetHeight());

  return glm::ivec2(x, y);
}
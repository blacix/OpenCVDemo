#ifndef IMAGE_PROCESSING
#define IMAGE_PROCESSING

#include "cameracalibration.h"
#include "arucomarkerdetector.h"
#include "motiondetector.h"
#include "mobilenetssd.h"

#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>

#include <vector>
#include <thread>
#include <functional>
#include <future>


class ImageProcessing
{
public:

    static const char* APP_CONF_PATH;

	ImageProcessing();
    void loadConfig(const std::string& appConfigPath);
    void loadCameraParams(const std::string& path);
    void calibrateCamera(CalibrationResultCallback callback);
    void enableMotionDetection(bool enabled);
    inline bool isMotionDetectionEnabled() const {return mMotionDetector.isEnabled();}
    void enableMarkerDetection(bool enabled);
    inline bool isMarkerDetectionEnabled() const {return mMarkerDetector.isEnabled();}
    void enableMarkerDisplay(bool enabled);
    inline bool isMarkerDisplayEnabled() const {return mMarkerDetector.isMarkerDisplayEnabled();}

    void enableRawDisplay(bool enabled);
    inline bool isRawDisplayEnabled() const {return mRawDisplay;}

    void start(ArucoMarkerListCallback callback);
	void stop();

    bool isCameraCalibrated() const;

private:

    static void loadCameraParams(const std::string& fileName, cv::Mat& cameraMatrixOut, cv::Mat& distCoeffsOut);

    void processStream();
    void processImage(cv::Mat& image);

    cv::Mat undistort(const cv::Mat& image) const;
    cv::Mat undistort(const cv::Mat&& image) const;

    void showImage(const std::string& winTitle, const cv::Mat& image) const;
    void destroyWindow(const std::string& winTitle) const;
    void destroyAllWindow() const;

    // app
    ArucoMarkerDetector mMarkerDetector;
    MotionDetector mMotionDetector;
    MobilenetSSD mSSD;

    std::thread mThread;
    std::vector<std::future<void>> mFutures;
    cv::Mat mCurrentImage;
    cv::VideoCapture mVideoCapture;
    int mDeviceID;

    bool mRawDisplay;
    bool mRunning;

    // calibration
    cv::Mat mCameraMatrix;
    cv::Mat mDistCoeffs;

};

#endif

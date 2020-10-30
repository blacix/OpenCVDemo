#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H
#include <opencv2/core.hpp>

int cameraCalibrationMain(const std::string& configFile, const std::string& windowName, cv::Mat& cameraMatrixOut, cv::Mat& distCoeffsOut);
enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };

typedef std::function<void(bool)> CalibrationResultCallback;
#endif // CAMERACALIBRATION_H

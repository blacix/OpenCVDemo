#include "mobilenetssd.h"

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/trace.hpp>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace cv;
using namespace cv::dnn;
using namespace std;

static string CLASSES[] = {"background", "aeroplane", "bicycle", "bird", "boat",
    "bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
    "dog", "horse", "motorbike", "person", "pottedplant", "sheep",
    "sofa", "train", "tvmonitor"};

MobilenetSSD::MobilenetSSD()
{

}

void MobilenetSSD::doIt(const cv::Mat& image)
{
    CV_TRACE_FUNCTION();
    String modelTxt = "MobileNetSSD_deploy.prototxt";
    String modelBin = "MobileNetSSD_deploy.caffemodel";

//        String imageFile = (argc > 1) ? argv[1] : "space_shuttle.jpg";
    Net net = dnn::readNetFromCaffe(modelTxt, modelBin);
//    Net net = dnn::readNetFromTensorflow("freeze_graph.pb");
    if (net.empty())
    {
        std::cerr << "Can't load network by using the following files: " << std::endl;
        std::cerr << "prototxt:   " << modelTxt << std::endl;
        std::cerr << "caffemodel: " << modelBin << std::endl;
        exit(-1);
    }
    Mat img = image.clone();

    Mat imageResized;
    resize(img, imageResized, Size(300,300));
    Mat inputBlob = blobFromImage(imageResized, 0.007843, Size(300,300), Scalar(127.5, 127.5, 127.5), false);

    net.setInput(inputBlob, "data");
    Mat detection = net.forward("detection_out");
    Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    ostringstream ss;
    float confidenceThreshold = 0.2;
    for (int i = 0; i < detectionMat.rows; i++)
    {
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence > confidenceThreshold)
        {
            int idx = static_cast<int>(detectionMat.at<float>(i, 1));
            int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * img.cols);
            int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * img.rows);
            int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * img.cols);
            int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * img.rows);

            Rect object((int)xLeftBottom, (int)yLeftBottom,
                        (int)(xRightTop - xLeftBottom),
                        (int)(yRightTop - yLeftBottom));

            rectangle(img, object, Scalar(0, 255, 0), 2);

            cout << CLASSES[idx] << ": " << confidence << endl;

            ss.str("");
            ss << confidence;
            String conf(ss.str());
            String label = CLASSES[idx] + ": " + conf;
            int baseLine = 0;
            Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
            putText(img, label, Point(xLeftBottom, yLeftBottom),
                    FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0));
        }
    }
    imshow("detections", img);
    waitKey(1);
}
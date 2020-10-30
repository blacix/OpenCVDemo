#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <vector>
#include <sstream>
#include <cstdio>

#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/aruco/dictionary.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

int main(int argc, char* argv[])
{

    qRegisterMetaType<QVector<int> >("QVector<int>");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

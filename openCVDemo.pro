QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14


win32 {
    INCLUDEPATH += $$(OPENCV_DIR)/include

    CONFIG( debug, debug|release ) {
        # debug
        LIBS += -L$$(OPENCV_DIR)/x64/vc16/lib -lopencv_core440d -lopencv_aruco440d -lopencv_highgui440d -lopencv_imgproc440d -lopencv_videoio440d -lopencv_calib3d440d -lopencv_features2d440d -lopencv_imgcodecs440d -lopencv_objdetect440d -lopencv_dnn_objdetect440d -lopencv_dnn440d
        # QMAKE_LIBDIR += LD$$(OPENCV_DIR)/x64/vc16/lib
    } else {
        # release
        LIBS += -L$$(OPENCV_DIR)/x64/vc16/lib -lopencv_core440 -lopencv_aruco440 -lopencv_highgui440 -lopencv_imgproc440 -lopencv_videoio440 -lopencv_calib3d440 -lopencv_features2d440d -lopencv_imgcodecs440 -lopencv_objdetect440 -lopencv_dnn_objdetect440 -lopencv_dnn440
        # QMAKE_LIBDIR += L$$(OPENCV_DIR)/x64/vc16/lib
        # c:\opencv\build\install\x64\vc16\lib\opencv_objdetect440.lib
    }
}


unix {
    INCLUDEPATH += /usr/local/include/opencv4
    LIBS += -L/usr/local/lib -lopencv_core -lopencv_aruco -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_calib3d -lopencv_features2d -lopencv_imgcodecs -lopencv_objdetect -lopencv_dnn_objdetect -lopencv_dnn
}


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arucomarkerdetector.cpp \
    cameracalibration.cpp \
    openCVDemo.cpp \
    mainwindow.cpp \
    ImageProcessing.cpp \
    mobilenetssd.cpp \
    motiondetector.cpp

HEADERS += \
    ImageProcessor.h \
    arucomarkerdetector.h \
    cameracalibration.h \
    mainwindow.h \
    ImageProcessing.h \
    mobilenetssd.h \
    motiondetector.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

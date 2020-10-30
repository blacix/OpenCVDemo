#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include "ImageProcessing.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void callback(int idCount);

signals:
    void signalArucoIdCount(QVector<int> ids);
    void signalCameraCalibrationResult(bool success);

private slots:
    void slotArucoIdCount(QVector<int> ids);
    void slotCameraCalibrationResult(bool success);

    void on_buttonCameraCalibration_clicked();
    void on_buttonLoadCameraParams_clicked();
    void on_buttonStart_clicked();
    void on_checkBoxMarkerDetection_stateChanged(int state);
    void on_checkBoxMotionDetection_stateChanged(int state);
    void on_checkBoxDisplayRaw_stateChanged(int state);
    void on_checkBoxMarkerDisplay_stateChanged(int arg1);
    void on_buttonLoadConfig_clicked();

private:
    void closeEvent(QCloseEvent *event) override;
    void loadConfig(const std::string& path);
    void updateConfigDisplay();

    Ui::MainWindow *ui;
    ImageProcessing mImageProcessing;
};
#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <functional>
#include <algorithm>
#include <QtGlobal>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(this, &MainWindow::signalArucoIdCount, this, &MainWindow::slotArucoIdCount);
    QObject::connect(this, &MainWindow::signalCameraCalibrationResult, this, &MainWindow::slotCameraCalibrationResult);

    updateConfigDisplay();
}

void MainWindow::slotArucoIdCount(QVector<int> ids)
{
    QString idString;
    for(int id : ids)
    {
        idString.append(QString::number(id)).append(", ");
    }

    this->ui->label_id_count->setText(QString::number(ids.size()));
    this->ui->label_id_list->setText(idString);
}

void MainWindow::slotCameraCalibrationResult(bool success)
{
    ui->checkBoxCameraCalibrated->setChecked(success);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    std::cout << "closeEvent" << std::endl;
    mImageProcessing.stop();

    QMainWindow::closeEvent(event);
}


MainWindow::~MainWindow()
{
    mImageProcessing.stop();
    delete ui;
}

void MainWindow::on_buttonCameraCalibration_clicked()
{
    auto callback = [=](bool success)->void
    {
        emit signalCameraCalibrationResult(success);
    };
    mImageProcessing.calibrateCamera(callback);
}

void MainWindow::on_buttonLoadCameraParams_clicked()
{
    mImageProcessing.loadCameraParams(ui->lineEditCameraParamsPath->text().trimmed().toStdString());
    ui->checkBoxCameraCalibrated->setChecked(mImageProcessing.isCameraCalibrated());
}

void MainWindow::on_buttonStart_clicked()
{
    ArucoMarkerListCallback callback = [&](std::vector<int> ids)->void
    {
        std::sort(ids.begin(), ids.end());
        QVector<int> qIDs;
        #if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
            qIDs = QVector<int>::fromStdVector(ids);
        #else
            qIDs = QVector<int>(ids.begin(), ids.end());
        #endif

        emit signalArucoIdCount(qIDs);
    };

    mImageProcessing.start(callback);
}

void MainWindow::on_checkBoxMarkerDetection_stateChanged(int state)
{
    mImageProcessing.enableMarkerDetection(state == Qt::Checked);
}

void MainWindow::on_checkBoxMotionDetection_stateChanged(int state)
{
    mImageProcessing.enableMotionDetection(state == Qt::Checked);
}

void MainWindow::on_checkBoxDisplayRaw_stateChanged(int state)
{
    mImageProcessing.enableRawDisplay(state == Qt::Checked);
}

void MainWindow::on_checkBoxMarkerDisplay_stateChanged(int state)
{
    mImageProcessing.enableMarkerDisplay(state == Qt::Checked);
}

void MainWindow::on_buttonLoadConfig_clicked()
{
    loadConfig(ui->lineEditConfigPath->text().trimmed().toStdString());
}

void MainWindow::loadConfig(const std::string& path)
{
    mImageProcessing.loadConfig(path.c_str());
    updateConfigDisplay();
}

void MainWindow::updateConfigDisplay()
{
    ui->checkBoxDisplayRaw->setChecked(mImageProcessing.isRawDisplayEnabled());
    ui->checkBoxMarkerDetection->setChecked(mImageProcessing.isMarkerDetectionEnabled());
    ui->checkBoxMarkerDisplay->setChecked(mImageProcessing.isMarkerDisplayEnabled());
    ui->checkBoxMotionDetection->setChecked(mImageProcessing.isMotionDetectionEnabled());
}

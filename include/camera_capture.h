#pragma once

#include <QObject>

#include <opencv2/opencv.hpp>

namespace logger
{
    class Logger;
}

class CameraCapture : public QObject
{
    Q_OBJECT

public:
    CameraCapture(const QJsonObject& config);
    ~CameraCapture();
    void configure(const QJsonObject& config);
private:

public slots:
    void onUpdate();
private slots:
    void startCamera();
    void stopCamera();
    void displayCameraError();
signals:
    void updateFrame(cv::Mat frame);
private:
    int mDeviceID{ 0 };
    std::unique_ptr<logger::Logger> mLogger;
    cv::Mat mFrame;
    cv::VideoCapture mCap;

};

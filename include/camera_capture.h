#pragma once

#include "tracker_global.h"
#include "frame_source.h"

class CameraCapture : public FrameSource
{
    Q_OBJECT
public:
    CameraCapture(const QJsonObject& config);
    ~CameraCapture();
    void configure(const QJsonObject& config);
private:

public slots:
    void onUpdate();
signals:
    void updateFrame(ProcessingStruct processing);
private:
    int mDeviceID{ 0 };
    cv::Mat mFrame;
    cv::VideoCapture mCap;

};

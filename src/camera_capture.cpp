#include "camera_capture.h"

#include "logger.hpp"

#include <QJsonObject>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <stdio.h>

CameraCapture::CameraCapture(const QJsonObject& config)
    : FrameSource(config)
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
    configure(config);
}

CameraCapture::~CameraCapture()
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
}

void CameraCapture::configure(const QJsonObject& config)
{
    mLogger = std::make_unique<logger::Logger>(logger::LogType::CAMERA_CAPTURE, logger::LogLevel::MEDIUM, logger::LogFunction::YES);
    mLogger->showThreadId(true);

    mDeviceID = config[DEVICE_ID].toInt();
    mLogger->print(QString("mDeviceID:%1").arg(mDeviceID), __FUNCTION__);
    
    int apiID = cv::CAP_ANY;
    mCap.open(mDeviceID, apiID);
    if (!mCap.isOpened())
    {
        mLogger->printError(QString("Unable to open camera"), __FUNCTION__);
    }
    mLogger->print(QString("getBackendName:%1").arg(QString::fromStdString(mCap.getBackendName())), __FUNCTION__);

    mCap.read(mFrame);
    if (mFrame.empty())
    {
        mLogger->printError(QString("Blank frame grabbed"), __FUNCTION__);
    }
    mLogger->print(QString("mFrame:(%1 x %2)").arg(mFrame.rows).arg(mFrame.cols), __FUNCTION__);
}

void CameraCapture::onUpdate()
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
    mCap.read(mFrame);
    if (mFrame.empty()) {
        std::cerr << "ERROR! blank frame grabbed\n";
    }

    emit updateFrame({mFrame, QDateTime::currentMSecsSinceEpoch() });
}

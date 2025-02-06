#include "camera_capture.h"

#include "global.h"
#include "logger.hpp"

#include <QMessageBox>
#include <QJsonObject>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <stdio.h>

CameraCapture::CameraCapture(QJsonObject const& config)
{
    configure(config);
}

CameraCapture::CameraCapture()
{

}

void CameraCapture::configure(QJsonObject const& config)
{
    logger::Logger logger(logger::LogType::CAMERA_CAPTURE, logger::LogLevel::HIGH, logger::LogFunction::YES);
    
    mDeviceID = config[DEVICE_ID].toInt();
    logger.print(QString("mDeviceID:%1").arg(mDeviceID));
}

void CameraCapture::startCamera()
{

}

void CameraCapture::stopCamera()
{

}

void CameraCapture::displayCameraError()
{

}

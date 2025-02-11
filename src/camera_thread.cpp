#include "camera_thread.h"

#include "logger.hpp"
#include <opencv2/opencv.hpp>

#include <QDateTime>

CameraThread::CameraThread(const QJsonObject& config)
{
    configure(config);
}

CameraThread::~CameraThread()
{}

void CameraThread::configure(const QJsonObject& config)
{
    mLogger = std::make_unique<logger::Logger>(logger::LogType::CAMERA_THREAD, logger::LogLevel::MEDIUM, logger::LogFunction::YES);
    timestamp = QDateTime::currentMSecsSinceEpoch();
}

void CameraThread::onUpdate()
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::LOW);
}

void CameraThread::onUpdateFrame(cv::Mat frame)
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::LOW);
    //cv::imshow("frame", frame);
    //cv::waitKey(0);
        
}
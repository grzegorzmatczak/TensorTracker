#include "camera_thread.h"

#include "global.h"
#include "logger.hpp"

#include <QJsonObject>
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
    mTimestamp = QDateTime::currentMSecsSinceEpoch();
    mWidth = config[RESIZE_WIDTH].toInt();
    mHeight = config[RESIZE_HEIGHT].toInt();
}

void CameraThread::initProcessing()
{

}

void CameraThread::onUpdate()
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
    emit updateFrame({ mGrayFrame, mTimestamp });
}

void CameraThread::onUpdateFrame(ProcessingStruct processing)
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
    cv::Mat resizedFrame;
    mTimestamp = processing.timestamp;
    if (processing.frame.cols == 0 || processing.frame.rows == 0)
    {
        mLogger->printError(QString("processing.frame.cols:%1 processing.frame.rows:%2 not correct")
            .arg(processing.frame.cols).arg(processing.frame.rows), __FUNCTION__);
        return;
    }
    if (processing.frame.channels() == 3)
    {
        cv::resize(processing.frame, resizedFrame, cv::Size(mWidth, mHeight));
        cv::cvtColor(resizedFrame, mGrayFrame, cv::COLOR_BGR2GRAY);
    }
    else if (processing.frame.channels() == 1)
    {
        cv::resize(processing.frame, mGrayFrame, cv::Size(mWidth, mHeight));
    }
    auto timestampProcess = QDateTime::currentMSecsSinceEpoch() - processing.timestamp;
    mLogger->print(QString("timestampProcess:%1").arg(timestampProcess), logger::LogLevel::MEDIUM, __FUNCTION__);
}
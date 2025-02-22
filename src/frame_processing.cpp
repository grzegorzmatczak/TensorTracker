#include "frame_processing.h"

#include "global.h"
#include "logger.hpp"
#include "medianfilter.hpp"

#include <QJsonObject>
#include <QDateTime>
#include <QElapsedTimer>

FrameProcessing::FrameProcessing(const QJsonObject& config)
{
    configure(config);
}

FrameProcessing::~FrameProcessing()
{
}

void FrameProcessing::configure(const QJsonObject& config)
{
    mLogger = std::make_unique<logger::Logger>(logger::LogType::FRAME_PROCESSING, logger::LogLevel::MEDIUM, logger::LogFunction::YES);
    mTimestamp = QDateTime::currentMSecsSinceEpoch();
    mWidth = config[RESIZE_WIDTH].toInt();
    mHeight = config[RESIZE_HEIGHT].toInt();
    cv::Mat frame = cv::Mat(mHeight, mWidth, CV_8UC1, cv::Scalar(0));
    mTimer = new QElapsedTimer();
    mMedian = cv::Mat(mHeight, mWidth, CV_8UC1, cv::Scalar(0));
    mDiffFrame = cv::Mat(mHeight, mWidth, CV_8UC1, cv::Scalar(0));
}

void FrameProcessing::onUpdate()
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
}

void FrameProcessing::onUpdateMedian(ProcessingStruct processing)
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
    mMedian = processing.frame;
}

void FrameProcessing::onUpdateFrame(ProcessingStruct processing)
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
    mTimer->start();
    if (processing.frame.cols != mWidth || processing.frame.rows != mHeight || processing.frame.channels() != 1)
    {
        mLogger->printError(QString("mWidth:%1 mHeight:%2 not correct").arg(mWidth).arg(mHeight), __FUNCTION__);
        return;
    }
    
    mLogger->print(QString("mFrame:(%1 x %2 x %3) type:%4")
        .arg(processing.frame.rows).arg(processing.frame.cols)
        .arg(processing.frame.channels()).arg(processing.frame.type()), __FUNCTION__);

    if (processing.frame.cols != mWidth || processing.frame.rows != mHeight || processing.frame.channels() != 1)
    {
        mLogger->print(QString("mWidth:%1 mHeight:%2").arg(mWidth).arg(mHeight), __FUNCTION__);
        return;
    }
    cv::absdiff(processing.frame, mMedian, mDiffFrame);
    cv::convertScaleAbs(mDiffFrame, mDiffFrame);
    cv::threshold(mDiffFrame, mDiffFrameThresh, 30, 255, 0);
    cv::medianBlur(mDiffFrameThresh, mDiffFrameThresh, 3);
    emit showImagesOpenCV(mDiffFrameThresh, "mDiffFrameThresh");
    mLogger->print(QString("time:%1[ms]").arg(mTimer->restart()), logger::LogLevel::MEDIUM, __FUNCTION__);
    mLogger->printEndFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
}
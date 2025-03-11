#include "frame_median.h"

#include "logger.hpp"
#include "medianfilter.hpp"

#include <QJsonObject>
#include <QDateTime>
#include <QElapsedTimer>

FrameMedian::FrameMedian(const QJsonObject& config)
{
    configure(config);
}

FrameMedian::~FrameMedian()
{
}

void FrameMedian::configure(const QJsonObject& config)
{
    mLogger = std::make_unique<logger::Logger>(logger::LogType::FRAME_MEDIAN, logger::LogLevel::MEDIUM, logger::LogFunction::YES);
    mTimestamp = QDateTime::currentMSecsSinceEpoch();
    mWidth = config[RESIZE_WIDTH].toInt();
    mHeight = config[RESIZE_HEIGHT].toInt();
    mSizeOfMedian = config[MEDIAN_SIZE].toInt();

    cv::Mat frame = cv::Mat(mHeight, mWidth, CV_8UC1, cv::Scalar(0));
    mMedianFilter = new MedianFilter(frame, mSizeOfMedian);
    mTimer = new QElapsedTimer();
}

void FrameMedian::onUpdate()
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
}

void FrameMedian::onUpdateFrame(ProcessingStruct processing)
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

    
    mMedianFilter->removeNode();
    mMedianFilter->addImage(processing.frame);
    mMedian = mMedianFilter->getMedian();
    emit updateMedian({ mMedian, mTimestamp });
    emit showImagesOpenCV(mMedian, "mMedian");
    emit showImagesOpenCV(processing.frame, "processing.frame");
    mLogger->print(QString("time median:%1[ms]").arg(mTimer->restart()), logger::LogLevel::MEDIUM, __FUNCTION__);
    mLogger->printEndFunction(__FUNCTION__, logger::LogLevel::MEDIUM);
}
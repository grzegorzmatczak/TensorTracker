#include "service.h"

#include "global.h"
#include "logger.hpp"

#include <stdio.h>
#include <iostream>
#include <vector>

#include <QJsonObject>
#include <QThread>
#include <QTimer>

#include "camera_capture.h"
#include "camera_thread.h"
#include "frame_processing.h"
#include "frame_median.h"
#include "frame_reader.h"

Service::Service()
{

}

Service::Service(const QJsonObject& config)
{
    configure(config);
    createStartupThreads(config);
}

Service::~Service()
{

}

void Service::configure(const QJsonObject& config)
{
    mLogger = std::make_unique<logger::Logger>(logger::LogType::SERVICE, logger::LogLevel::MEDIUM, logger::LogFunction::YES);
    mLogger->showThreadId(true);
    mTimer = new QTimer(this);
    mTimerCamera = new QTimer(this);

    connect(mTimer, &QTimer::timeout, this, &Service::onUpdate);
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<ProcessingStruct>("ProcessingStruct");
}

void Service::createStartupThreads(const QJsonObject& config) 
{
    QJsonObject serviceConfig{ config[SERVICE].toObject() };
    mCameraFPS = serviceConfig[CAMERA_FPS].toInt();
    mLogger->print(QString("mCameraFPS:%1").arg(mCameraFPS), __FUNCTION__);
    if (mCameraFPS > 0 && mCameraFPS < 100)
    {
        mCameraTimerValue = 1000 / mCameraFPS;
        mLogger->print(QString("mCameraTimerValue:%1").arg(mCameraTimerValue), __FUNCTION__);
    }

    //QJsonObject cameraCaptureConfig{ config[CAMERA_CAPTURE_CONFIG].toObject() };
    //capture = new CameraCapture(cameraCaptureConfig);
    QJsonObject frameReaderConfig{ config[FRAME_READER_CONFIG].toObject() };
    capture = new FrameReader(frameReaderConfig);

    captureThread = new QThread();
    capture->moveToThread(captureThread);
    connect(mTimer, &QTimer::timeout, capture, &FrameSource::onUpdate);
    captureThread->start();

    QJsonObject cameraThreadConfig{ config[CAMERA_THREAD_CONFIG].toObject() };
    camera = new CameraThread(cameraThreadConfig);
    cameraThread = new QThread();
    camera->moveToThread(cameraThread);
    connect(mTimerCamera, &QTimer::timeout, camera, &CameraThread::onUpdate);
    cameraThread->start();
    connect(capture, &FrameSource::updateFrame, camera, &CameraThread::onUpdateFrame);

    QJsonObject frameProcessingConfig{ config[FRAME_PROCESSING_CONFIG].toObject() };
    frameProcessing = new FrameProcessing(frameProcessingConfig);
    frameProcessingThread = new QThread();
    frameProcessing->moveToThread(frameProcessingThread);
    frameProcessingThread->start();
    connect(camera, &CameraThread::updateFrame, frameProcessing, &FrameProcessing::onUpdateFrame);
    connect(frameProcessing, &FrameProcessing::showImagesOpenCV, this, &Service::onShowImagesOpenCV);

    QJsonObject frameMedianConfig{ config[FRAME_MEDIAN_CONFIG].toObject() };
    frameMedian = new FrameMedian(frameMedianConfig);
    frameMedianThread = new QThread();
    frameMedian->moveToThread(frameMedianThread);
    frameMedianThread->start();
    connect(camera, &CameraThread::updateFrame, frameMedian, &FrameMedian::onUpdateFrame);
    connect(frameMedian, &FrameMedian::updateMedian, frameProcessing, &FrameProcessing::onUpdateMedian);
    connect(frameMedian, &FrameMedian::showImagesOpenCV, this, &Service::onShowImagesOpenCV);
    
    mTimerCamera->start(mCameraTimerValue);
    mTimer->start(100);
}

void Service::onUpdate()
{
    mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::LOW);
}

void Service::onShowImagesOpenCV(cv::Mat frame, std::string name)
{
    if ((frame.rows > 1) && (frame.cols > 1) && (!frame.empty()))
    {
        cv::imshow(name, frame);
        cv::waitKey(1);
    }
}
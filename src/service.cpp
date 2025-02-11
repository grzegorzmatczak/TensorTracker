#include "service.h"

#include "global.h"
#include "logger.hpp"

#include <stdio.h>
#include <iostream>
#include <vector>

#include <QJsonObject>
#include <QThread>
#include <QTimer>

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
	mLogger = new logger::Logger(logger::LogType::SERVICE, logger::LogLevel::HIGH, logger::LogFunction::YES);
	mLogger->showThreadId(true);
	mTimer = new QTimer(this);
	mTimerCamera = new QTimer(this);
	
	connect(mTimer, &QTimer::timeout, this, &Service::onUpdate);
	qRegisterMetaType<cv::Mat>("cv::Mat");
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

	QJsonObject cameraCaptureConfig{ config[CAMERA_CAPTURE_CONFIG].toObject() };
	capture = new CameraCapture(cameraCaptureConfig);
	captureThread = new QThread();
	capture->moveToThread(captureThread);
	connect(mTimer, &QTimer::timeout, capture, &CameraCapture::onUpdate);
	captureThread->start();

	QJsonObject cameraThreadConfig{ config[CAMERA_THREAD_CONFIG].toObject() };
	camera = new CameraThread(cameraThreadConfig);
	cameraThread = new QThread();
	camera->moveToThread(cameraThread);
	connect(mTimerCamera, &QTimer::timeout, camera, &CameraThread::onUpdate);
	cameraThread->start();

	connect(capture, &CameraCapture::updateFrame, camera, &CameraThread::onUpdateFrame);

	mTimerCamera->start(mCameraTimerValue);
	mTimer->start(100);
}

void Service::onUpdate()
{
	mLogger->printStartFunction(__FUNCTION__, logger::LogLevel::LOW);
}

#include "service.h"

#include "global.h"

#include <stdio.h>
#include <iostream>
#include <vector>

#include <QJsonObject>

Service::Service()
{

}

Service::Service(QJsonObject const& config)
{
	configure(config);
	createStartupThreads(config);
}

Service::~Service()
{

}

void Service::configure(QJsonObject const& config)
{

}

void Service::createStartupThreads(QJsonObject const& config)
{
	QJsonObject cameraCaptureConfig{ config[CAMERA_CAPTURE_CONFIG].toObject() };
	capture = new CameraCapture(cameraCaptureConfig);
	captureThread = new QThread();

	capture->moveToThread(captureThread);
}

#pragma once

#include "camera_capture.h"

#include <QObject>
#include <QThread>

class Service : public QObject
{
	Q_OBJECT

public:
	Service();
	Service(QJsonObject const& config);
	~Service();
	void configure(QJsonObject const& config);

private:
	void createStartupThreads(QJsonObject const& config);

	CameraCapture* capture{ nullptr };
	QThread* captureThread{ nullptr };
};

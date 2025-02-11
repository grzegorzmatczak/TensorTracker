#pragma once

#include "camera_capture.h"
#include "camera_thread.h"

#include <QObject>

namespace logger
{
	class Logger;
}

class Service : public QObject
{
	Q_OBJECT

public:
	Service();
	Service(const QJsonObject& config);
	~Service();
	void configure(const QJsonObject& config);
private slots:
	void onUpdate();
private:
	void createStartupThreads(const QJsonObject& config);
private:
	CameraCapture* capture{ nullptr };
	QThread* captureThread{ nullptr };
	CameraThread* camera{ nullptr };
	QThread* cameraThread{ nullptr };

	QTimer* mTimer{ nullptr };
	QTimer* mTimerCamera{ nullptr };

	logger::Logger* mLogger;

	int mCameraTimerValue{ 100 };
	int mCameraFPS{ 25 };

};

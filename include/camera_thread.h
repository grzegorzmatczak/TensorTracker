#pragma once

#include <QObject>

#include <opencv2/opencv.hpp>

namespace logger
{
	class Logger;
}

class CameraThread : public QObject
{
	Q_OBJECT

public:
	CameraThread(const QJsonObject& config);
	~CameraThread();
	void configure(const QJsonObject& config);

public slots:
	void onUpdate();
	void onUpdateFrame(cv::Mat frame);
private:
	qint64 timestamp{};
	logger::Logger* mLogger;

};
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QCameraGrabberProcessDemo.h"

#include <QTimer>
#include <QLabel>
#include <QStackedWidget>
#include <QScrollBar>
#include <QPainter>

#include "MatProcessorManager.h"
#include "VR_3DCanvas.h"


class QCameraGrabberProcessDemo : public QMainWindow
{
	Q_OBJECT

public:
	QCameraGrabberProcessDemo(QWidget *parent = Q_NULLPTR);

private slots:
	void process();
	void processSelectionChanged();

private:
	Ui::QCameraGrabberProcessDemoClass ui;

	QTimer mTimer;

	size_t mCurrentProcessView;
	QLabel * mImageViewer;
	QLabel * mFPSViewer;
	QScrollBar * mProcessSelector;
	QStackedWidget * mControlInterface;
	VR_3DCanvas mScene3D;

	MatProcessorManager mProcessManager;

	static QImage::Format opencvToQtImageFormat(cv::Mat const & image);

};











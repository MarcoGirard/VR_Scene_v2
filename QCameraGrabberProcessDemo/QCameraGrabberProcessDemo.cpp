#include "QCameraGrabberProcessDemo.h"

#include "opencv2/core.hpp"

#include <QElapsedTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "AverageBuilder.h"

#include <QDebug>


QCameraGrabberProcessDemo::QCameraGrabberProcessDemo(QWidget *parent)
	: QMainWindow(parent), mCurrentProcessView{ 0 }
{
	ui.setupUi(this);

	mProcessManager.appendProcessor(new MatGrabProcessor("Grab"));
	mProcessManager.appendProcessor(new MatFlipProcessor("Flip", MatFlipProcessor::FlipSide::Horizontally));
	mProcessManager.appendProcessor(new MatBlurProcessor("Blur", 13));
	mProcessManager.appendProcessor(new MatRGBToHSVProcessor("toHSV"));
	mProcessManager.appendProcessor(new Mat3CThresholdProcessor("Threshold", 55,88,27,255,68,255));
	mProcessManager.appendProcessor(new MatBlobFind("Blob"));

	mImageViewer = new QLabel;
	mFPSViewer = new QLabel;
	mFPSViewer->setAlignment(Qt::AlignVCenter & Qt::AlignRight);
	QVBoxLayout * leftLayout{ new QVBoxLayout };
	leftLayout->addWidget(mImageViewer);
	leftLayout->addWidget(mFPSViewer);
	leftLayout->addStretch();

	mProcessSelector = new QScrollBar;
	mProcessSelector->setOrientation(Qt::Horizontal);
	mControlInterface = new QStackedWidget;
	mControlInterface->setCurrentIndex(5);
	QVBoxLayout * rightLayout{ new QVBoxLayout };
	rightLayout->addWidget(mProcessSelector);
	rightLayout->addWidget(mControlInterface);
	rightLayout->addStretch();

	QWidget * mainWidget{ new QWidget };
	QHBoxLayout * mainLayout{ new QHBoxLayout };
	mainWidget->setLayout(mainLayout);
	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(rightLayout);

	mProcessSelector->setRange(0, mProcessManager.processorCount() - 1);
	mProcessSelector->setValue(0);
	for (int i{ 0 }; i < mProcessManager.processorCount(); ++i) {
		mControlInterface->addWidget(mProcessManager.controlInterface(i));
	}

	setCentralWidget(mainWidget);

	connect(mProcessSelector, &QScrollBar::valueChanged, this, &QCameraGrabberProcessDemo::processSelectionChanged);


	connect(&mTimer, &QTimer::timeout, this, &QCameraGrabberProcessDemo::process);
	mScene3D.show();
	mTimer.start(20);
}

void QCameraGrabberProcessDemo::processSelectionChanged()
{
	mControlInterface->setCurrentIndex(mProcessSelector->value());
	mCurrentProcessView = mProcessSelector->value();
}


void QCameraGrabberProcessDemo::process()
{
	static AverageBuilder<double, 10> averageFPS;
	static QElapsedTimer elapsedTimer;
	double v = 1000.0 / elapsedTimer.restart();
	qDebug() << v;
	averageFPS.add(v); // 1.0 / (elapsedTimer.restart() / 1000.0)

	mProcessManager.process();
	mScene3D.setViewerPosition(mProcessManager.viewerPosition());
	//mVideoCaptureDevice >> mCapturedImage;
	cv::Mat const & result{ mProcessManager.image(mCurrentProcessView) };
	mImageViewer->setPixmap(QPixmap::fromImage(QImage((unsigned char*)result.data, result.cols, result.rows, opencvToQtImageFormat(result)).rgbSwapped()));
	mFPSViewer->setText(QString("FPS : %1").arg(averageFPS.average(), 6, 'f', 3, QChar('0')));
}




QImage::Format QCameraGrabberProcessDemo::opencvToQtImageFormat(cv::Mat const & image)
{
	switch (image.type()) {
		case CV_8U: return QImage::Format_Grayscale8;
		case CV_8UC3: return QImage::Format_RGB888;
		case CV_8UC4: return QImage::Format_RGB32;
	}

	return QImage::Format_Invalid;
}




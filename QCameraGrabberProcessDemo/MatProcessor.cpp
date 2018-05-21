#include "MatProcessor.h"



#include <QLabel>
#include <QVBoxLayout>

QWidget* MatProcessor::createControlInterface()
{
	QLabel * widget{ new QLabel };
	widget->setAlignment(Qt::AlignCenter);
	widget->setText("No control available");
	widget->setEnabled(false);

	return widget;
}

QWidget * MatProcessor::packageInterface(QWidget * widget)
{
	QWidget * package{ new QWidget };
	QVBoxLayout * layout{ new QVBoxLayout };
	package->setLayout(layout);

	QLabel * title{ new QLabel(QString::fromStdString(mName)) };
	title->setAlignment(Qt::AlignCenter);
	QFont titleFont{ title->font() };
	titleFont.setBold(true);
	title->setFont(titleFont);

	layout->addWidget(title);
	layout->addWidget(widget);
	layout->addStretch();

	return package;
}


MatGrabProcessor::MatGrabProcessor(std::string const & name, int cameraDeviceIndex)
	: MatProcessor(name), mCameraDeviceIndex{ cameraDeviceIndex }
{
	if (!mVideoCaptureDevice.open(mCameraDeviceIndex)) {
		throw cv::Exception(0, "Cannot initialize video capture device.", "MatGrabber::MatGrabber", __FILE__, __LINE__);
	}
}
void MatGrabProcessor::process(cv::Mat const & inputImage)
{
	(void)inputImage;

	//mImage = cv::imread("frame_1.jpg", CV_LOAD_IMAGE_UNCHANGED);
	mVideoCaptureDevice >> mImage;

}


#include <QGroupBox>
#include <QRadioButton>
#include <QVBoxLayout>

MatFlipProcessor::MatFlipProcessor(std::string const & name, FlipSide flipSide)
	: MatProcessor(name), mFlipCode{ static_cast<int>(flipSide) }
{
}

void MatFlipProcessor::process(cv::Mat const & inputImage)
{
	cv::flip(inputImage, mImage, mFlipCode);
}

QWidget* MatFlipProcessor::createControlInterface()
{
	QGroupBox * widget{ new QGroupBox };
	widget->setTitle("Flip option");
	QVBoxLayout * layout{ new QVBoxLayout };
	widget->setLayout(layout);

	QRadioButton * flipHorizontally{ new QRadioButton("Horizontally") };
	QRadioButton * flipVertically{ new QRadioButton("Vertically") };
	QRadioButton * flipBoth{ new QRadioButton("Both") };

	layout->addWidget(flipHorizontally);
	layout->addWidget(flipVertically);
	layout->addWidget(flipBoth);
	layout->addStretch();

	QObject::connect(flipHorizontally, &QRadioButton::toggled, [this]()->void { this->setFlipSide(FlipSide::Horizontally); });
	QObject::connect(flipVertically, &QRadioButton::toggled, [this]()->void { this->setFlipSide(FlipSide::Vertically); });
	QObject::connect(flipBoth, &QRadioButton::toggled, [this]()->void { this->setFlipSide(FlipSide::Both); });

	switch (static_cast<FlipSide>(mFlipCode)) {
		case FlipSide::Horizontally :
			flipHorizontally->setChecked(true);
			break;
		case FlipSide::Vertically:
			flipVertically->setChecked(true);
			break;
		case FlipSide::Both:
			flipBoth->setChecked(true);
			break;
	}

	return widget;
}


#include <QScrollBar>
MatBlurProcessor::MatBlurProcessor(std::string const & name, size_t kernelSize)
	: MatProcessor(name), mKernelSize{ kernelSize }
{
}
void MatBlurProcessor::process(cv::Mat const & inputImage)
{
	cv::blur(inputImage, mImage, cv::Size(mKernelSize, mKernelSize), cv::Point(-1, -1));
}

QWidget* MatBlurProcessor::createControlInterface()
{
	QGroupBox * widget{ new QGroupBox };
	widget->setTitle("Kernel size");
	QHBoxLayout * layout{ new QHBoxLayout };
	widget->setLayout(layout);

	QScrollBar * kernelSizeValue{ new QScrollBar };
	kernelSizeValue->setOrientation(Qt::Horizontal);
	kernelSizeValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	kernelSizeValue->setRange(1, 15);
	kernelSizeValue->setValue((mKernelSize - 1) / 2);
	QLabel * kernelSizeText{ new QLabel(QString::number(mKernelSize)) };
	kernelSizeText->setAlignment(Qt::AlignCenter);

	layout->addWidget(kernelSizeValue);
	layout->addWidget(kernelSizeText);
	

	QObject::connect(kernelSizeValue, &QScrollBar::valueChanged, [this, kernelSizeText](int value)->void { size_t kernelSize{ static_cast<size_t>(value) * 2 + 1 }; this->setKernelSize(kernelSize); kernelSizeText->setText(QString::number(kernelSize)); });

	return widget;
}


void MatRGBToHSVProcessor::process(cv::Mat const & inputImage)
{
	cv::cvtColor(inputImage, mImage, CV_BGR2HSV);
}





#include "QLowHighScrollBar.h"

Mat3CThresholdProcessor::Mat3CThresholdProcessor(std::string const & name, uint8_t minR, uint8_t maxR, uint8_t minG, uint8_t maxG, uint8_t minB, uint8_t maxB)
	: MatProcessor(name), mMinValues(minR, minG, minB), mMaxValues(maxR, maxG, maxB)
{
}
void Mat3CThresholdProcessor::set(uint8_t minC0, uint8_t maxC0, uint8_t minC1, uint8_t maxC1, uint8_t minC2, uint8_t maxC2)
{
	mMinValues[0] = minC0;
	mMinValues[1] = minC1;
	mMinValues[2] = minC2;
	mMaxValues[0] = maxC0;
	mMaxValues[1] = maxC1;
	mMaxValues[2] = maxC2;
}
void Mat3CThresholdProcessor::process(cv::Mat const & inputImage)
{
	cv::inRange(inputImage, mMinValues, mMaxValues, mImage);
}

QWidget* Mat3CThresholdProcessor::createControlInterface()
{
	QGroupBox * widget{ new QGroupBox };
	widget->setTitle("3 channels threshold ranges");
	QVBoxLayout * layout{ new QVBoxLayout };
	widget->setLayout(layout);

	QLowHighScrollBar * redRange{ new QLowHighScrollBar("Channel 1") };
	QLowHighScrollBar * greenRange{ new QLowHighScrollBar("Channel 2") };
	QLowHighScrollBar * blueRange{ new QLowHighScrollBar("Channel 3") };

	redRange->setRange(0, 255);
	greenRange->setRange(0, 255);
	blueRange->setRange(0, 255);

	redRange->set(mMinValues[0], mMaxValues[0]);
	greenRange->set(mMinValues[1], mMaxValues[1]);
	blueRange->set(mMinValues[2], mMaxValues[2]);

	layout->addWidget(redRange);
	layout->addWidget(greenRange);
	layout->addWidget(blueRange);
	layout->addStretch();

	QObject::connect(redRange, &QLowHighScrollBar::valueUpdated, [this, redRange]()->void { this->setC0(redRange->lowValue(), redRange->highValue()); });
	QObject::connect(greenRange, &QLowHighScrollBar::valueUpdated, [this, greenRange]()->void { this->setC1(greenRange->lowValue(), greenRange->highValue()); });
	QObject::connect(blueRange, &QLowHighScrollBar::valueUpdated, [this, blueRange]()->void { this->setC2(blueRange->lowValue(), blueRange->highValue()); });

	return widget;
}

MatBlobFind::MatBlobFind(std::string const & name, int cameraDeviceIndex)
	: MatProcessor(name)
{
	params.filterByArea = true;
	params.minArea = 50;
	params.maxArea = 18000;
	params.filterByColor = true;
	params.blobColor = 255;
	params.minConvexity = 0.5;

	detector = cv::SimpleBlobDetector::create(params);

}

void MatBlobFind::process(cv::Mat const & inputImage)
{
	//cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size2i(11, 11));
	//morphologyEx(inputImage, mImage, cv::MORPH_OPEN, kernel);
	inputImage.copyTo(mImage);
	detector->detect(mImage, keypoints);
	drawKeypoints(mImage, keypoints, mImage, cv::Scalar(255, 128, 128), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	
	if (keypoints.size() > 1) {
		x1 = keypoints.at(0).pt.x;
		x2 = keypoints.at(1).pt.x;
		y1 = keypoints.at(0).pt.y;
		y2 = keypoints.at(1).pt.y;
		mX = (x1 + x2) / 2;
		mY = (y1 + y2) / 2;
		mZ = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
		putText(mImage,
			"+",
			cv::Point(mX, mY), // Coordinates
			cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
			1.0, // Scale. 2.0 = 2x bigger
			cv::Scalar(255, 255, 255), // Color
			1, // Thickness
			CV_AA); // Anti-alias
		circle(mImage, cv::Point(10, 10), 10, cv::Scalar(0, 255, 0), -1);
	} else {
		circle(mImage, cv::Point(10, 10), 10, cv::Scalar(0, 0, 255), -1);
	}

}

int MatBlobFind::getDistance()
{
	return std::max(x1, x2) - std::min(x1, x2);
}

bool MatBlobFind::isTracked()
{
	if (keypoints.size() == 2) {
		tracked = true;
	}
	else {
		tracked = false;
	}
	return tracked;
}

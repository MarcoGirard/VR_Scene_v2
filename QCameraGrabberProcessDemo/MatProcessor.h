#ifndef MAT_PROCESSOR_H
#define MAT_PROCESSOR_H


#include "opencv2/opencv.hpp"

#include <QWidget>

class MatProcessor
{
public:
	MatProcessor(std::string const & name = "") : mName{ name } {}
	virtual ~MatProcessor() {}

	
	std::string const & name() const { return mName; }
	cv::Mat const & image() const { return mImage; }

	virtual void process(cv::Mat const & inputImage) = 0;

	QWidget* controlInterface() { return packageInterface(createControlInterface()); }

protected:
	std::string mName;
	cv::Mat mImage;

	virtual QWidget* createControlInterface();
	QWidget * packageInterface(QWidget * widget);
};



class MatGrabProcessor : public MatProcessor
{
public:
	MatGrabProcessor(std::string const & name, int cameraDeviceIndex = 0);

	virtual void process(cv::Mat const & inputImage) override;

protected:
	int mCameraDeviceIndex;
	cv::VideoCapture mVideoCaptureDevice;
};



class MatFlipProcessor : public MatProcessor
{
public:
	enum class FlipSide { Horizontally = 1, Vertically = 0, Both = -1 };
	MatFlipProcessor(std::string const & name = "", FlipSide flipSide = FlipSide::Horizontally);

	void setFlipSide(FlipSide flipSide) { mFlipCode = static_cast<int>(flipSide); }
	FlipSide flipSide() const { return static_cast<FlipSide>(mFlipCode); }

	virtual void process(cv::Mat const & inputImage) override;

	virtual QWidget* createControlInterface() override;

protected:
	int mFlipCode;

};

class MatBlurProcessor : public MatProcessor
{
public:
	MatBlurProcessor(std::string const & name = "", size_t kernelSize = 3);

	size_t kernelSize() const { return mKernelSize; }
	void setKernelSize(size_t kernelSize) { if (kernelSize > 1) mKernelSize = kernelSize; }

	virtual void process(cv::Mat const & inputImage) override;

	virtual QWidget* createControlInterface() override;

protected:
	size_t mKernelSize;
};

class MatRGBToHSVProcessor : public MatProcessor
{
public:
	MatRGBToHSVProcessor(std::string const & name = "") : MatProcessor(name) {}

	virtual void process(cv::Mat const & inputImage) override;
};

class Mat3CThresholdProcessor : public MatProcessor
{
public:
	Mat3CThresholdProcessor(std::string const & name = "", uint8_t minC0 = 64, uint8_t maxC0 = 88, uint8_t minC1 = 64, uint8_t maxC1 = 159, uint8_t minC2 = 64, uint8_t maxC2 = 138);

	uint8_t minC0() const { return mMinValues[0]; }
	uint8_t minC1() const { return mMinValues[1]; }
	uint8_t minC2() const { return mMinValues[2]; }
	uint8_t maxC0() const { return mMaxValues[0]; }
	uint8_t maxC1() const { return mMaxValues[1]; }
	uint8_t maxC2() const { return mMaxValues[2]; }
	void setMinC0(uint8_t value) { mMinValues[0] = value; }
	void setMinC1(uint8_t value) { mMinValues[1] = value; }
	void setMinC2(uint8_t value) { mMinValues[2] = value; }
	void setMaxC0(uint8_t value) { mMaxValues[0] = value; }
	void setMaxC1(uint8_t value) { mMaxValues[1] = value; }
	void setMaxC2(uint8_t value) { mMaxValues[2] = value; }
	void set(uint8_t minR, uint8_t maxR, uint8_t minG, uint8_t maxG, uint8_t minB, uint8_t maxB);
	void setC0(uint8_t min, uint8_t max) { mMinValues[0] = min; mMaxValues[0] = max; }
	void setC1(uint8_t min, uint8_t max) { mMinValues[1] = min; mMaxValues[1] = max; }
	void setC2(uint8_t min, uint8_t max) { mMinValues[2] = min; mMaxValues[2] = max; }

	virtual void process(cv::Mat const & inputImage) override;

	virtual QWidget* createControlInterface() override;

protected:
	cv::Scalar mMinValues, mMaxValues;
};

class MatBlobFind : public MatProcessor
{
public:
	MatBlobFind(std::string const & name, int cameraDeviceIndex = 0);

	virtual void process(cv::Mat const & inputImage) override;
	int x() { return mX; }
	int y() { return mY; }
	int z() { return mZ; }
	int getX1() { return x1; }
	int getX2() { return x2; }
	int getY1() { return y1; }
	int getY2() { return y2; }
	int getDistance();
	bool isTracked();


protected:
	std::vector<cv::KeyPoint> keypoints;
	bool tracked{ false };
	int mX{ 0 }, mY{ 0 }, mZ{ 0 };
	int x1{ 0 }, y1{ 0 }, x2{ 0 }, y2{ 0 };
	
	cv::SimpleBlobDetector::Params params;
	cv::Ptr<cv::SimpleBlobDetector> detector;
	
};



#endif // MAT_PROCESSOR_H


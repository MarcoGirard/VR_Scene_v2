#include "MatProcessorManager.h"




MatProcessorManager::~MatProcessorManager()
{
	for (auto processor : mProcessors) {
		delete processor;
	}
}

void MatProcessorManager::appendProcessor(MatProcessor * processor)
{
	for (auto & insertedProcessor : mProcessors) {
		if (insertedProcessor->name() == processor->name()) {
			throw std::invalid_argument("MatProcessorManager::appendProcessor processor argument must have a unique name. Name is already used.");
		}
	}

	mProcessors.push_back(processor);
	mMapProcessorIndex[processor->name()] = mProcessors.size() - 1;
}

void MatProcessorManager::process(cv::Mat const & inputImage)
{
	if (processorCount() > 0) {
		mProcessors[0]->process(inputImage);
		for (int i{ 1 }; i < mProcessors.size(); ++i) {
			mProcessors[i]->process(mProcessors[i - 1]->image());
		}
	}
}



ViewerPosition MatProcessorManager::viewerPosition()
{
	int x{ static_cast<MatBlobFind*>(mProcessors.at(mMapProcessorIndex["Blob"]))->x() };
	int y{ static_cast<MatBlobFind*>(mProcessors.at(mMapProcessorIndex["Blob"]))->y() };
	int z{ static_cast<MatBlobFind*>(mProcessors.at(mMapProcessorIndex["Blob"]))->z() };
	return ViewerPosition(x, y, z);
	//return ViewerPosition(static_cast<MatBlobFind*>(mProcessors.at(mMapProcessorIndex["Blob"]))->x(),
	//	                  static_cast<MatBlobFind*>(mProcessors.at(mMapProcessorIndex["Blob"]))->y(),
	//				      static_cast<MatBlobFind*>(mProcessors.at(mMapProcessorIndex["Blob"]))->z());
}



#ifndef MAT_PROCESSOR_MANAGER_H
#define MAT_PROCESSOR_MANAGER_H


#include <string>
#include <vector>
#include <map>

#include "MatProcessor.h"
#include "ViewerPosition.h"


class MatProcessorManager
{
public:
	virtual ~MatProcessorManager();

	void appendProcessor(MatProcessor * processor);

	std::string const & name(int index) const { return mProcessors[index]->name(); }

	cv::Mat const & image(int index) const { return mProcessors[index]->image(); }
	cv::Mat const & image(std::string const & name) const { return image(mMapProcessorIndex.at(name)); }
	cv::Mat const & result() const { return image(mProcessors.size() - 1); }
	
	QWidget * controlInterface(int index) { return mProcessors[index]->controlInterface(); }
	QWidget * controlInterface(std::string const & name) { return controlInterface(mMapProcessorIndex.at(name)); }

	int processorCount() const { return mProcessors.size(); }

	void process(cv::Mat const & inputImage = cv::Mat());
	ViewerPosition viewerPosition();

protected:
	std::vector<MatProcessor*> mProcessors;
	std::map<std::string, int> mMapProcessorIndex;
};


#endif // MAT_PROCESSOR_MANAGER_H




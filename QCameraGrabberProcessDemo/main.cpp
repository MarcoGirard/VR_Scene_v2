#include "QCameraGrabberProcessDemo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCameraGrabberProcessDemo w;
	w.show();
	return a.exec();
}

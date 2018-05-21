#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

struct CubePosition
{
	QVector3D center;
	QVector3D scale;
};

class QGeometryEngine : protected QOpenGLFunctions
{
public:
	QGeometryEngine();
	virtual ~QGeometryEngine();

	void addCube(QVector3D center = QVector3D(0.0, 0.0, 0.0), QVector3D scale = QVector3D(1.0,1.0,1.0));
	void drawCubeGeometry(QOpenGLShaderProgram *program);
	std::vector<CubePosition> cubePositions() { return mCubePositions; }

private:
	void initCubeGeometry();
	std::vector<CubePosition> mCubePositions;
	QOpenGLBuffer arrayBuf;
	QOpenGLBuffer indexBuf;
};

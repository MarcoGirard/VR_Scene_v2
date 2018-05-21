#pragma once

#include "QGeometryEngine.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include "ViewerPosition.h"

class VR_3DCanvas : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	explicit VR_3DCanvas(QWidget * parent = 0);
	~VR_3DCanvas();

	void setViewerPosition(ViewerPosition const & p);

protected:
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	void timerEvent(QTimerEvent *e) override;
	double mX{ 0 }, mY{ 0 }, mZ{ 0 };

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;


	void initShaders();
	void initTextures();

private:
	QBasicTimer timer;
	QOpenGLShaderProgram program;
	QGeometryEngine *geometries;

	QOpenGLTexture *texture;

	QMatrix4x4 projection;

	QVector2D mousePressPosition;
	QVector3D rotationAxis;
	qreal angularSpeed;
	QQuaternion rotation;
};


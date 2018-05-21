#include "VR_3DCanvas.h"

/* Pour ce code, et la classe GeometryEngine, je suis parti de l'exemple du Cube OpenGL de Qt Creator */


VR_3DCanvas::VR_3DCanvas(QWidget * parent) :
	QOpenGLWidget(parent),
	geometries(0),
	texture(0),
	angularSpeed(0)
{
}


VR_3DCanvas::~VR_3DCanvas()
{
	makeCurrent();
	delete texture;
	delete geometries;
	doneCurrent();
}


void VR_3DCanvas::mousePressEvent(QMouseEvent * e)
{
	mousePressPosition = QVector2D(e->localPos());
}

void VR_3DCanvas::mouseReleaseEvent(QMouseEvent * e)
{
	// Mouse release position - mouse press position
	QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

	// Rotation axis is perpendicular to the mouse position difference
	// vector
	QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

	// Accelerate angular speed relative to the length of the mouse sweep
	qreal acc = diff.length() / 100.0;

	// Calculate new rotation axis as weighted sum
	rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

	// Increase angular speed
	angularSpeed += acc;
}

void VR_3DCanvas::timerEvent(QTimerEvent * e)
{
	// Decrease angular speed (friction)
	angularSpeed *= 0.99;

	// Stop rotation when speed goes below threshold
	if (angularSpeed < 0.01) {
		angularSpeed = 0.0;
	}
	else {
		// Update rotation
		rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

		// Request an update
		update();
	}
}

void VR_3DCanvas::setViewerPosition(ViewerPosition const & p)
{
	qDebug() << "input : " << p.x << " " << p.y << " " << p.z;
	//mZ = 0.010048*pow(p.z, 2) - 3.538225 * p.z + 378.683356;
	mZ = 12278.894503 * pow(p.z, -1.004948) + 0;
	mY = (-0.001503 * mZ - 0.001758)*p.y + 0.39765 * mZ + 97.403028;
	mX = (-0.001467 * mZ - 0.013139)  * p.x + 0.479079 * mZ + 3.572861;
	update();
}

void VR_3DCanvas::initializeGL()
{
	initializeOpenGLFunctions();

	glClearColor(0.2, 0.3, 0.3, 1);

	initShaders();
	initTextures();

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	// Enable back face culling
	glEnable(GL_CULL_FACE);

	geometries = new QGeometryEngine;
	timer.start(12, this);
}

void VR_3DCanvas::resizeGL(int w, int h)
{
	// Calculate aspect ratio
	qreal aspect{ qreal(w) / qreal(h ? h : 1) };

	// Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
	const qreal zNear{ 5.0 }, zFar{ 2500.0 }, fov{ 30.0 };

	// Reset projection
	projection.setToIdentity();

	// Set perspective projection
	//projection.perspective(fov, aspect, zNear, zFar);
	projection.ortho(-34.6 / 2.0, 34.6 / 2.0, -19.5 / 2.0, 19.5 / 2.0, zNear, zFar);
}

void VR_3DCanvas::paintGL()
{
	// Clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	texture->bind();
	// Calculate model view transformation
	QMatrix4x4 matrix;
	QVector3D positionReelle = QVector3D(mX, mY-98.0, -35.0);
	qDebug() << "real : " << positionReelle.x() << " " << positionReelle.y() << " " << positionReelle.z();
	//pos.normalize();
	//qDebug() << "normalized : " << pos.x() << " " << pos.y() << " " << pos.z();
	/*pos.setX(pos.x() * 4);
	pos.setY(pos.y() * -400 - 370);
	pos.setZ(pos.z() * 9 - 12);*/
	//qDebug() << "multiplied : " << pos.x() << " " << pos.y() << " " << pos.z();
	//matrix.translate(pos);
	//matrix.translate(0.0,0.0,-5.0);
	//matrix.rotate(rotation);
	matrix.lookAt(positionReelle, QVector3D(0.0, 0.0, 5.0), QVector3D(0.0, 1.0, 0.0));

	// Set modelview-projection matrix
	// Use texture unit 0 which contains cube.png
	program.setUniformValue("texture", 0);

	// Draw cube geometry
	QMatrix4x4 matrixRef(matrix);

	for (auto cube : geometries->cubePositions()) {
		matrix = matrixRef;
		matrix.translate(cube.center);
		matrix.scale(cube.scale);
		program.setUniformValue("mvp_matrix", projection * matrix);
		geometries->drawCubeGeometry(&program);
	}
}

void VR_3DCanvas::initShaders()
{
	// Compile vertex shader
	if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "vshader.glsl"))
		close();

	// Compile fragment shader
	if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "fshader.glsl"))
		close();

	// Link shader pipeline
	if (!program.link())
		close();

	// Bind shader pipeline for use
	if (!program.bind())
		close();
}

void VR_3DCanvas::initTextures()
{
	// Load cube.png image
	texture = new QOpenGLTexture(QImage("woodBox.png").mirrored());

	// Set nearest filtering mode for texture minification
	texture->setMinificationFilter(QOpenGLTexture::Nearest);

	// Set bilinear filtering mode for texture magnification
	texture->setMagnificationFilter(QOpenGLTexture::Linear);

	// Wrap texture coordinates by repeating
	// f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
	texture->setWrapMode(QOpenGLTexture::Repeat);
}

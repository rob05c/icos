#include <QtWidgets>
#include "myglwidget.h"

namespace
{
/// lets us glEnable/glDisable in one line.
inline void parameterizedGlEnable(const GLenum value, const bool enable) {
    if(enable)
        glEnable(value);
    else
        glDisable(value);
}

void drawtriangle(float *v1, float *v2, float *v3)
{
  {
    glBegin(GL_POLYGON);
    glNormal3fv(v1);
    glVertex3fv(v1);

    glNormal3fv(v2);
    glVertex3fv(v2);

    glNormal3fv(v3);
    glVertex3fv(v3);
    glEnd();
   }
}

void normalize(float v[3]) {
   GLfloat d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
   if (d == 0.0) {
//      error("zero length vector");
      return;
   }
   v[0] /= d; v[1] /= d; v[2] /= d;
}

void normcrossprod(float v1[3], float v2[3], float out[3])
{
   //GLint i;
   //GLint j;
   //GLfloat length;

   out[0] = v1[1]*v2[2] - v1[2]*v2[1];
   out[1] = v1[2]*v2[0] - v1[0]*v2[2];
   out[2] = v1[0]*v2[1] - v1[1]*v2[0];
   normalize(out);
}

void subdivide(float *v1, float *v2, float *v3, long depth) {
   GLfloat v12[3], v23[3], v31[3];
   GLint i;

   if (depth == 0) {
      drawtriangle(v1, v2, v3);
       return;
   }
   for (i = 0; i < 3; i++) {
      v12[i] = (v1[i]+v2[i])/2;
      v23[i] = (v2[i]+v3[i])/2;
      v31[i] = (v3[i]+v1[i])/2;
   }
   normalize(v12);
   normalize(v23);
   normalize(v31);
   subdivide(v1, v12, v31, depth-1);
   subdivide(v2, v23, v12, depth-1);
   subdivide(v3, v31, v23, depth-1);
   subdivide(v12, v23, v31, depth-1);
}

/// \param accuracy sphere approximation accuracy.
void drawSphere(const size_t accuracy) {
    #define X .525731112119133606
    #define Z .850650808352039932

    static GLfloat vdata[12][3] = {
           {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
           {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
           {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
        };

    static GLint tindices[20][3] = {
           {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
           {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
           {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
           {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
        };

    for(int i = 0; i < 20; i++) {
       subdivide(&vdata[tindices[i][0]][0],
                &vdata[tindices[i][1]][0],
                 &vdata[tindices[i][2]][0], accuracy);
    }
}
}

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(parent)
    , xRot(0)
    , yRot(0)
    , zRot(0)
    , accuracy(0)
    , whiteLight(true)
    , yellowLight(false)
    , smoothShading(false)
{
}

MyGLWidget::~MyGLWidget()
{
}

QSize MyGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    angle %= 360;
}

void MyGLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle == xRot)
        return;
    xRot = angle;
    emit xRotationChanged(angle);
    updateGL();
}

void MyGLWidget::setAccuracy(int accuracy_) {
    accuracy = accuracy_;
    updateGL();
}

void MyGLWidget::setWhiteLight(bool on) {
    whiteLight = on;
    updateGL();
}

void MyGLWidget::setYellowLight(bool on) {
    yellowLight = on;
    updateGL();
}

/// \param on smooth shading on or off. This is an int, so it can be linked with a combo box change event
void MyGLWidget::setSmoothShading(int on) {
    smoothShading = (bool)on;
    updateGL();
}

void MyGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle == yRot)
        return;
    yRot = angle;
    emit yRotationChanged(angle);
    updateGL();
}

void MyGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle == zRot)
        return;
    zRot = angle;
    emit zRotationChanged(angle);
    updateGL();
}

void MyGLWidget::initializeGL()
{
    qglClearColor(Qt::black);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    static GLfloat whiteLightPosition[4]   = { 10, 10, -10, 1.0 };
    static GLfloat yellowLightPosition[4]  = { -10, 5, -10, 1.0 };
    static GLfloat dimWhite[4]             = { 0.5, 0.5, 0.5, 1.0 };
    static GLfloat yellow[4]               = { 1.0, 1.0, 0.0, 1.0 };
    //static GLfloat white[4]                = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, whiteLightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, dimWhite);

    glLightfv(GL_LIGHT1, GL_POSITION, yellowLightPosition);
    glLightfv(GL_LIGHT1, GL_AMBIENT, dimWhite);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, yellow);
    glLightfv(GL_LIGHT1, GL_SPECULAR, yellow);
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot, 1.0, 0.0, 0.0);
    glRotatef(yRot, 0.0, 1.0, 0.0);
    glRotatef(zRot, 0.0, 0.0, 1.0);
    draw();
}

void MyGLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + dy);
        setYRotation(yRot + dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + dy);
        setZRotation(zRot + dx);
    }

    lastPos = event->pos();
}

void MyGLWidget::draw()
{
    parameterizedGlEnable(GL_LIGHT0, whiteLight);
    parameterizedGlEnable(GL_LIGHT1, yellowLight);
    glShadeModel(smoothShading ? GL_SMOOTH : GL_FLAT);
    drawSphere(accuracy);
}

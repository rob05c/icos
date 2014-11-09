#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();
signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
public slots:
    void setAccuracy(int accuracy);
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setWhiteLight(bool on);
    void setYellowLight(bool on);
    void setSmoothShading(int on);
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);

private:
    void draw();

    int    xRot;
    int    yRot;
    int    zRot;
    int    accuracy;
    bool   whiteLight;
    bool   yellowLight;
    bool   smoothShading;
    QPoint lastPos;
};

#endif // MYGLWIDGET_H

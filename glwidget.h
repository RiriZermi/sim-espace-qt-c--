#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

#include "simulation.h"
#include "renderer.h"

using namespace std;
class QKeyEvent;
class QWheelEvent;
class QMouseEvent;
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);
    ~GLWidget();
    void setSimu(Simulation*);
    void tick();
    Renderer* getRenderer(){return render_;}
    Simulation* getSimulation(){return simu_;}

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    //void mouseMoveEvent(QMouseEvent *event)override;

private:

    Simulation* simu_;
    Renderer* render_;
    double dt = 0.016; //ms

signals:
    void simuIsSet();



    

};



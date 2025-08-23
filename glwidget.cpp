#include "glwidget.h"
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <cmath>
#include <vector>
#include <iostream>
#include <QTimer>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), 
      simu_(nullptr),
      render_(nullptr),
      lastMousePos(),
      mousePressed(false)
{   
    render_ = new Renderer();
    
    setFocusPolicy(Qt::StrongFocus);

    //Timer signal slot
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout,this, &GLWidget::tick);
    timer->start(int(dt*1000));

    //mousse tracking
    setMouseTracking(true);
}

GLWidget::~GLWidget()
{
    delete render_;
    delete simu_;
}

void GLWidget::initializeGL()
{   
    initializeOpenGLFunctions();
    render_->init();
}


void GLWidget::setSimu(Simulation* simu){
    simu_=simu;
    vector<Astre*> astres = simu->getAstres();
    double max = 100; //valeur initial minimal
    for(size_t i=0;i<astres.size();i++){
        if(abs(astres[i]->x)>max){
            max = abs(astres[i]->x);
        }
        if(abs(astres[i]->y)>max){
            max = abs(astres[i]->y);
        }
    }
    render_->setScaleFactor(1/(max));
    render_->setMax(max);
    // render_->init();


    emit simuIsSet();
    
}

void GLWidget::tick(){
    if (simu_ != nullptr){
        simu_->updateSim(dt);
        update();
        }
}

void GLWidget::resizeGL(int w, int h)
{   
    render_->setWidth(w);
    render_->setHeight(h);
    glViewport(0, 0, w, h);
}

void GLWidget::paintGL()
{  
    
    if (simu_ != nullptr) {
        vector<Astre*> astres = simu_->getAstres();
        render_->render(astres);
    }
   
}
void GLWidget::keyPressEvent(QKeyEvent *event){
    render_->keyPressEvent(event);
}

void GLWidget::wheelEvent(QWheelEvent *event){
    render_->wheelEvent(event);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mousePressed = true;
        lastMousePos = event->pos();
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mousePressed = false;
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (mousePressed) {
        QPoint delta = event->pos() - lastMousePos;
        float sensitivity = 0.01f;

        // yaw += delta.x() * sensitivity;
        // pitch += delta.y() * sensitivity;

        render_->addPitch(delta.x() * sensitivity);
        render_->addYaw(delta.y() * sensitivity);

        // // Clamp pitch to avoid flipping
        // pitch = std::clamp(pitch, -89.0f, 89.0f);

        lastMousePos = event->pos();
    }
}



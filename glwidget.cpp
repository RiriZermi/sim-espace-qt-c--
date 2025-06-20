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
      render_(nullptr)
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

// void GLWidget::mouseMoveEvent(QMouseEvent *event){
//     int width = this->width();
//     int height = this->height();
//     render_->mouseMoveEvent(event,width,height,this);

// }



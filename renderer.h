#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <QOpenGLShaderProgram>

#include "astres.h"

using namespace std;
class QWheelEvent;
class QMouseEvent;
class QKeyEvent;
class GLWidget;

class Renderer : protected QOpenGLFunctions_3_3_Core {
public:
    Renderer();
    ~Renderer();
    void init();                    // charger shaders, buffers
    //void resize(int w, int h);      // configurer projection
    void render(vector<Astre*>); // dessiner une sphère à la position de la planète
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *eventj,int,int,GLWidget*);
    void setScaleFactor(double);
    void followAstre(Astre*);
    void addPitch(float);
    void addYaw(float);
    void updateViewMatrix();
    void updateProjectionMatrix();
    void initShaders();
    void initAstresBuffers();
    void initGridVertex();
    void initGridBuffers();
    void initCircleVertex();
    void initSphereVertex();
    void updateGridVertex();
    void initMatrices();
    void setMax(float m){max = m;}
    void setWidth(int width){w = width;}
    void setHeight(int height){h=height;}

private:
    QOpenGLShaderProgram* shaderProgram;
    unsigned int vao, vbo, ebo;
    vector<float> vertices;
    vector<unsigned int> indices;
    unsigned int vaoGrid, vboGrid, eboGrid;
    vector<float> verticesGrid;
    vector<unsigned int> indicesGrid;

    glm::mat4 view,projection;
    int rgbLoc,modeLoc;
    int viewLoc,projectionLoc;
    double scaleFactor;
    float pitch,yaw;
    double camXPos,camYPos,camZPos;
    Astre* astreFollowed;
    float max;
    bool isInit;
    int w,h;
    float deg;
};


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

private:
    void initShaders();
    void initAstresVertex();
    void initCircleVertex();
    void initMatrices();
    QOpenGLShaderProgram* shaderProgram;
    unsigned int vao, vbo;
    vector<float> vertices;

    glm::mat4 view,projection;
    int viewLoc,projectionLoc;
    double scaleFactor;
    double camXPos,camYPos;
    Astre* astreFollowed;

};


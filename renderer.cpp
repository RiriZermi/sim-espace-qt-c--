#include <cmath>
#include <iostream>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QToolTip>
#include <QPoint>
#include "renderer.h"
#include "glwidget.h"
#include "astres.h"
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer()
    : shaderProgram(nullptr), vao(0), vbo(0), scaleFactor(1), camXPos(0), camYPos(0), astreFollowed(nullptr)
{

}

Renderer::~Renderer(){
    delete shaderProgram;
}

void Renderer::init() {
    initializeOpenGLFunctions();
    //glEnable(GL_DEPTH_TEST);

    initShaders();
    initAstresVertex();
    initMatrices();
}

void Renderer::initMatrices(){
    shaderProgram->bind();

    projectionLoc = shaderProgram->uniformLocation("projection");
    viewLoc = shaderProgram->uniformLocation("view");
    projection = glm::mat4(1.0f);
    projection = glm::scale(projection,glm::vec3(scaleFactor,scaleFactor,1));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

    view = glm::mat4(1.0f);
    view = glm::translate(view,glm::vec3(0,0,0));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

    shaderProgram->release();
}
void Renderer::initAstresVertex(){

    initCircleVertex();

    // VAO / VBO 
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Renderer::initShaders(){
    shaderProgram = new QOpenGLShaderProgram();
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
        R"(#version 330 core
        layout(location = 0) in vec2 position;
        uniform mat4 model;
        uniform mat4 projection;
        uniform mat4 view;
        void main() {
            gl_Position =   projection * view * model * vec4(position, 0.0, 1.0);
        })"
    );
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
        R"(#version 330 core
        out vec4 FragColor;
        uniform vec3 rgb;
        void main() {
            FragColor = vec4(rgb,1.0);
        })"
    );
    shaderProgram->link();
}

void Renderer::render(vector<Astre*> astres) {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vao);
    shaderProgram->bind();
    int modelLoc = shaderProgram->uniformLocation("model");

    if (astreFollowed != nullptr){
        camXPos = astreFollowed->x;
        camYPos = astreFollowed->y;
        view = glm::mat4(1.0f);
        view = glm::translate(view,glm::vec3(-camXPos,-camYPos,0.0));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    }

    for (size_t i =0;i<astres.size();i++){
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(astres[i]->x,astres[i]->y,0.0));
        model = glm::scale(model,glm::vec3(astres[i]->radius,astres[i]->radius,0.0)); 
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);


        int rgbLoc = shaderProgram -> uniformLocation("rgb");
        glUniform3f(rgbLoc,astres[i]->r,astres[i]->g,astres[i]->b);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size()/2);

    }
    glBindVertexArray(0);
    shaderProgram->release();
}

void Renderer::followAstre(Astre* astre){
    astreFollowed = astre;
}

void Renderer::initCircleVertex(){
    int points = 100;
    vertices.push_back(0.0);
    vertices.push_back(0.0);
    for (int i=0;i<points;i++){
        float angle = 2 * M_PI * i / (points-1);
        vertices.push_back(cos(angle)); //x
        vertices.push_back(sin(angle)); //y
    }
}

void Renderer::keyPressEvent(QKeyEvent *event){
    if (astreFollowed==nullptr){
        switch (event->key()) {
            case Qt::Key_Up:
                camYPos += (1/(scaleFactor*50));
                break;
            case Qt::Key_Down:
                camYPos -= (1/(scaleFactor*50));
                break;
            case Qt::Key_Left:
                camXPos -= (1/(scaleFactor*50));
                break;
            case Qt::Key_Right:
                camXPos += (1/(scaleFactor*50));
                break;
            default:
                
                break;
        }
        shaderProgram->bind();
        view = glm::mat4(1.0f);
        view = glm::translate(view,glm::vec3(-camXPos,-camYPos,1.0));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        shaderProgram->release();
    }

}

void Renderer::wheelEvent(QWheelEvent *event){
    int val = event->angleDelta().y();
    if (val>0){
        scaleFactor *=1.1;
        
    }else{
        scaleFactor /= 1.1;
    }
    shaderProgram->bind();
    projection = glm::mat4(1.0f);
    projection = glm::scale(projection,glm::vec3(scaleFactor,scaleFactor,1));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
    shaderProgram->release();
}

void Renderer::mouseMoveEvent(QMouseEvent *event, int width, int height,GLWidget* glwidget){

    float xScreen = event->pos().x();
    float yScreen = event->pos().y();
    QPoint point = QPoint(xScreen,yScreen);
    point = glwidget->mapToGlobal(point);

    float xNDC = 2.0f * (xScreen/width) -1;
    float yNDC = -2.0f * (yScreen/height) + 1;

    glm::mat4 inversedMat = glm::inverse(projection*view);

    glm::vec4 worldPos = inversedMat * glm::vec4(xNDC,yNDC,0,1);

    // Afficher une infobulle à la position de la souris (global)
    QToolTip::showText(point,
        QString("World: (%1 m, %2 m)")
            .arg((int)worldPos.x)
            .arg((int)worldPos.y),
         static_cast<QWidget*>(glwidget) // parent widget
    );


}

void Renderer::setScaleFactor(double s){

    scaleFactor = s;
}

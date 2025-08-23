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
    : shaderProgram(nullptr), 
    vao(0), 
    vbo(0), 
    ebo(0),
    vaoGrid(0),
    vboGrid(0),
    eboGrid(0),
    scaleFactor(1), 
    pitch(M_PI/2),
    yaw(0), 
    camXPos(0),
    camYPos(0),
    camZPos(1.0), 
    astreFollowed(nullptr),
    max(0),
    isInit(false)
{
    
}

Renderer::~Renderer(){
    delete shaderProgram;
}

void Renderer::init() {
    isInit = true;
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    initShaders();
    initAstresBuffers();
    initGridBuffers();
    initMatrices();
    
}

void Renderer::initMatrices(){
    shaderProgram->bind();

    projectionLoc = shaderProgram->uniformLocation("projection");
    viewLoc = shaderProgram->uniformLocation("view");
    projection = glm::mat4(1.0f);
    projection = glm::scale(projection,glm::vec3(scaleFactor,scaleFactor,scaleFactor));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

    view = glm::mat4(1.0f);
    view = glm::translate(view,glm::vec3(0,0,0));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    shaderProgram->release();

}
void Renderer::initAstresBuffers(){

    initSphereVertex();

    // VAO / VBO 
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1,&ebo);

    glBindVertexArray(vao);

    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Renderer::initGridBuffers(){
    if (!isInit) return;
    initGridVertex();
    

    // VAO / VBO 
    glGenVertexArrays(1, &vaoGrid);
    glGenBuffers(1, &vboGrid);
    glGenBuffers(1,&eboGrid);

    glBindVertexArray(vaoGrid);

    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboGrid);
    glBufferData(GL_ARRAY_BUFFER, verticesGrid.size() * sizeof(float), verticesGrid.data(), GL_DYNAMIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboGrid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesGrid.size() * sizeof(unsigned int), indicesGrid.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}
void Renderer::initShaders(){
    shaderProgram = new QOpenGLShaderProgram();
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
        R"(#version 330 core 
        layout(location = 0) in vec3 position;
        uniform bool mode;
        uniform mat4 model;
        uniform mat4 projection;
        uniform mat4 view;
        out vec3 fragPos;

        uniform vec3 astresPos[100];
        uniform float astresMass[100];
        uniform int astresNbr;
        
        const float c = 300000000.0;
        const float G = 6.6e-11;
        void main() {
            if (mode){
                fragPos=position;
                gl_Position =   projection * view * model * vec4(position, 1.0);
            } else {
                vec3 tmp = position;
                float max = 3*2.27e11;
                vec3 worldPos = max * position;
                // z transfo
                float totalDisplacement = 0.0;
                for(int i=0;i<astresNbr;i++){
                    vec3 diff = astresPos[i] - worldPos;
                    float distance = length(diff);
                    float dz = G*astresMass[i]/(distance+1e10);
                    totalDisplacement += dz;   
                }
                tmp.z = totalDisplacement/(5e11);
                if (tmp.z>0.99){
                tmp.z = 0.99;
                } 
                //
                gl_Position = projection * view * model * vec4(tmp,1.0);
            }   
        })"
    );
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
        R"(#version 330 core
        out vec4 FragColor;
        uniform vec3 rgb;
        uniform bool mode;
        in vec3 fragPos;
        void main() {
            if (mode){
                float factor = ((fragPos.x*fragPos.x + fragPos.y*fragPos.y)+2)/4 ;
                vec3 color = rgb * factor;
                FragColor = vec4(color, 1.0);
            } else {
                FragColor = vec4(rgb,1.0);
            }
        })"
    );
    shaderProgram->link();

    modeLoc = shaderProgram->uniformLocation("mode");
    rgbLoc = shaderProgram->uniformLocation("rgb");



}

void Renderer::render(vector<Astre*> astres) {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // glClear(GL_COLOR_BUFFER_BIT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram->bind();
    //--------------Astres--------------------------//
    glBindVertexArray(vao);
    int modelLoc = shaderProgram->uniformLocation("model");

    if (astreFollowed != nullptr){
        camXPos = astreFollowed->x;
        camYPos = astreFollowed->y;
        view = glm::mat4(-1.0f);
        view = glm::translate(view,glm::vec3(-camXPos,-camYPos,0.0));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    }

    for (size_t i =0;i<astres.size();i++){
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(astres[i]->x,astres[i]->y,0.0));
        model = glm::scale(model,glm::vec3(astres[i]->radius,astres[i]->radius,astres[i]->radius)); 
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

        glUniform3f(rgbLoc,astres[i]->r,astres[i]->g,astres[i]->b);
        glUniform1i(modeLoc,1);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    }

    //---------------Grid---------------------//
    glBindVertexArray(vaoGrid);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model,glm::vec3(3*max));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    //
    int astresPosLoc = shaderProgram->uniformLocation("astresPos");
    int astresMassLoc = shaderProgram->uniformLocation("astresMass");
    int astresNbrLoc = shaderProgram->uniformLocation("astresNbr");
    vector<glm::vec3> astresPos(100);
    int astresNbr = astres.size();

    for (int i=0;i<astresNbr;i++){
        astresPos[i]=glm::vec3(astres[i]->x,astres[i]->y,0.0);
    }
    vector<float> astresMass(100);
    for (int i=0;i<astresNbr;i++){
        astresMass[i] = astres[i]->m;
    }
    glUniform3fv(astresPosLoc, astresNbr, &astresPos[0][0]);
    glUniform1fv(astresMassLoc, astresNbr, &astresMass[0]);
    glUniform1i(astresNbrLoc, astresNbr);
    
    glUniform3f(rgbLoc,0.5,0.5,0.5);
    glUniform1i(modeLoc,0);
    glDrawElements(GL_LINES, indicesGrid.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    shaderProgram->release();
    // cout << "-------------------TOTAL DISPLACEMENT----------------" << endl;
    // float maxVal = 0.0;
    // for(int j=0;j<indicesGrid.size();j+=3){
    //     glm::vec3 position = glm::vec3(verticesGrid[j],verticesGrid[j+1],verticesGrid[j+2]);
    //     // cout << "position : "<<position[0] <<" "<<position[1] <<" "<<position[2] << "\n";
    //     position = position * max;
    //     float totalDisplacement=0;
    //     float const c = 300'000'000;
    //     float const G = 6.6* pow(10,-11);
    //     float spread = 1e11;
    //     for(int i=0;i<astresNbr;i++){
    //         glm::vec3 diff = astresPos[i] - position;
    //         float distance = length(diff);
    //         float dz = G*astresMass[i]/distance;
    //         totalDisplacement += dz;  
    //     }
    //     if (totalDisplacement > maxVal) maxVal=totalDisplacement;
    //     // cout << totalDisplacement << "\n";
    // }
    // cout<<maxVal<<endl;
    // cout << "GRID VERTEX \n";
    // for (int i=0;i<verticesGrid.size();i=i+3){
    //     glm::vec4 vector= projection*view*model* glm::vec4(verticesGrid[i],verticesGrid[i+1],-0.5,1);
    //     cout << vector[0] << " " << vector[1] << " "<< vector[2] << endl;
    // }
    // cout << "WORD 000 " <<endl;
    // glm::vec4 vector= projection*view *model* glm::vec4(0,0,0,1);
    // cout << vector[0] << " " << vector[1] << " "<< vector[2] << endl;
    // cout << "-----MODEL--------\n";
    // for(int i=0;i<4;i++){
    //     cout << "\n";
    //     for(int j=0;j<4;j++){
    //         cout << model[j][i] << "  ";
    //     }
    // }
    // cout <<"----------------------\n"<<endl;

    // cout << "-----VIEW--------\n";
    // for(int i=0;i<4;i++){
    //     for(int j=0;j<4;j++){
    //         cout << view[j][i] << "  ";
    //     }
    //     cout << "\n";
    // }
    // cout <<"----------------------\n"<<endl;
    // cout <<"yaw " << yaw << endl;
    // cout << "pitch " << pitch <<endl;
    // cout << "-----Projeciton--------\n";
    // for(int i=0;i<4;i++){
    //     for(int j=0;j<4;j++){
    //         cout << projection[j][i] << "  ";
    //     }
    //     cout << "\n";
    // }
    // cout <<"----------------------"<<endl;

    // cout << "-----PRODUIT--------\n";
    // for(int i=0;i<4;i++){
    //     for(int j=0;j<4;j++){
    //         cout << (projection*view*model)[j][i] << "  ";
    //     }
    //     cout << "\n";
    // }
    // cout <<"----------------------\n"<<endl;
    
    // cout << "-----POINTG--------\n";
    // for(int i=0;i<4;i++){
    //     for(int j=0;j<4;j++){
    //         cout << (projection*view*model)[j][i]*(glm::vec4(1.0,0.0,0.0,1.0)) << "  ";
    //     }
    //     cout << "\n";
    // }
    // cout <<"----------------------\n"<<endl;

}

void Renderer::followAstre(Astre* astre){
    astreFollowed = astre;
}

void Renderer::initCircleVertex(){
    int points = 5;
    vertices.push_back(0.0);
    vertices.push_back(0.0);
    vertices.push_back(0.0);
    for (int i=0;i<points;i++){
        float angle = 2 * M_PI * i / (points-1);
        vertices.push_back(cos(angle)); //x
        vertices.push_back(sin(angle)); //y
        vertices.push_back(0.0);
    }
}
void Renderer::initSphereVertex(){
    //---------Vertices------------
    int n=10;
    float phi; //latitude angle
    float theta; // longitude angle
    vertices.push_back(0);
    vertices.push_back(0); 
    vertices.push_back(-1);
    for(int i=1;i<n-1;i++){
        for(int j=0;j<n;j++){
            phi = (M_PI/n) * i - (M_PI/2);
            theta = (2*M_PI/(n-1)) * j;
            vertices.push_back(cos(theta)* cos(phi));
            vertices.push_back(sin(theta)* cos(phi)); 
            vertices.push_back(sin(phi));
        }
    }
    vertices.push_back(0);
    vertices.push_back(0); 
    vertices.push_back(1);

    //---------Indices-----------

    for(int k=0;k<n-1;k++){
        indices.push_back(0);
        indices.push_back(k+1);
        indices.push_back(k+2);

        indices.push_back(n*(n-2)+1);
        indices.push_back(n*(n-3)+1+k);
        indices.push_back(n*(n-3)+1+k+1);
    }
    
    unsigned int begL1,begL2;
    for(int l=1;l<n-1;l++){
        begL1 = 1 + n*(l-1);
        begL2 = (n+1) + n*(l-1);
        for(int k=0;k<n-1;k++){
            indices.push_back(begL1+k);
            indices.push_back(begL1+k+1);
            indices.push_back(begL2+k);

            indices.push_back(begL1+k+1);
            indices.push_back(begL2+k);
            indices.push_back(begL2+k+1);

        }
    }

 

}


void Renderer::initGridVertex(){
    //------Vertex---------------
    // float max2 = 1.0;
    // float spacing = (2*max2)/15;
    // unsigned int points_per_side = static_cast<unsigned int>((2 * max2) / spacing) + 1;
    unsigned int points_per_side = 75;
    float spacing = 2.0 / (points_per_side-1);
    float x,y;
    for(unsigned int i=0;i<points_per_side;i++){
        for(unsigned int j=0;j<points_per_side;j++){
            x = -1 + spacing * i;
            y = -1 + spacing * j;
            verticesGrid.push_back(x);
            verticesGrid.push_back(y);
            verticesGrid.push_back(0.0);
        }
    }

    //------Indice----------
    unsigned int N = points_per_side;

    // Lignes horizontales
    for (unsigned int i = 0; i < N; ++i) {
        for (unsigned int j = 0; j < N - 1; ++j) {
            unsigned int idx1 = i * N + j;
            unsigned int idx2 = i * N + (j + 1);
            indicesGrid.push_back(idx1);
            indicesGrid.push_back(idx2);
        }
    }


    // Lignes verticales
    for (unsigned int i = 0; i < N - 1; ++i) {
        for (unsigned int j = 0; j < N; ++j) {
            unsigned int idx1 = i * N + j;
            unsigned int idx2 = (i + 1) * N + j;
            indicesGrid.push_back(idx1);
            indicesGrid.push_back(idx2);
        }
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
        updateViewMatrix();
    }

}

void Renderer::wheelEvent(QWheelEvent *event){
    int val = event->angleDelta().y();
    if (val>0){
        scaleFactor *=1.1;
        
    }else{
        scaleFactor /= 1.1;
    }

    updateProjectionMatrix();
    updateViewMatrix();
}


void Renderer::setScaleFactor(double s){

    scaleFactor = s;
}

void Renderer::addPitch(float delta){
    pitch += delta;
    pitch = fmod(pitch,2*M_PI);
    updateProjectionMatrix();
    updateViewMatrix();
}

void Renderer::addYaw(float delta){
    yaw += delta;
    yaw = fmod(yaw,2*M_PI);
    updateProjectionMatrix();
    updateViewMatrix();
}

void Renderer::updateProjectionMatrix(){
    // shaderProgram->bind();
    // projection = glm::mat4(1.0f);
    // projection = glm::scale(projection,glm::vec3(scaleFactor,scaleFactor,scaleFactor));
    // glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
    // shaderProgram->release();

    // float nearPlane = 10000.0f;
    // float farPlane = 100000000.0f;
    // shaderProgram->release();

    shaderProgram->bind();

    float fov = glm::radians(100.0f); // champ de vision vertical (en radians)
    float aspect = (float)w / (float)h; // ratio écran
    float d = (1/scaleFactor);
    float nearPlane = 0.1*d;      // proche de la caméra
    float farPlane  = 3*d; // très loin

    projection = glm::perspective(fov, aspect, nearPlane, farPlane);

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
    shaderProgram->release();
    

    
}

void Renderer::updateViewMatrix(){

    

    double r =  1/scaleFactor;
    // double r=1;
    shaderProgram->bind();

    glm::vec3 cameraPos;
    

    cameraPos.x = cos(yaw)* cos(pitch)*r;
    cameraPos.y = sin(yaw)* cos(pitch)*r; 
    cameraPos.z = sin(pitch) * r;

    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    view = glm::lookAt(cameraPos, cameraTarget, up);
    //view = glm::mat4(1.0f);
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

    shaderProgram->release();

    // cout << "-----VIEW--------\n";
    // for(int i=0;i<4;i++){
    //     for(int j=0;j<4;j++){
    //         cout << view[j][i] << "  ";
    //     }
    //     cout << "\n";
    // }
    
}






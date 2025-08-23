
#include <QApplication>
#include <QSurfaceFormat>
#include <iostream>
#include <cmath>
#include "mainwindow.h"
#include "glwidget.h"


int main(int argc, char *argv[])
{   QApplication app(argc, argv);
    MainWindow window; 
    //simu
    Simulation* simu = new Simulation();
    
    Astre Terre = Astre("Terre",149*pow(10,9),0.0,0.0,30000,5.972*pow(10,24),6*pow(10,6)*1000,0.0,0.0,1.0);
    Astre Soleil = Astre("Soleil",0,0,0,0,1.989*pow(10,30),696*pow(10,6),1.0,1.0,0.0);
    Astre Mars = Astre("Mars",227*pow(10,9),0.0,0.0,24000,6.4*pow(10,23),3*pow(10,6)*1000,1.0,0.4,0.1);
    Astre Lune = Astre("Lune",149*pow(10,9) + 384*pow(10,6) , 0.0 , 0.0, 30000 + 1000, 7.3*pow(10,22),2*pow(10,6),1.0,1.0,1.0);
    simu->addAstre(&Terre);
    simu->addAstre(&Soleil);
    simu->addAstre(&Mars);
    simu->addAstre(&Lune);
    Astre Soleil1 = Astre("Soleil",149e9,0,0,10000,1.989*pow(10,30)*5,696*pow(10,6)*10,1.0,1.0,0.0);
    Astre Soleil2 = Astre("Soleil",-149e9,0,0,-10000,1.989*pow(10,30)*5,696*pow(10,6)*10,1.0,1.0,0.0);
    // simu->addAstre(&Soleil1);
    // simu->addAstre(&Soleil2);
    
    // Astre Terre = Astre(0,0.0,0.0,0,6*pow(10,24),6*pow(10,6));
    // Terre.setColor(0.0,0.0,1.0);
    // Astre Lune = Astre(384*pow(10,6),0.0,0.0,1000,7.3*pow(10,22),2*pow(10,6));
    // Lune.setColor(1.0,1.0,1.0);
    


    

    GLWidget* glwidget = window.getGLWidget();

    glwidget->setSimu(simu);

    


    window.resize(500, 500); 
    window.show();
    

    return app.exec();

}

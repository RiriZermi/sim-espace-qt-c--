#include "mainwindow.h"
#include "glwidget.h"
#include "simulation.h"
#include <iostream>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include "astredialog.h"
#include "widget_incr_decr.h"
#include "widget_on_off.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),menuBar(nullptr), followMenu(nullptr)
{
    // Création du label avec un texte simple
    glwidget = new GLWidget(this);

    render_ = glwidget->getRenderer();

    // Centrage dans la fenêtre principale
    setCentralWidget(glwidget);

    //QMenu / QMenuBar
    menuBar = new QMenuBar(this);
    //followMenu
    followMenu = new QMenu("Follow Astres",this);
    menuBar->addMenu(followMenu);

    //addAstreAction
    addAstreAction = new QAction("Add Astre",this);
    menuBar->addAction(addAstreAction);
    connect(addAstreAction,QAction::triggered,this,&MainWindow::addAstre);

    //delAstreMenu
    delAstreMenu = new QMenu("Delete Astre",this);
    menuBar->addMenu(delAstreMenu);

    //Resize Astre
    resizeAstreMenu = new QMenu("Resize Astre",this);
    menuBar->addMenu(resizeAstreMenu);

    //Simu time acc/sloww
    simuTimeAction = new QAction("Simu Time accelerate/slow",this);
    menuBar->addAction(simuTimeAction);
    connect(simuTimeAction,&QAction::triggered,this,&MainWindow::openSimuTimeWidget);

    //place menu bar
    setMenuBar(menuBar);

    //emit signal when simu is set
    connect(glwidget,&GLWidget::simuIsSet,this,&MainWindow::setSimu);

}

void MainWindow::setSimu(){
    simu_= glwidget->getSimulation();
    connect(simu_,&Simulation::astresUpdated,this,MainWindow::astresUpdated);

    //add on_off wiwdget
    onOffWidget* stopGoWidget = new onOffWidget(this, &(simu_->getOn()));
    //stopGoWidget->setAttribute(Qt::WA_DeleteOnClose, false); // on ne veut pas qu’il se ferme
    stopGoWidget->setFixedSize(150, 50); // adapte selon la taille souhaitée
    stopGoWidget->move(10, 10); // coin haut gauche
    stopGoWidget->show();

    //add name after simu
    astresUpdated();
}

void MainWindow::astresUpdated(){;
    addFollowQActions();
    addDeleteActions();
    addResizeActions();

    //---Update render max-----
    vector<Astre*> astres = simu_->getAstres();
    float max = 100; //valeur initial minimal
    for(size_t i=0;i<astres.size();i++){
        if(abs(astres[i]->x)>max){
            max = abs(astres[i]->x);
        }
        if(abs(astres[i]->y)>max){
            max = abs(astres[i]->y);
        }
    }
    render_->setMax(max);
    render_->initGridBuffers();

}

void MainWindow::addFollowQActions(){
    //clear
    followMenu->clear();
    
    vector<Astre*> astres = simu_->getAstres();
    //Nothing
    QAction* action = new QAction("Static view",this);
    connect(action,&QAction::triggered,this,[this](){
        render_->followAstre(nullptr);
    });
    followMenu->addAction(action);
    for(size_t i=0; i<astres.size();++i){
        
        QAction* action = new QAction((astres[i]->name).c_str(),this);
        Astre* astre = astres[i];
        connect(action,&QAction::triggered,this,[this,astre](){
            //followAstre(astre);
            render_->followAstre(astre);
        });
        followMenu->addAction(action);

    }

}


void MainWindow::addDeleteActions(){
    delAstreMenu->clear();
    
    vector<Astre*> astres = simu_->getAstres();

    for(size_t i=0; i<astres.size();++i){
        
        QAction* action = new QAction((astres[i]->name).c_str(),this);
        Astre* astre = astres[i];
        connect(action,&QAction::triggered,this,[this,astre](){
            simu_->removeAstre(astre);
            
        });
        delAstreMenu->addAction(action);

    }
    

}

void MainWindow::addResizeActions(){
    resizeAstreMenu->clear();
    
    vector<Astre*> astres = simu_->getAstres();

    for(size_t i=0; i<astres.size();++i){
        
        QAction* action = new QAction((astres[i]->name).c_str(),this);
        Astre* astre = astres[i];
        connect(action,&QAction::triggered,this,[this,astre](){
            double *value = &(astre->radius);
            incrDecrWidget *r = new incrDecrWidget(nullptr,value,2);  
            r->setAttribute(Qt::WA_DeleteOnClose); 
            r->setWindowTitle("Radius size"); 
            r->show();
            r->raise();
            r->activateWindow();

            
        });
        resizeAstreMenu->addAction(action);

    }
}


void MainWindow::addAstre(){
    AstreDialog dialog(this);

    if (dialog.exec()==QDialog::Accepted){
        double x = dialog.getX();
        double y = dialog.getY(); 
        double vx = dialog.getVx(); 
        double vy = dialog.getVy(); 
        double radius = dialog.getRadius(); 
        double m = dialog.getM();   
        string name = dialog.getName();
        double r = dialog.getR();
        double g = dialog.getG();
        double b = dialog.getB();

        Astre* astre = new Astre(name, x, y, vx, vy, m, radius, r, g, b);

        if (simu_ != nullptr) simu_->addAstre(astre); 
    }
    
}

void MainWindow::openSimuTimeWidget(){
    if (simu_ != nullptr){
        double *value = &(simu_->getCoefMult());
        incrDecrWidget *w = new incrDecrWidget(this,value,1.5);  
        w->setWindowFlags(Qt::Window);  //independant
        w->setAttribute(Qt::WA_DeleteOnClose); 
        w->setWindowTitle("Simu time"); 
        w->show();
        w->raise();
        w->activateWindow();
    }

}


MainWindow::~MainWindow()
{
    // Pas besoin de delete label, Qt s'en occupe (car parenté)
}

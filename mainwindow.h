#pragma once

#include <QMainWindow>
#include <vector>

class GLWidget;
class Simulation;
class Renderer;
class Astre;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    GLWidget* getGLWidget(){return glwidget;}
    ~MainWindow();

    void addFollowQActions();
    void addDeleteActions();
    void addResizeActions();
    void addAstre();

    void openSimuTimeWidget();
    
    void setSimu();
    //void followAstre(Astre*);

private slots:
    void astresUpdated();
private:
    GLWidget *glwidget;
    QMenuBar* menuBar;
    QMenu *followMenu,*delAstreMenu,*resizeAstreMenu;
    QAction* addAstreAction,*simuTimeAction;
    Simulation* simu_;
    Renderer* render_;
};

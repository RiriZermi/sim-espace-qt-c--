#pragma once
#include <vector>
#include "astres.h"
#include <QObject>

using namespace std;
class Simulation:public QObject
{
    Q_OBJECT
    public:

    Simulation();
    ~Simulation();
    void addAstre(Astre*);
    void removeAstre(Astre*);
    vector<Astre*> getAstres();
    void updateSim(double);
    void applyGravityNewton(double);

    double& getCoefMult();
    bool& getOn();

    signals:
    void astresUpdated();

    private:
    vector<Astre*> astres;
    double coefMult_;
    bool on_;



};
#include "simulation.h"
#include <cmath>
#include <iostream>

Simulation::Simulation(): astres(), on_(true)
{
    coefMult_=1;
}

Simulation::~Simulation(){}

void Simulation::addAstre(Astre* astre){
    astres.push_back(astre);
    emit astresUpdated();
}

void Simulation::removeAstre(Astre* astre){
    for (unsigned int i=0; i<astres.size();i++){
        if (astres[i]== astre) {
            astres.erase(astres.begin()+i);
            break;
        }
    }
    emit astresUpdated();
}
vector<Astre*> Simulation::getAstres(){
    
    return astres;
}

void Simulation::updateSim(double dt){
    if (on_){
        dt *= 24*60*365 * coefMult_; // 1 minute relle = 1 année simu * coefMult ex : si coef mult = 2 1 minute relle = 2 année simu
        applyGravityNewton(dt); //update astres vel

        for (unsigned int i=0;i<astres.size();i++){
            astres[i]->x += dt*astres[i]->vx;
            astres[i]->y += dt*astres[i]->vy;
        }
    }


}

void Simulation::applyGravityNewton(double dt){
    const double G = 6.67430e-11f;
    // const double G = 10000;
    for (unsigned int i=0;i<astres.size();i++){
        for (unsigned int j=i+1;j<astres.size();j++){

            Astre& a1 = *astres[i];
            Astre& a2 = *astres[j];

            double dx = a2.x - a1.x;
            double dy = a2.y - a1.y;

            double distSquare = dx*dx + dy*dy;
            double dist = sqrt(distSquare);
            if (dist < 1e-10f) continue; // éviter division par zéro

            double F = G * a1.m * a2.m / distSquare;

            double Fx = F * dx / dist;
            double Fy = F * dy / dist;

            double a1_x = Fx / a1.m;
            double a1_y = Fy / a1.m;
            
            double a2_x = -Fx / a2.m;
            double a2_y = -Fy / a2.m;

            a1.vx += a1_x * dt;
            a1.vy += a1_y * dt;
            a2.vx += a2_x * dt;
            a2.vy += a2_y * dt;
        }
    }
}

double& Simulation::getCoefMult() {return coefMult_;}
bool& Simulation::getOn(){return on_;}
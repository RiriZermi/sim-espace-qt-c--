#pragma once
#include <string>
class Astre{
    public:
    Astre(std::string NAME, double X, double Y,double VX, double VY, double M, double RADIUS,double R=1.0, double G=1.0,double B=1.0):name(NAME),x(X),y(Y),vx(VX),vy(VY),m(M),radius(RADIUS),r(R),g(G),b(B){}
    std::string name;
    double x,y;
    double vx,vy;
    double m;
    double radius;
    double r,g,b;
};
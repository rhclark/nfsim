#include "NFcore.hh"
#include "DiffusionClass.hh"

using namespace std;
using namespace NFcore;

void DiffusionClass::getValue(string& value){
    value = "";
}

double DiffusionClass::getDiffusionValue(Complex* complex){
    return 0;
}

void ConstantDiffusion::getValue(string& value){
    value = "";
}

double ConstantDiffusion::getDiffusionValue(Complex* complex){
    //KB*T/(6*PI*mu_EC*Rs)
    return 0;
}

void EinsteinStokes::getValue(string& value){
    value = "";
}

double EinsteinStokes::getDiffusionValue(Complex* complex){
    //KB*T*LOG((mu_PM*h/(Rc*(mu_EC+mu_CP)/2))-gamma)/(4*PI*mu_PM*h)
    return 0;
}

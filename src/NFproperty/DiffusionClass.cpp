#include "NFcore.hh"
#include "DiffusionClass.hh"

using namespace std;
using namespace NFcore;

void DiffusionClass::getValue(string& value){
    value = "";
}

double DiffusionClass::getDiffusion(){
    return 0;
}

void ConstantDiffusion::getValue(string& value){
    value = "";
}

double ConstantDiffusion::getDiffusion(){
    return 0;
}

void EinsteinStokes::getValue(string& value){
    value = "";
}

double EinsteinStokes::getDiffusion(){
    return 0;
}

#include "NFcore.hh"
#include "NFproperty.hh"

using namespace std;
using namespace NFcore;

void DiffusionClass::getValue(string& value){
    value = "";
}

double DiffusionClass::getDiffusionValue(){
    return 0;
}

void ConstantDiffusion::getValue(string& value){
    value = this->value;
}

double ConstantDiffusion::getDiffusionValue(){
    return stof(this->value);
}




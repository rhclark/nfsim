#include "NFcore.hh"
#include "NFproperty.hh"
#include "NFutil.hh"

using namespace std;
using namespace NFcore;


void EinsteinStokes::getValue(string& value){
    value = "";
}

double EinsteinStokes::getDiffusionValue(){
    double kb = NFutil::convertToDouble(this->getProperty("kb")->getValue());
    double t =  NFutil::convertToDouble(this->getProperty("temperature")->getValue());
    double rs = NFutil::convertToDouble(this->getProperty("rs")->getValue());
    double viscosity = NFutil::convertToDouble(this->getProperty("viscosity")->getValue());
    double pi = 3.141592;

    return kb*t/(6*pi*viscosity*rs);
    //KB*T/(6*PI*mu_EC*Rs)
    //KB*T*LOG((mu_PM*h/(Rc*(mu_EC+mu_CP)/2))-gamma)/(4*PI*mu_PM*h)
    return 0;
}

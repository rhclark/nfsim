#include "NFcore.hh"
#include "NFproperty.hh"
#include "NFutil.hh"

using namespace std;
using namespace NFcore;


void SaffmanDelbruck::getValue(string& value){
    value = "";
}

double SaffmanDelbruck::getDiffusionValue(){
    //KB*T*LOG((mu_PM*h/(Rc*(mu_EC+mu_CP)/2))-gamma)/(4*PI*mu_PM*h)
    double kb = NFutil::convertToDouble(this->getProperty("kb")->getValue());
    double t =  NFutil::convertToDouble(this->getProperty("temperature")->getValue());
    double rc = NFutil::convertToDouble(this->getProperty("rs")->getValue());
    double gamma = NFutil::convertToDouble(this->getProperty("gamma")->getValue());
    double width = NFutil::convertToDouble(this->getProperty("width")->getValue());
    double viscosity = NFutil::convertToDouble(this->getProperty("viscosity")->getValue());
    double surrounding_viscosity = getViscositySurroundingVolume();
    double size = 1;
    double pi = 3.141592;


    return kb*t*log((viscosity*width/(rc*surrounding_viscosity))-gamma)/(4*pi*viscosity*width);

}

double SaffmanDelbruck::getViscositySurroundingVolume(){
    //property->complex->compartment->system
    Compartment* compartment = dynamic_cast<Compartment*>(this->getContainer()->getContainer());
    if(!compartment){
        return -1.0;
    }
    //this might be worth doing in a different way once we have a more complex hierarchy
    System* system = dynamic_cast<System*>(compartment->getContainer());
    vector<shared_ptr<Compartment>> children;
    system->getAllCompartments().getCompartmentChildren(compartment->getName(), children);
    shared_ptr<Compartment> parent = system->getAllCompartments().getCompartment(compartment->getOutside());

    double parentViscosity = NFutil::convertToDouble(parent->getProperty("viscosity")->getValue());
    double childViscosity = NFutil::convertToDouble(children.at(0)->getProperty("viscosity")->getValue());

    return (parentViscosity + childViscosity)/2;
}

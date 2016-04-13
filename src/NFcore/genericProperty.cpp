#include "NFcore.hh"
#include "NFproperty.hh"

using namespace std;
using namespace NFcore;

GenericProperty::GenericProperty(string name, string value){
    this->name = name;
    this->value = value;
}

GenericProperty::GenericProperty(GenericProperty* genericProperty){
    this->name = genericProperty->getName();
    this->value = genericProperty->getValue();
    //since a GenericProperty can also be a Hierarchical node...
    this->propertyList = genericProperty->getProperties();
}

void GenericProperty::getValue(string& value){
    value = this->value;
}

GenericProperty* PropertyFactory::getPropertyClass(string id, string value){
    GenericProperty* property;
    if(id == "diffusion_function"){
        if(value == "Einstein_Stokes")
            property = new EinsteinStokes(id, value);
        else if(value =="Saffman_Delbruck")
            property = new SaffmanDelbruck(id, value);
        else
            property = new ConstantDiffusion(id, value);
    }
    else
        property =  new GenericProperty(id, value);

    return property;
}
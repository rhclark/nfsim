#include "NFcore.hh"
#include "NFproperty.hh"

using namespace std;
using namespace NFcore;

GenericProperty::GenericProperty(string name, string value){
    this->name = name;
    this->value = value;
}

GenericProperty::GenericProperty(shared_ptr<GenericProperty> genericProperty){
    this->name = genericProperty->getName();
    this->value = genericProperty->getValue();
    //since a GenericProperty can also be a Hierarchical node...
    this->propertyList = genericProperty->getProperties();
}


void GenericProperty::getValue(string& value){
    value = this->value;
}

shared_ptr<GenericProperty> PropertyFactory::getPropertyClass(string id, string value){
    shared_ptr<GenericProperty> property;
    if(id == "diffusion_function"){
        if(value == "Einstein_Stokes")
            property = std::make_shared<EinsteinStokes>(id, value);
        else if(value =="Saffman_Delbruck")
            property = std::make_shared<SaffmanDelbruck>(id, value);
        else if(value =="Derived_Diffusion")
            property = std::make_shared<DerivedDiffusion>(id, value);
        else
            property = std::make_shared<ConstantDiffusion>(id, value);
    }
    else
        property =  std::make_shared<GenericProperty>(id, value);

    return property;
}
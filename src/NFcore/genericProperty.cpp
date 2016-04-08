#include "NFcore.hh"


using namespace std;
using namespace NFcore;

GenericProperty::GenericProperty(string name, string value){
    this->name = name;
    this->value = value;
}

void GenericProperty::getValue(string& value){
    value = this->value;
}
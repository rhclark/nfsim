/*
 * CompartmentList.cpp
 *
 *  Created on: 26 March, 2016
 *      Author: Jose Juan Tapia
 */


#include "NFcore.hh"
#include <math.h>

using namespace std;
using namespace NFcore;




// Destructor
CompartmentList::~CompartmentList()
{
    // delete complex objects on the list
    Compartment* c;

    compartmentList.clear();
}


Compartment* CompartmentList::getCompartment(string name)
{
    return compartmentList.find(name)->second;
}

void CompartmentList::getCompartmentChildren(string name, vector<Compartment*> &children){
    for(auto it: compartmentList){
        if(it.second->getOutside() == name){
            children.push_back(it.second);
        }
    }
}

bool CompartmentList::addCompartment(Compartment* compartment)
{
    compartment->setContainer(sys);
    compartmentList[compartment->getName()] = compartment;

    return true;
}

bool CompartmentList::addCompartment(string name, int dimensions, double size, string outside)
{
    Compartment *compartment = new Compartment(name, dimensions, size, outside);
    compartment->setContainer(sys);
    this->addCompartment(compartment);
}

//compartment initialization method

Compartment::Compartment(string name, int spatialDimensions, double size, string outside)
{
    this->name = name;
    this->spatialDimensions = spatialDimensions;
    this->size = size;
    this->outside = outside;

    //this->addProperty("compartmentName", name);
    //this->addProperty("compartmentOuside", outside);
}


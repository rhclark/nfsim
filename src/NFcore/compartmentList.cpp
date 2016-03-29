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

bool CompartmentList::addCompartment(Compartment* compartment)
{
    compartmentList[compartment->getName()] = compartment;

    return true;
}

bool CompartmentList::addCompartment(string name, int dimensions, double size, string outside)
{
    Compartment *compartment = new Compartment(name, dimensions, size, outside);
    this->addCompartment(compartment);
}

//compartment initialization method

Compartment::Compartment(string name, int spatialDimensions, double size, string outside)
{
    this->name = name;
    this->spatialDimensions = spatialDimensions;
    this->size = size;
    this->outside = outside;
}

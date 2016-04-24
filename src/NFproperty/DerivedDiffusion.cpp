#include "NFcore.hh"
#include "NFproperty.hh"
#include "NFutil.hh"

using namespace std;
using namespace NFcore;


void DerivedDiffusion::getValue(string& value){
    value = "";
}

double DerivedDiffusion::getDiffusionValue(){
    //KB*T/(6*PI*mu_EC*Rs)
    vector<double> subunits2D;
    vector<double> subunits3D;
    double acc =0;
    Complex* complex = dynamic_cast<Complex*>(this->getContainer());
    for(auto it: complex->getComplexMembers()){
        string dimensionality = "";
        it->getProperty("dimensionality")->getValue(dimensionality);
        shared_ptr<DiffusionClass> diffusion = dynamic_pointer_cast<DiffusionClass>(it->getProperty("diffusion_function"));
        if(dimensionality == "2")
            subunits2D.push_back(diffusion->getDiffusionValue());
        else
            subunits3D.push_back(diffusion->getDiffusionValue());
    }
    
    if (subunits2D.size() > 0){

        for(auto it:subunits2D){
            acc += pow(it, -2);
        }
        acc =  pow(acc,-(0.5));
    }
    else{
        for(auto it:subunits3D){
            acc += pow(it, -3);
        }
        acc = pow(acc,-(0.3333333333333));
    }
    
    return acc;
}

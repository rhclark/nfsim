#include <NFapiAux.hh>
#include <NFinput/NFinput.hh>


using namespace std;

string calculateOriginalCompartment(string referenceSpecies, map<string, string> originalCompartments){
    map<int, vector<NFinput::componentStruct>> componentList;
    //a list containing an id-> molecule name equivalence list
    map<int, string> moleculeIndex; 
    map<int, string> moleculeCompartment;
    //stores molecule and component information
    vector<NFinput::componentStruct> componentIndex;
    vector<pair<int, int>> bondNumbers;

    map<string, string> speciesCompartmentMap;

    NFinput::transformComplexString(referenceSpecies, componentList, moleculeIndex, moleculeCompartment, 
                           componentIndex, bondNumbers);


    string speciesCompartment;
    for(auto it: moleculeIndex){
        speciesCompartment = originalCompartments[it.second];
    }
    return speciesCompartment;
}

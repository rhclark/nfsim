#include <NFapi.hh>
#include <NFinput/NFinput.hh>
#include <boost/range/irange.hpp>
#include <NFapiAux.hh>


namespace NFapi {
    NFinput::XMLStructures* xmlStructures = nullptr;
    System* system = nullptr;
    NFinput::XMLFlags xmlflags;
    map<numReactantQueryIndex, std::map<std::string, vector<map<string,string>>>> numReactantQueryDict;
    map<numReactantQueryIndex, vector<queryResults*>> mSystemQueryDict;
}

using namespace boost;

Compartment* NFapi::getCompartmentInformation(const std::string compartmentName)
{
    return NFapi::system->getAllCompartments().getCompartment(compartmentName);
};
//function declarations
void NFapi::calculateRxnMembership(System *s, 
                                        std::map<Complex*, vector<ReactionClass*>> &molMembership, 
                                        const int numOfReactants)
{
        Molecule* mol = nullptr;
        Complex* complex = nullptr;

        molMembership.clear();
        //iterate over all molecule types...
        for(auto i: irange(0, s->getNumOfMoleculeTypes())){
            // and all molecule agents associated with those molecule types...
            for(auto m: irange(0, s->getMoleculeType(i)->getMoleculeCount())){
                mol = s->getMoleculeType(i)->getMolecule(m);
                complex = mol->getComplex();
                std::vector<ReactionClass*> rxnMembership = s->getMoleculeType(i)->getReactionClassMembership(mol);

                //keep only those reactions that match the number of reactants we are interested in
                rxnMembership.erase( std::remove_if( rxnMembership.begin(), rxnMembership.end(),
                              [numOfReactants](ReactionClass* p){
                                return p->getNumOfReactants() != numOfReactants || p->get_a() == 0; 
                              }), rxnMembership.end() );

                // if there's any reactions we qualify for store that molecule
                if ((rxnMembership.size()) > 0){
                    molMembership[complex].reserve( molMembership[complex].size() + distance(rxnMembership.begin(),rxnMembership.end()));
                    molMembership[complex].insert( molMembership[complex].end(),rxnMembership.begin(),rxnMembership.end());

                }


            }
        }

}


bool NFapi::setupNFSim(const char* filename, bool verbose){
    map<string,string> argMap;
    argMap["cb"] = "1";
    argMap["xml"] = filename;

    NFapi::xmlflags = getXMLInitializationParameters(argMap, verbose);
    NFapi::xmlStructures = getXMLStructureFromFlags(argMap, verbose);

    if(NFapi::xmlStructures == nullptr)
        return false;
    
    return true;
}

bool NFapi::resetSystem(){

    if(NFapi::system != NULL)
        delete NFapi::system;

    NFapi::system = initializeNFSimSystem(NFapi::xmlStructures, NFapi::xmlflags.cb, NFapi::xmlflags.globalMoleculeLimit, NFapi::xmlflags.verbose,
                        NFapi::xmlflags.suggestedTraversalLimit, NFapi::xmlflags.evaluateComplexScopedLocalFunctions);
    return NFapi::system != NULL;
}

bool NFapi::initSystemXML(const string initXML){
    TiXmlDocument* doc = new TiXmlDocument();
    doc->Parse(initXML.c_str());
    TiXmlHandle hDoc(doc);

    TiXmlElement *  listOfSpecies = hDoc.FirstChildElement().Node()->FirstChildElement("ListOfSpecies");
    bool result = NFinput::initStartSpecies(listOfSpecies, NFapi::system, NFinput::parameter, NFinput::allowedStates, false);
    NFapi::system->prepareForSimulation();

    return result;
}

bool NFapi::initSystemNauty(const std::map<std::string, int> localMap){

    bool result = NFinput::initStartSpeciesFromCannonicalLabels(localMap, NFapi::system, 
                        NFinput::parameter, NFinput::allowedStates, false);

    NFapi::system->prepareForSimulation();

    return result;
}


void NFapi::queryByNumReactant(std::map<std::string, vector<map<string,string>>> &structData, const int numOfReactants) {
    std::map<Complex*, vector<ReactionClass*>> molMembership;
    NFapi::calculateRxnMembership(NFapi::system, molMembership, numOfReactants);

    for(auto cpx: molMembership){
        vector<map<string,string>> reactions;
        if (structData.find(cpx.first->getCanonicalLabel()) == structData.end()){
        for(auto rxn: cpx.second){
                std::map<std::string, string> localData;
                localData["rate"] = std::to_string(rxn->getBaseRate());
                localData["name"] = rxn->getName();
                //localData["numPlayers"] = 0;
                reactions.push_back(localData);
            }
            structData[cpx.first->getCanonicalLabel()] = reactions;
        }
    }


}

map<string, string> NFapi::extractSpeciesCompartmentMapFromNauty(const std::string nauty){
    //temporarily map a molecule idx to its children components
    map<int, vector<NFinput::componentStruct>> componentList;
    //a list containing an id-> molecule name equivalence list
    map<int, string> moleculeIndex; 
    map<int, string> moleculeCompartment;
    //stores molecule and component information
    vector<NFinput::componentStruct> componentIndex;
    vector<pair<int, int>> bondNumbers;

    map<string, string> speciesCompartmentMap;

    NFinput::transformComplexString(nauty, componentList, moleculeIndex, moleculeCompartment, 
                           componentIndex, bondNumbers);

    Compartment* finalCompartment = nullptr;
    for(auto it:moleculeCompartment){
        auto temp = getCompartmentInformation(it.second);

        if(temp->getSpatialDimensions() == 2){
            finalCompartment = temp;
        }
        else if(finalCompartment == nullptr){
            finalCompartment = temp;
        }
    }

    for(auto it: moleculeIndex){
        speciesCompartmentMap[it.second] = finalCompartment->getName();
    }
    return speciesCompartmentMap;
}

string NFapi::extractSpeciesCompartmentFromNauty(const std::string nauty){
    auto compartmentMap = NFapi::extractSpeciesCompartmentMapFromNauty(nauty);
    return compartmentMap.begin()->second;
}


bool NFapi::initAndQueryByNumReactant(NFapi::numReactantQueryIndex &query, 
                                      std::map<std::string, vector<map<string,
                                                     string>>> &structData)
{

    //memoization
    if(NFapi::numReactantQueryDict.find(query) != NFapi::numReactantQueryDict.end()){
        structData = NFapi::numReactantQueryDict[query];
    }
    else{
        if(!NFapi::resetSystem())
            return false;
        if(!NFapi::initSystemNauty(query.initMap)){
            return false;
        }
        if(query.options.find("reaction") != query.options.end())
            NFapi::stepSimulation(query.options["reaction"]);

        if(query.options.find("numReactants") != query.options.end())
            NFapi::queryByNumReactant(structData, std::stoi(query.options["numReactants"]));


        //store for future use
        NFapi::numReactantQueryDict[query] = structData;
    }
    return true;
}

bool NFapi::initAndQuerySystemStatus(NFapi::numReactantQueryIndex &query, 
                                     vector<queryResults*> &labelSet)
{
    //memoization
    if(NFapi::mSystemQueryDict.find(query) != NFapi::mSystemQueryDict.end()){
        labelSet = NFapi::mSystemQueryDict[query];
    }
    else{
        map<string, string> inputCompartments;
        if(!NFapi::resetSystem())
            return false;
        if(!NFapi::initSystemNauty(query.initMap))
        {
            return false;
        }
        // get species comparment info
        for(auto it:query.initMap){
            auto newMap = NFapi::extractSpeciesCompartmentMapFromNauty(it.first);
            inputCompartments.insert(newMap.begin(), newMap.end());
        }


        if(query.options.find("reaction") != query.options.end()){
            NFapi::stepSimulation(query.options["reaction"]);
        }

        if(query.options.find("systemQuery") != query.options.end()){
            NFapi::querySystemStatus(query.options["systemQuery"], labelSet);            

            if(query.options["systemQuery"] == "complex"){
                for(auto it: labelSet){
                    it->originalCompartment = calculateOriginalCompartment(it->label, inputCompartments);
                }
            }
        }


        //store for future use
        NFapi::mSystemQueryDict[query] = labelSet;

    }
    return true;

}


void NFapi::querySystemStatus(std::string printParam, vector<queryResults*> &labelSet)
{

    labelSet.clear();

    if (printParam == "complex")
    {      
        const vector<Complex*> complexList = (NFapi::system->getAllComplexes()).getAllComplexesVector();
        
        for(auto complex: complexList){
                
            if(complex->isAlive()){
                queryResults* results = new queryResults();
                results->label = complex->getCanonicalLabel();
                results->compartment = complex->getCompartment()->getName();
                //double diffusion = complex->getProperty("diffusion");
                labelSet.push_back(results);
            }
        }
        
    }
    else if(printParam == "observables")
    {
        map<string,double> basicMolecules = NFapi::system->getAllObservableCounts();
        for(auto it: basicMolecules){
            for(int i: irange(0,(int)it.second)){
                queryResults* results = new queryResults();
                results->label = it.first;
                labelSet.push_back(results);
            }
        }
    }
}

bool NFapi::queryObservables(map<std::string, double> &observables){
    observables = NFapi::system->getAllObservableCounts();

    return true;
}


bool NFapi::stepSimulation(const std::string rxnName){
    auto rxn = NFapi::system->getReaction(rxnName);
    //sending a negative number causes the firing function to recalculate the random number used
    //for argument
    rxn->fire(-1);
    return 0;
}

bool NFapi::stepSimulation(){
    NFapi::system->singleStep();

}

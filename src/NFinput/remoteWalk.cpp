#include "NFinput.hh"
#include <boost/range/irange.hpp>


namespace RPCServer {
System* system = NULL;
}


using namespace std;
using namespace NFcore;
using namespace boost;

//function declarations
int remoteenterMainMenuLoop(System *s);
void remoteenterStepLoop(System *s);

void remotegetPrintout(System *s) ;
int remotegetInput(int min, int max);
double remotegetInput(double min);


RPCServer::nfsimReset::nfsimReset(NFinput::XMLStructures* x, NFinput::XMLFlags f): xmlStructures(x), xmlflags(f)
{
    this->_signature = "i:i";
    this->_help = "This method simulates a chemical system given a simulation time and number of output steps";

}

void RPCServer::nfsimReset::execute(xmlrpc_c::paramList const& paramList,
                                    xmlrpc_c::value *   const  retvalP)
{


    //RPCServer::system = NFinput::initializeFromXML(xmlflags.filename,xmlflags.cb,xmlflags.globalMoleculeLimit,xmlflags.verbose,
    //                                      xmlflags.suggestedTraversalLimit,xmlflags.evaluateComplexScopedLocalFunctions);
    paramList.verifyEnd(0);


    RPCServer::system = initializeNFSimSystem(xmlStructures, xmlflags.cb, xmlflags.globalMoleculeLimit, xmlflags.verbose,
                        xmlflags.suggestedTraversalLimit, xmlflags.evaluateComplexScopedLocalFunctions);

    if ((RPCServer::system) == NULL)
        *retvalP = xmlrpc_c::value_boolean(false);
    else
        *retvalP = xmlrpc_c::value_boolean(true);

}

RPCServer::nfsimInit::nfsimInit()
{
    this->_signature = "i:iiiii";
    this->_help = "This method initializes the nfsim system with some speceis starting conditions";

}

void RPCServer::nfsimInit::execute(xmlrpc_c::paramList const& paramList,
                                   xmlrpc_c::value *   const  retvalP)
{

    std::string const initXML(paramList.getString(0));
    paramList.verifyEnd(1);


    TiXmlDocument* doc = new TiXmlDocument();
    doc->Parse(initXML.c_str());
    TiXmlHandle hDoc(doc);

    TiXmlElement *  listOfSpecies = hDoc.FirstChildElement().Node()->FirstChildElement("ListOfSpecies");

    bool result = NFinput::initStartSpecies(listOfSpecies, RPCServer::system, NFinput::parameter, NFinput::allowedStates, true);

    RPCServer::system->prepareForSimulation();

    *retvalP = xmlrpc_c::value_boolean(result);
}


RPCServer::nfsimStep::nfsimStep()
{
    this->_signature = "i:iiiiii";
    this->_help = "This method advances the simulation one step";

}

void RPCServer::nfsimStep::execute(xmlrpc_c::paramList const& paramList,
                                   xmlrpc_c::value *   const  retvalP)
{

    //Perform a single simulation step
    RPCServer::system->singleStep();

    *retvalP = xmlrpc_c::value_boolean(true);

}




RPCServer::nfsimQuery::nfsimQuery() {
    // signature and help strings are documentation -- the client
    // can query this information with a system.methodSignature and
    // system.methodHelp RPC.
    this->_signature = "i:iiii";
    this->_help = "This method returns the current state of the system";
}
void RPCServer::nfsimQuery::execute(xmlrpc_c::paramList const& paramList,
                                    xmlrpc_c::value *   const  retvalP) {

    int const numOfReactants(paramList.getInt(0));
    paramList.verifyEnd(1);


    calculateRxnMembership(RPCServer::system, numOfReactants);
    *retvalP = xmlrpc_c::value_int(1);
}

void RPCServer::nfsimQuery::calculateRxnMembership(System *s, const int numOfReactants)
{
        this->molMembership.clear();
        //iterate over all molecule types...
        for(auto i: irange(0, s->getNumOfMoleculeTypes())){
            // and all molecule agents associated with those molecule types...
            for(auto m: irange(0, s->getMoleculeType(i)->getMoleculeCount())){

                Molecule* mol = s->getMoleculeType(i)->getMolecule(m);
                std::vector<ReactionClass*> rxnMembership = s->getMoleculeType(i)->getReactionClassMembership(mol);

                //keep only those reactions that match the number of reactants we are interested in
                rxnMembership.erase( std::remove_if( rxnMembership.begin(), rxnMembership.end(),
                              [numOfReactants](ReactionClass* p){
                                return p->getNumOfReactants() == numOfReactants; 
                              }), rxnMembership.end() );

                // if there's any reactions we qualify for store them
                if ((rxnMembership.size()) > 0){
                    mol->printDetails();
                    cout << "associated rates:\n";
                    for (auto rxn: rxnMembership){
                        cout<< rxn->getBaseRate() << " ";
                    }
                    cout <<"\n";
                    molMembership.emplace(std::make_pair(mol, rxnMembership));

                }


            }
        }
}



void NFinput::remoteWalk(NFinput::XMLStructures* xmlStructures, NFinput::XMLFlags xmlFlags)
{
    try {
        xmlrpc_c::registry myRegistry;

        xmlrpc_c::methodPtr const nfsimResetO(new RPCServer::nfsimReset(xmlStructures, xmlFlags));
        xmlrpc_c::methodPtr const nfsimInitO(new RPCServer::nfsimInit);
        xmlrpc_c::methodPtr const nfsimStepO(new RPCServer::nfsimStep);
        xmlrpc_c::methodPtr const nfsimQueryO(new RPCServer::nfsimQuery);


        myRegistry.addMethod("nfsim.reset", nfsimResetO);
        myRegistry.addMethod("nfsim.init", nfsimInitO);
        myRegistry.addMethod("nfsim.step", nfsimStepO);
        myRegistry.addMethod("nfsim.query", nfsimQueryO);

        xmlrpc_c::serverAbyss nfsimServer(
            xmlrpc_c::serverAbyss::constrOpt()
            .registryP(&myRegistry)
            .portNumber(8080));

        nfsimServer.run();
    } catch (exception const& e) {
        cerr << "Something failed.  " << e.what() << endl;
    }
}



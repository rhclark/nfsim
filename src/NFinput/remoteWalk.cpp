#include "NFinput.hh"


namespace RPCServer{
	System* system = NULL;
}


using namespace std;
using namespace NFcore;


//function declarations
int remoteenterMainMenuLoop(System *s);
void remoteenterStepLoop(System *s);

void remotegetPrintout(System *s) ;
int remotegetInput(int min, int max);
double remotegetInput(double min);


RPCServer::nfsimReset::nfsimReset(NFinput::XMLStructures* xmlStructures, NFinput::XMLFlags xmlflags) {
    // signature and help strings are documentation -- the client
    // can query this information with a system.methodSignature and
    // system.methodHelp RPC.
    this->_signature = "i:i";
        // method's result and two arguments are integers
    this->_help = "This method simulates a chemical system given a simulation time and number of output steps";

    this->xmlStructures = xmlStructures;
    this->xmlflags = xmlflags;


}

void RPCServer::nfsimReset::execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {
        

	//RPCServer::system = NFinput::initializeFromXML(xmlflags.filename,xmlflags.cb,xmlflags.globalMoleculeLimit,xmlflags.verbose,
	//										xmlflags.suggestedTraversalLimit,xmlflags.evaluateComplexScopedLocalFunctions);
	paramList.verifyEnd(0);

	RPCServer::system = initializeNFSimSystem(xmlStructures, xmlflags.cb, xmlflags.globalMoleculeLimit, xmlflags.verbose, 
			                      xmlflags.suggestedTraversalLimit, xmlflags.evaluateComplexScopedLocalFunctions);

	
	//if (RPCServer::system) == NULL:



}



RPCServer::nfsimSimulate::nfsimSimulate() {
    // signature and help strings are documentation -- the client
    // can query this information with a system.methodSignature and
    // system.methodHelp RPC.
    this->_signature = "i:iii";
        // method's result and two arguments are integers
    this->_help = "This method simulates a chemical system given a simulation time and number of output steps";
}
void RPCServer::nfsimSimulate::execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {
        
    int const simtime(paramList.getInt(0));
    int const nsteps(paramList.getInt(1));
    
    paramList.verifyEnd(2);
    
    RPCServer::system->sim(simtime,nsteps);

    // Sometimes, make it look hard (so client can see what it's like
    // to do an RPC that takes a while).
}


RPCServer::nfsimPrint::nfsimPrint(){
    // signature and help strings are documentation -- the client
    // can query this information with a system.methodSignature and
    // system.methodHelp RPC.
    this->_signature = "i:iiii";
    this->_help = "This method returns the current state of the system";
}
void RPCServer::nfsimPrint::execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {
        
    std::string const inputParameter(paramList.getString(0));
    paramList.verifyEnd(1);

    
    if(inputParameter == "observables")
    {	
    	RPCServer::system->printAllObservableCounts(RPCServer::system->getCurrentTime());
    }
    else if(inputParameter == "reactions")
    {
    	RPCServer::system->printAllReactions();
    }
    else if(inputParameter == "species")
    {
    	(RPCServer::system->getAllComplexes()).printAllComplexes();
    }
	*retvalP = xmlrpc_c::value_int(1);
}

void NFinput::remoteWalk(System *s)
{
    try {
    	RPCServer::system = s;
        xmlrpc_c::registry myRegistry;

        xmlrpc_c::methodPtr const nfsimSimulateO(new RPCServer::nfsimSimulate);
        xmlrpc_c::methodPtr const nfsimPrintO(new RPCServer::nfsimPrint);

        myRegistry.addMethod("nfsim.simulate", nfsimSimulateO);
        myRegistry.addMethod("nfsim.print", nfsimPrintO);
        
        xmlrpc_c::serverAbyss myAbyssServer(
            xmlrpc_c::serverAbyss::constrOpt()
            .registryP(&myRegistry)
            .portNumber(8080));
        
        myAbyssServer.run();
    } catch (exception const& e) {
        cerr << "Something failed.  " << e.what() << endl;
    }
}

void NFinput::remoteWalk(NFinput::XMLStructures* xmlStructures, NFinput::XMLFlags xmlFlags)
{
    try {
        xmlrpc_c::registry myRegistry;

        xmlrpc_c::methodPtr const nfsimResetO(new RPCServer::nfsimReset(xmlStructures, xmlFlags));

        myRegistry.addMethod("nfsim.reset", nfsimResetO);
        
        xmlrpc_c::serverAbyss myAbyssServer(
            xmlrpc_c::serverAbyss::constrOpt()
            .registryP(&myRegistry)
            .portNumber(8080));
        
        myAbyssServer.run();
    } catch (exception const& e) {
        cerr << "Something failed.  " << e.what() << endl;
    }
}


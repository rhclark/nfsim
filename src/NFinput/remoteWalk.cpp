#include "NFinput.hh"
#define WIN32_LEAN_AND_MEAN  /* required by xmlrpc-c/server_abyss.hpp */

#include <stdexcept>
#ifdef _WIN32
#  include <windows.h>
#else
#  include <unistd.h>
#endif


#include <iostream>
#include <stdint.h>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>



using namespace std;
using namespace NFcore;


//function declarations
int remoteenterMainMenuLoop(System *s);
void remoteenterStepLoop(System *s);

void remotegetPrintout(System *s) ;
int remotegetInput(int min, int max);
double remotegetInput(double min);



class nfsimEquilibrate : public xmlrpc_c::method {
public:
    nfsimEquilibrate(System* system) {
        // signature and help strings are documentation -- the client
        // can query this information with a system.methodSignature and
        // system.methodHelp RPC.
    	this->system = system;
        this->_signature = "i:ii";
            // method's result and two arguments are integers
        this->_help = "This method equilibrates a chemical system given an equilibration time";
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {
        
        int const eqtime(paramList.getInt(0));
        
        paramList.verifyEnd(1);
        
	    system->equilibrate(eqtime);

        // Sometimes, make it look hard (so client can see what it's like
        // to do an RPC that takes a while).
    }

private:
	System* system;
};

class nfsimSimulate : public xmlrpc_c::method {
public:
    nfsimSimulate(System* system) {
        // signature and help strings are documentation -- the client
        // can query this information with a system.methodSignature and
        // system.methodHelp RPC.
    	this->system = system;
        this->_signature = "i:iii";
            // method's result and two arguments are integers
        this->_help = "This method simulates a chemical system given a simulation time and number of output steps";
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {
        
        int const simtime(paramList.getInt(0));
        int const nsteps(paramList.getInt(1));
        
        paramList.verifyEnd(2);
        
	    this->system->sim(simtime,nsteps);

        // Sometimes, make it look hard (so client can see what it's like
        // to do an RPC that takes a while).
    }

private:
	System* system;
};

class nfsimPrint : public xmlrpc_c::method {
public:
    nfsimPrint(System* system) {
        // signature and help strings are documentation -- the client
        // can query this information with a system.methodSignature and
        // system.methodHelp RPC.
    	this->system = system;
        this->_signature = "i:iiii";
        this->_help = "This method returns the current state of the system";
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) {
        
        std::string const inputParameter(paramList.getString(0));
        paramList.verifyEnd(1);

        
	    if(inputParameter == "observables")
	    {	
	    	this->system->printAllObservableCounts(system->getCurrentTime());
	    }
	    else if(inputParameter == "reactions")
	    {
	    	this->system->printAllReactions();
	    }
	    else if(inputParameter == "species")
	    {
	    	(system->getAllComplexes()).printAllComplexes();
	    }
		*retvalP = xmlrpc_c::value_int(1);
    }

private:
	System* system;
};


void NFinput::remoteWalk(System *s)
{
    try {
        xmlrpc_c::registry myRegistry;

        xmlrpc_c::methodPtr const nfsimEquilibrateO(new nfsimEquilibrate(s));
        xmlrpc_c::methodPtr const nfsimSimulateO(new nfsimSimulate(s));
        xmlrpc_c::methodPtr const nfsimPrintO(new nfsimPrint(s));


        myRegistry.addMethod("nfsim.equilibrate", nfsimEquilibrateO);
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




void remoteprintMainMenu()
{
	cout<<"---------------------------"<<endl;
	cout<<" (0) equilibriate"<<endl;
	cout<<" (1) simulate"<<endl;
	cout<<" (2) enter stepper"<<endl;
	cout<<" (3) change output file"<<endl;
	cout<<" (4) print stuff"<<endl;
	cout<<" (5) exit"<<endl;
}

int remoteenterMainMenuLoop(System *s)
{
	bool exit = false;
	int exitCode = 0;
	while(true)
	{
		cout<<endl;
		remoteprintMainMenu();
		int selection = remotegetInput(0,5);
		double time = 0; int steps=0;
		switch(selection) {
			case 0:
			    cout<<"\nEnter equilibriation time: "<<endl;
			    time = remotegetInput(0.0);
			    cout<<"\nequilibriating for "<< time <<" seconds.  Please wait..."<<endl;
			    s->equilibrate(time);
			    break;
			case 1:
				cout<<"\nEnter simulation time: "<<endl;
				time = remotegetInput(0.0);
				cout<<"\nEnter number of output steps: "<<endl;
				steps = remotegetInput(0,INT_MAX);
				s->sim(time,steps);
				break;
			case 2:
				remoteenterStepLoop(s);
				break;
			case 3:
				cout<<"changing output file..."<<endl;
				exitCode = 2;
				break;
			case 4:
				cout<<endl<<endl;
				remotegetPrintout(s);
				break;
			case 5:
				exit = true;
				break;
		}

		if(exit) break;
	}
	return exitCode;
}




void remoteprintStepMenu()
{
	cout<<"---------------------------"<<endl;
	cout<<"(0) step to next reaction"<<endl;
	cout<<"(1) step"<<endl;
	cout<<"(2) step 1 second"<<endl;
	cout<<"(3) step 10 seconds"<<endl;
	cout<<"(4) print stuff"<<endl;
	cout<<"(5) output observables to file"<<endl;
	cout<<"(6) return"<<endl;
}

void remoteenterStepLoop(System *s)
{
	bool exit = false;
	while(true)
	{
		cout<<endl;
		remoteprintStepMenu();
		int selection = remotegetInput(0,6);
		double time = 0;
		switch(selection) {
			case 0:
				s->singleStep();
			    break;
			case 1:
				cout<<"\nEnter duration of step: "<<endl;
				time = remotegetInput(0.0);
				s->stepTo(s->getCurrentTime()+time);
				break;
			case 2:
				cout<<"Stepping for 1 second.  Please wait...  ";
				s->stepTo(s->getCurrentTime()+1);
				cout<<"done.\nCurrent Time is: "<<s->getCurrentTime()<<endl;
				break;
			case 3:
				cout<<"Stepping for 10 seconds.  Please wait...  ";
				s->stepTo(s->getCurrentTime()+10);
				cout<<"done.\nCurrent Time is: "<<s->getCurrentTime()<<endl;
				break;
			case 4:
				cout<<endl<<endl;
				remotegetPrintout(s);
				break;
			case 5:
				cout<<"Writing to file...  ";
				s->outputAllObservableCounts();
				cout<<"done."<<endl;
				break;
			case 6:
				exit = true;
				break;
		}

		if(exit) break;
	}
}





void remoteprintSpecificMolecule(System *s)
{
	while(true)
	{
		cout<<"Select the MoleculeType:"<<endl;
		cout<<" (-1) none"<<endl;
		for(int m=0; m<s->getNumOfMoleculeTypes(); m++)
			cout<<" ("<<m<<") "<<s->getMoleculeType(m)->getName()<<" - has "<< s->getMoleculeType(m)->getMoleculeCount()<<" molecules."<<endl;
		int selection = remotegetInput(-1,s->getNumOfMoleculeTypes()-1);
		if(selection==-1) break;

		while(true)
		{
			cout<<endl<<"Select a molecule (0 to "<<s->getMoleculeType(selection)->getMoleculeCount()-1<<", or -1 to exit):"<<endl;
			int selection2 = remotegetInput(-1,s->getMoleculeType(selection)->getMoleculeCount()-1);
			if(selection2==-1) break;
			cout<<endl<<endl;
			s->getMoleculeType(selection)->getMolecule(selection2)->printDetails();
		}
	}
}
void remoteprintSpecificMoleculeByUid(System *s)
{
	while(true)
	{
		cout<<"Enter the molecule's unique id (or -1 to return):"<<endl;
		int selection = remotegetInput(-1,Molecule::getUniqueIdCount()-1);
		if(selection==-1) break;

		cout<<endl;
		Molecule *mol = s->getMoleculeByUid(selection);
		if(mol!=0) mol->printDetails();
	}
}

void remotegetPrintout(System *s)
{
	cout<<"What would you like to print out?"<<endl;
	cout<<" (0) all reactions"<<endl;
	cout<<" (1) all MoleculeTypes"<<endl;
	cout<<" (2) all index values and names"<<endl;
	cout<<" (3) all complexes"<<endl;
	cout<<" (4) all observables"<<endl;
	cout<<" (5) specific reaction"<<endl;
	cout<<" (6) specific molecule by MoleculeType"<<endl;
	cout<<" (7) specific molecule by unique ID"<<endl;
	int selection = remotegetInput(0,8);
	switch(selection) {
		case 0:
			cout<<endl<<endl;
			s->printAllReactions();
		    break;
		case 1:
			cout<<endl<<endl;
			s->printAllMoleculeTypes();
			break;
		case 2:
			cout<<endl<<endl;
			s->printIndexAndNames();
			break;
		case 3:
			cout<<endl<<endl;
			// NETGEN -- redirect this call to the ComplexList object at s->allComplexes
			(s->getAllComplexes()).printAllComplexes();
			break;
		case 4:
			cout<<endl<<endl;
			s->printAllObservableCounts(s->getCurrentTime());
			break;
		case 5:
			cout<<endl<<endl;
			s->getReaction(0)->printDetails();
			break;
		case 6:
			cout<<endl<<endl;
			remoteprintSpecificMolecule(s);
			break;
		case 7:
			cout<<endl<<endl;
			remoteprintSpecificMoleculeByUid(s);
			break;
	}
}








int remotegetInput(int min, int max)
{
	bool validInput = false;
	int intVal = -1;
	while(!validInput)
	{
		cout<<">";
		string input; cin>>input;
		try {
			intVal = NFutil::convertToInt(input);
			if(intVal>=min&&intVal<=max) validInput=true;
			else cout<<"   ---not an option, try again."<<endl;
		} catch (std::runtime_error e) {
			cout<<"   ---not an option, try again."<<endl;
		}
		cin.clear();
	}
	return intVal;
}

double remotegetInput(double min)
{
	bool validInput = false;
	double doubVal = -1;
	while(!validInput)
	{
		cout<<">";
		string input; cin>>input;
		try {
			doubVal = NFutil::convertToDouble(input);
			if(doubVal>=min) validInput=true;
			else cout<<"   ---not an option, try again."<<endl;
		} catch (std::runtime_error e) {
			cout<<"   ---not an option, try again."<<endl;
		}
		cin.clear();
	}
	return doubVal;
}





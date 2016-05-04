#ifndef NFINPUT_HH_
#define NFINPUT_HH_

#include <map>
#include <vector>
#include <stdio.h>
// JUSTIN -- added to resolve problem finding INT_MAX
#include <limits.h>
#include <exception>


#include "../NFcore/NFcore.hh"
#include "../NFoutput/NFoutput.hh"
#include "../NFfunction/NFfunction.hh"
#include "../NFreactions/reactions/reaction.hh"
#include "TinyXML/tinyxml.h"

#define WIN32_LEAN_AND_MEAN  /* required by xmlrpc-c/server_abyss.hpp */

#include <stdexcept>
#ifdef _WIN32
#  include <windows.h>
#else
#  include <unistd.h>
#endif


#include <iostream>




using namespace NFcore;



//!  Functionality to handle input from XML files or command line arguments
/*!
	This is a very straightforward set of functions and simple classes that
	together parse and handle all the major input into NFsim.  The raw XML
	parsing code comes from the TinyXML project and can be found in its
	unedited form in the TinyXML directory.  Thanks TinyXML!  You perform great
	and you are definitely tiny!
    @author Michael Sneddon
 */
namespace NFinput {

	struct XMLStructures{
		string modelName;
		TiXmlElement *pListOfParameters;
		TiXmlElement *pListOfCompartments;
		TiXmlElement *pListOfFunctions;
		TiXmlElement *pListOfMoleculeTypes;
		TiXmlElement *pListOfSpecies;
		TiXmlElement *pListOfReactionRules;
		TiXmlElement *pListOfObservables;

		//this is kept separately till we define a proper structure for BNG-XML v1.1 properties
		TiXmlElement *pListOfExtendedBNGXML;
	};

	//! A simple structure containing XML initialization flags
	/*!
	  @author Jose Juan Tapia
	*/
	struct XMLFlags{
		std::string filename;
	    int suggestedTraversalLimit;
	    bool evaluateComplexScopedLocalFunctions;
	    int globalMoleculeLimit;
	    bool cb;
	    bool verbose;
	};

	struct componentStruct{
		string name;
		string state;
		int id;
		int membership;
		int order;
		vector<int> bonds;
	};

	struct moleculeStruct{
		vector<componentStruct> components;
	};

	struct complexStruct{
		vector<moleculeStruct> molecules;
		map<string, string> bondList;
	};


	//JJT: Contains maps that are used to share information between XML initialization methods
	// we are extracting them to this namespace so those XML init methods can be used in isolation.
	extern map<string, double> parameter;
	extern map<string,int> allowedStates;


	// JJT: Extracts information relevant to NFSim from a BNG-XML
	XMLStructures* loadXMLDataStructures(TiXmlDocument*, bool verbose);

	// JJT: Initializes the NFSim:system object from an XMLStructures object
	System* initializeNFSimSystem(XMLStructures*, bool, int, bool, int &, bool);


	//! Loads an XML file from I/O into memory. This is isolated so that we can reuse 
	//! the system specification later without reloading from disk
	/*!
		@author Jose Juan Tapia
	*/
	XMLStructures* loadXMLFile(string filename, bool verbose);

	//! Maintains information about a component of a TemplateMolecule.
	/*!
    	@author Michael Sneddon
	 */
	class component {
		public:
			component(shared_ptr<TemplateMolecule> t, string name);
			component(MoleculeType *mt, string name);
			~component();

			shared_ptr<TemplateMolecule> t;
			MoleculeType *mt;
			string name;
			string uniqueId;

			string symPermutationName;


			string numOfBondsLabel;
			string stateConstraintLabel;
	};



	//! Maintains information about a component of a TemplateMolecule.
	/*!
    	@author Michael Sneddon
	 */
	System * initializeFromXML(
			string filename,
			bool blockSameComplexBinding,
			int globalMoleculeLimit,
			bool verbose,
			int &suggestedTraversalLimit,
			bool evaluateComplexScopedLocalFunctions=false );

	//! Reads the parameter XML block and puts them in the parameter map.
	/*!
    	@author Michael Sneddon
	 */
	bool initParameters(
			TiXmlElement *pListOfParameters,
			System *s,
			map <string,double> &parameter,
			bool verbose);


	//! Reads the compartment XML block and puts them in the compartment list.
	/*!
    	@author Jose Juan Tapia
	 */
	bool initCompartments(
			TiXmlElement *pListOfCompartments, 
			System *s, 
			map <string,double> &parameter, 
			bool verbose);

	//! Reads the Function XML block and adds the Functions to the system.
	/*!
	   	@author Michael Sneddon
	*/
	bool initFunctions(
		TiXmlElement * pListOfFunctions,
		System * system,
		map <string,double> &parameter,
		TiXmlElement * pListOfObservables,
		map<string,int> &allowedStates,
		bool verbose);


	//! Reads the compartment XML block and puts them in the compartment list.
	/*!
    	@author Jose Juan Tapia
	 */
	bool initSystemExtendedProperties(TiXmlElement* pListOfExtendedBNGXML, 
									  System* s, 
									  map <string,double> &parameter, 
									  bool verbose);

	bool initEntityProperties(TiXmlElement* pListOfProperties,
							  HierarchicalNode* node,
							  map <string,double> &parameter, 
							  bool verbose);

	//! Reads the MoleculeType XML block and adds the MoleculeTypes to the system.
	/*!
    	@author Michael Sneddon
	 */
	bool initMoleculeTypes(
			TiXmlElement * pListOfMoleculeTypes,
			System * system,
			map<string,int> &allowedStates,
			bool verbose);





	bool FindReactionRuleSymmetry(
			TiXmlElement * pRxnRule,
			System * s,
			map <string,double> &parameter,
			map<string,int> &allowedStates,
			map <string, component> &symComps,
			map <string, component> &symRxnCenter,
			bool verbose);

	bool readPatternForSymmetry(
			TiXmlElement * pListOfMol,
			System * s,
			string patternName,
			map <string, component> &comps,
			map <string, component> &symComps,
			bool verbose);

	bool generateRxnPermutations(vector<map<string,component> > &permutations,
			map<string,component> &symComps,
			map<string,component> &symRxnCenter,
			bool verbose);


	bool readObservableForTemplateMolecules(
			TiXmlElement *pObs,
			string observableName,
			vector <shared_ptr<TemplateMolecule>> &tmList,
			vector <string> &stochRelation,
			vector <int> &stochQuantity,
			System *s,
			map <string,double> &parameter,
			map<string,int> &allowedStates,
			int obsType,
			bool verbose,
			int &suggestedTraversalLimit);


	bool initStartSpeciesFromCannonicalLabels(
		const std::map<std::string, int> &initMap,
		System * s,
		map<string,double> &parameter,
		map<string,int> &allowedStates,
		bool verbose);

	void transformComplexString(const string &,
								map<int, vector<shared_ptr<componentStruct>>> &,
								map<int, string> &,
								map<int, string> &,
								vector<shared_ptr<componentStruct>> &,
								vector<pair<int, int >> &bondNumbers);

	//! Reads a Species XML block, creates the molecules and adds them to the system.
	/*!
    	@author Michael Sneddon
	 */
	bool initStartSpecies(
			TiXmlElement * pListOfSpecies,
			System * system,
			map <string,double> &parameter,
			map<string,int> &allowedStates,
			bool verbose);

	//! Reads a string number (int or double) and parses it to integer
	/*!
		@author Michael Sneddon, Jose Juan Tapia
	*/
	int stringToInt(const std::string& s, const std::string& );
	double stringToDouble(const std::string &s, const std::string&);

	//! Reads a reactionRule XML block and adds the rules to the system.
	/*!
    	@author Michael Sneddon
	 */
	bool initReactionRules(
			TiXmlElement * pListOfReactionRules,
			System * system,
			map <string,double> &parameter,
			map<string,int> &allowedStates,
			bool blockSameComplexBinding,
			bool verbose,
			int &suggestedTraversalLimit);


	//! Sets the properties associated to a reaction rule
	/*!
		TODO: when the ListOfProperties field is created send it in too as a parameter
		@author Jose Juan Tapia
	*/
	bool setReactionRuleExtendedProperties(ReactionClass*, map<string,weak_ptr<TemplateMolecule>>);

	//Sums the dimensionality of individual reactants
	/*
		@author Jose Juan Tapia
	*/
	bool calculateReactionDimensionality(map<string, vector<MoleculeType*>>, int&);


	//! Reads an observable XML block and adds the new observables to the system.
	/*!
    	@author Michael Sneddon
	 */
	bool initObservables(
			TiXmlElement * pListOfObservables,
			System * system,
			map <string,double> &parameter,
			map<string,int> &allowedStates,
			bool verbose,
			int &suggestedTraversalLimit);


	//! Reads a pattern XML block and returns the set of new TemplateMolecule objects.
	/*!
    	@author Michael Sneddon
	 */
	shared_ptr<TemplateMolecule> readPattern(
			TiXmlElement * pListOfMol,
			System * s, map <string,double> &parameter,
			map<string,int> &allowedStates,
			string patternName,
			map <string, weak_ptr<TemplateMolecule>> &templates,
			map <string, component> &comps,
			map <string, component> &symMap,
			bool verbose,
			int &suggestedTraversalLimit);

	//! Reads a pattern XML block and returns the set of new TemplateMolecule objects.
	/*!
    	@author Michael Sneddon
	 */

		bool readProductPattern(
			TiXmlElement * pListOfMol,
			System * s, map <string,double> &parameter,
			map<string,int> &allowedStates,
			string patternName,
			vector <MoleculeType *> &productMoleculeTypes,
			vector < vector <int> > &stateInformation,
			vector < vector <int> > &bindingSiteInformation,
			bool verbose);

	//! Reads a product molecule XML block and returns a TemplateMolecule objects.
	/*!
	 *  As a side-effect, this also creates components for the product molecule
	 *   and all its sites/
	   	@author JustinHogg (based on Michael Sneddon's readProductPattern method)
	 */

	bool readProductMolecule(
			TiXmlElement * pMol,
			System * s,
			map <string,double> & parameter,
			map<string,int> & allowedStates,
			string patternName,
			vector <MoleculeCreator *> & moleculeCreatorsList,
			map <string, component> & comps,
			bool verbose );


	bool lookup(component *&c, string id, map<string,component> &comps, map<string,component> &symMap);



	//! Parses command line arguments from the console nicely.
	/*!
    	@author Michael Sneddon
	 */
	bool parseArguments(int argc, const char *argv[], map<string,string> &argMap);


	//! Looks up the argument in the argMap and tries to parse the value as an integer
	/*!
    	@author Michael Sneddon
	 */
	int parseAsInt(map<string,string> &argMap, string argName, int defaultValue);

	//! Looks up the argument in the argMap and tries to parse the value as a double
	/*!
    	@author Michael Sneddon
	 */
	double parseAsDouble(map<string,string> &argMap, string argName, double defaultValue);


	//! Looks up the argument in the argMap and tries to parse the value as a comma delimited sequence of ints
	/*!
    	@author Michael Sneddon
	 */
	void parseAsCommaSeparatedSequence(map<string,string> &argMap,string argName,vector<int> &sequence);



	//! Allows the user to walk through the system with an interactive text-based program
	/*!
    	@author Michael Sneddon
	 */
	void walk(System *s);


	//! Allows the user to walk through the system through remote XML-RPC calls
	/*!
    	@author Jose Juan Tapia
	 */
	//void remoteWalk(NFinput::XMLStructures*, NFinput::XMLFlags);


	//! Parses the cmd line arg that specifies system dumps, and schedules them.
	/*!
	    This method works by parsing the argument, initializing the class DumpSystem defined
	    in the file NFoutput.hh, and adding the DumpSystem to the System.
    	@author Michael Sneddon
	 */
	bool createSystemDumper(string paramStr, System *s, bool verbose);



	//! Parses a matlab style sequence (ie startValue:step:endValue) into the vector
	/*!
    	@author Michael Sneddon
	 */
	bool parseSequence(string numString, vector <double> &outputTimes);





	////////////// Functions that will allow parsing and running of an RNF script file

	bool readRNFfile(map<string,string> &argMap, vector<string> &commands, bool verbose);
	bool runRNFcommands(System *s, map<string,string> &argMap, vector<string> &commands, bool verbose);



	//bool runRNFscript(map<string,string> argMap) {};
   // bool runRNFscript(System *s, string filename);
}


#endif /*NFINPUT_HH_*/

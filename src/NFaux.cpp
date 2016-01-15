////////////////////////////////////////////////////////////////////////////////
//
//    NFsim: The Network Free Stochastic Simulator
//    A software platform for efficient simulation of biochemical reaction
//    systems with a large or infinite state space.
//
//    Copyright (C) 2009,2010,2011,2012
//    Michael W. Sneddon, James R. Faeder, Thierry Emonet
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//
//    For more information on NFsim, see http://emonet.biology.yale.edu/nfsim
//
////////////////////////////////////////////////////////////////////////////////

#include <NFsim.hh>

NFinput::XMLFlags getXMLInitializationParameters(map<string,string> argMap, bool verbose){
    NFinput::XMLFlags xmlflags;
    xmlflags.suggestedTraversalLimit = ReactionClass::NO_LIMIT;
    xmlflags.evaluateComplexScopedLocalFunctions = true;
    xmlflags.globalMoleculeLimit = 200000;
    xmlflags.cb = false;
    xmlflags.verbose = verbose;
    xmlflags.filename = argMap.find("xml")->second;

    //Create the system from the XML file
    // flag for blocking same complex binding.  If given,
    // then a molecule is blocked from binding another if
    // it is in the same complex

    bool blockSameComplexBinding = false;
    if (argMap.find("bscb")!=argMap.end()) {
        if(verbose) cout<<"  Blocking same complex binding...\n";

        blockSameComplexBinding = true;
    }

    bool turnOnComplexBookkeeping = false;
    if (argMap.find("cb")!=argMap.end())
        turnOnComplexBookkeeping = true;

    // enable/disable evaluation of complex scoped local functions
    if (argMap.find("nocslf")!=argMap.end())
        xmlflags.evaluateComplexScopedLocalFunctions = false;

    if (argMap.find("gml")!=argMap.end()) {
        xmlflags.globalMoleculeLimit = NFinput::parseAsInt(argMap,"gml",xmlflags.globalMoleculeLimit);
    }

    if(turnOnComplexBookkeeping || blockSameComplexBinding) xmlflags.cb=true;

    return xmlflags;
}


XMLStructures* getXMLStructureFromFlags(map<string,string> argMap, bool verbose)
{
    //Find the xml file that defines the system
    if (argMap.find("xml")!=argMap.end())
    {
        string filename = argMap.find("xml")->second;
        if(!filename.empty())
        {
            XMLStructures* xmlDataStructures = NFinput::loadXMLFile(filename, verbose);

            return xmlDataStructures;




        } else {
            cout<<"-xml flag given, but no file was specified, so no system was created."<<endl;
        }
    } else {
        cout<<"Couldn't create a system from your XML file.  No -xml [filename] flag given."<<endl;
    }
    return nullptr;

}

System *initSystemFromFlags(map<string,string> argMap, bool verbose)
{
    //Find the xml file that defines the system
    if (argMap.find("xml")!=argMap.end())
    {
        string filename = argMap.find("xml")->second;
        if(!filename.empty())
        {
            NFinput::XMLFlags flags = getXMLInitializationParameters(argMap, verbose);

            System *s = NFinput::initializeFromXML(flags.filename,flags.cb,flags.globalMoleculeLimit,flags.verbose,
                                                    flags.suggestedTraversalLimit,flags.evaluateComplexScopedLocalFunctions);


            if(s!=NULL)
            {
                if (setSystemVariables(argMap, verbose, flags.suggestedTraversalLimit, s)){
                    //Finally, return the system if we made it here without problems
                    return s;
                }
                else
                {
                    return 0;
                }
                
            }
            else  {
                cout<<"Couldn't create a system from your XML file.  I don't know what you did."<<endl;
                return 0;
            }
        } else {
            cout<<"-xml flag given, but no file was specified, so no system was created."<<endl;
        }
    } else {
        cout<<"Couldn't create a system from your XML file.  No -xml [filename] flag given."<<endl;
    }
    return 0;
}

bool setSystemVariables(map<string,string> argMap, bool verbose, int suggestedTraversalLimit, System* s){
    if(verbose) {cout<<endl;}

    //If requested, be sure to output the values of global functions
    if (argMap.find("ogf")!=argMap.end()) {
        s->turnOnGlobalFuncOut();
        if(verbose) cout<<"\tGlobal function output (-ogf) flag detected."<<endl<<endl;
    }

    // Also set the dumper to output at specified time intervals
    if (argMap.find("dump")!=argMap.end()) {
        if(!NFinput::createSystemDumper(argMap.find("dump")->second, s, verbose)) {
            cout<<endl<<endl<<"Error when creating system dump outputters.  Quitting."<<endl;
            delete s;
            return 0;
        }
    }

    // Set the universal traversal limit
    if (argMap.find("utl")!=argMap.end()) {
        int utl = -1;
        utl = NFinput::parseAsInt(argMap,"utl",utl);
        s->setUniversalTraversalLimit(utl);
        if(verbose) cout<<"\tUniversal Traversal Limit (UTL) set manually to: "<<utl<<endl<<endl;
    } else {
        s->setUniversalTraversalLimit(suggestedTraversalLimit);
        if(verbose) cout<<"\tUniversal Traversal Limit (UTL) set automatically to: "<<suggestedTraversalLimit<<endl<<endl;
    }

    if (verbose){
        // report status of complex-scoped local functions
        if ( s->getEvaluateComplexScopedLocalFunctions() ) {
            cout<<"\tComplex-scoped local function evaluation is enabled."<<endl<<endl;
        }
        else {
            cout<<"\tComplex-scoped local function evaluation is DISABLED!"<<endl<<endl;
        }
    }



    // turn on the event counter, if need be
    if (argMap.find("oec")!=argMap.end()) {
        s->turnOnOutputEventCounter();
        if(verbose) cout<<"\tEvent counter output (-oec) flag detected."<<endl<<endl;
    }

    // set the output to binary
    if (argMap.find("b")!=argMap.end()) {
        s->setOutputToBinary();
        if(verbose) cout<<"\tStandard output is switched to binary format."<<endl<<endl;
    }


    if(argMap.find("csv")!=argMap.end()) {
        s->turnOnCSVformat();
    }


    // tag any reactions that were tagged
    if (argMap.find("rtag")!=argMap.end()) {
        vector <int> sequence;
        NFinput::parseAsCommaSeparatedSequence(argMap,"rtag",sequence);

        if(verbose) {
            cout<<"\tTagging reactions by id (from the -rtag flag):";
            for(unsigned int k=0; k<sequence.size(); k++) cout<<" "<<sequence.at(k);
            cout<<endl;
        }
        for(unsigned int k=0; k<sequence.size(); k++) s->tagReaction(sequence.at(k));

    }


    //Register the output file location, if given
    if (argMap.find("o")!=argMap.end()) {
        string outputFileName = argMap.find("o")->second;
        s->registerOutputFileLocation(outputFileName);
        s->outputAllObservableNames();
    } else {
        if(s->isOutputtingBinary()) {
            s->registerOutputFileLocation(s->getName()+"_nf.dat");
            if(verbose) { cout<<"\tStandard output will be written to: "<< s->getName()+"_nf.dat" <<endl<<endl; }
        }
        else {
            s->registerOutputFileLocation(s->getName()+"_nf.gdat");
            s->outputAllObservableNames();
            if(verbose) cout<<"\tStandard output will be written to: "<< s->getName()+"_nf.gdat" <<endl<<endl;
        }
    }

    //turn off on the fly calculation of observables
    if(argMap.find("notf")!=argMap.end()) {
        s->turnOff_OnTheFlyObs();
        if(verbose) cout<<"\tOn-the-fly observables is turned on (detected -notf flag)."<<endl<<endl;
    }
    return 1;
}

bool runFromArgs(System *s, map<string,string> argMap, bool verbose)
{
    // default simulation time is 10 seconds outputting
    // once per second
    double eqTime = 0;
    double sTime = 10;
    int oSteps = 10;

    //Get the simulation time that the user wants
    eqTime = NFinput::parseAsDouble(argMap,"eq",eqTime);
    sTime = NFinput::parseAsDouble(argMap,"sim",sTime);
    oSteps = NFinput::parseAsInt(argMap,"oSteps",(int)oSteps);

    //Prepare the system for simulation!!
    s->prepareForSimulation();

    //Output some info on the system if we ask for it
    if(verbose) {
        cout<<"\n\nparse appears to be successful.  Here, check your system:\n";
        s->printAllMoleculeTypes();
        s->printAllReactions();
        s->printAllObservableCounts(0);
        cout<<endl;
        s->printAllFunctions();
        cout<<"-------------------------\n";
    }


    //If requested, walk through the simulation instead of running the simulation
    if (argMap.find("walk")!=argMap.end()) {
        NFinput::walk(s);
    }
    else {
        // Do the run
        cout<<endl<<endl<<endl<<"Equilibrating for :"<<eqTime<<"s.  Please wait."<<endl<<endl;
        s->equilibrate(eqTime);
        s->sim(sTime,oSteps);
    }

    // save the final list of species, if requested...
    if (argMap.find("ss")!=argMap.end()) {
        string filename = argMap.find("ss")->second;
        if(!filename.empty())  s->saveSpecies(filename);
        else   s->saveSpecies();
    }

    if(verbose) {
        cout<<endl<<endl;
        s->printAllReactions();
        cout<<endl;
        s->printAllObservableCounts(s->getCurrentTime());
    }

    return true;
}
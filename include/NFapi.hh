////////////////////////////////////////////////////////////////////////////////
//
//    NFsim: The Network Free Stochastic Simulator
//    A software platform for efficient simulation of biochemical reaction
//    systems with a large or infinite state space.
//
//    Copyright (C) 2009,2010,2011,2012
//    Michael W. Sneddon, James R. Faeder, Thierry Emonet, Jose Juan Tapia
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



#ifndef NFAPI_HH_
#define NFAPI_HH_

#include <NFsim.hh>



namespace NFapi{

    //memoization structures
    struct numReactantQueryIndex{
        std::map<string, int> initMap;
        std::map<string, string> options;

    };

    typedef struct numReactantQueryIndex numReactantQueryIndex;

    bool operator<(const numReactantQueryIndex& l, const numReactantQueryIndex& r) {
     return (l.initMap<r.initMap || (l.initMap==r.initMap && l.options<r.options));
    }

    bool operator==(const numReactantQueryIndex& l, const numReactantQueryIndex& r) {
     return (l.initMap==r.initMap && l.options==r.options);
    }




    //results container
    /*struct queryResults{
        string label;
        string compartment;
        string originalCompartment;
    };*/

    bool setupNFSim(const char* filename, bool);
    bool resetSystem();
    bool deleteSystem();
    
    bool initSystemXML(const string);
    bool initSystemNauty(const std::map<string, int>);
    //performs  exactly one simulation step
    bool stepSimulation();
    //performs exactly one simulation step by firing reaction rxn
    bool stepSimulation(const string rxn);


    void queryByNumReactant(std::map<std::string, vector<map<string,string>*>*> &, const int, const bool onlyActive=true);
    void querySystemStatus(std::string, vector<map<string, string>*> &);
    void calculateRxnMembership(System *, std::map<Complex*, vector<ReactionClass*>> &, 
                                            const int, const bool);

    //convenience function that calls reset, initializes the system as indicated by the query and queryByNumReactant while performing memoization
    bool initAndQueryByNumReactant(NFapi::numReactantQueryIndex &, 
                                   std::map<std::string, vector<map<string,string>*>*> &);



    bool initAndQuerySystemStatus(NFapi::numReactantQueryIndex &, 
                                  vector<map<string, string>*> &);


    bool queryObservables(map<std::string, double> &);

    //information about the model definition/compartments
    shared_ptr<Compartment> getCompartmentInformation(const std::string compartmentName);
    map<string, string> extractSpeciesCompartmentMapFromNauty(const std::string nautyString);

    //receives a nauty string, returns the compartment associated to the string
    string extractSpeciesCompartmentFromNauty(const std::string nautyString);

    extern map<numReactantQueryIndex, std::map<std::string, vector<map<string,string>*>*>> numReactantQueryDict;
    extern map<numReactantQueryIndex, vector<map<string, string>*>> mSystemQueryDict;

    extern NFinput::XMLFlags xmlflags;
    extern NFinput::XMLStructures* xmlStructures;
    extern System* system;
}


#endif

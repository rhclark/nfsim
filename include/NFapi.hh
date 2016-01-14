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
        int numReactants;
    };

    typedef struct numReactantQueryIndex numReactantQueryIndex;

    bool operator<(const numReactantQueryIndex& l, const numReactantQueryIndex& r) {
     return (l.initMap<r.initMap || (l.initMap==r.initMap && l.numReactants<r.numReactants));
    }

    bool operator==(const numReactantQueryIndex& l, const numReactantQueryIndex& r) {
     return (l.initMap==r.initMap && l.numReactants==r.numReactants);
    }


    bool setupNFSim(const char* filename, bool);
    bool resetSystem();
    bool initSystemXML(const string);
    bool initSystemNauty(const std::map<string, int>);
    //performs  exactly one simulation step
    bool stepSimulation();
    //performs exactly one simulation step by firying reaction rxn
    bool stepSimulation(const string rxn);


    void queryByNumReactant(std::map<std::string, vector<map<string,string>>> &, const int);
    void querySystemStatus(std::string, vector<string> &);
    void calculateRxnMembership(System *, std::map<Complex*, vector<ReactionClass*>> &, 
                                            const int);

    //convenience function that calls reset, initNauty and queryByNumReactant while performing memoization
    bool initAndQueryByNumReactant(const std::map<string, int>, std::map<std::string, vector<map<string,string>>> &, const int);

    extern map<numReactantQueryIndex, std::map<std::string, vector<map<string,string>>>> numReactantQueryDict;
    extern NFinput::XMLFlags xmlflags;
    extern NFinput::XMLStructures* xmlStructures;
    extern System* system;
}


#endif

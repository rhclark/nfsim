#ifndef XMLRPC_HH_
#define XMLRPC_HH_

//xml-rcp libraries
#include <stdint.h>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

/*
    namespace for the definition of objects we use for the NFSim XML-RPC server
    @author: Jose Juan Tapia
*/
namespace RPCServer{
    extern System* system;

    //define one class for each rpc service we offer

    class nfsimEquilibrate;
    class nfsimSimulate;
    class nfsimPrint;

    void calculateRxnMembership(System*, std::map<Complex*, vector<ReactionClass*>> &molMembership, const int);

    class nfsimEquilibrate: public xmlrpc_c::method
    {
    public:
        nfsimEquilibrate();
        void execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP);
    };

    class nfsimSimulate: public xmlrpc_c::method
    {
    public:
        nfsimSimulate();
        void execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP);
    };

    class nfsimQuery: public xmlrpc_c::method
    {
    public:
        nfsimQuery();
        void execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP);
        string serializeOutput();
        string serializeJsonOutput();
        


    private:
        std::map<Complex*, vector<ReactionClass*>> molMembership;
    };

    //! An XMLRPC server is initialized with a series of XML structures defining the system's initial state
    // This method recreates the global system object setting it to factory defaults
    /*!
        @author Jose Juan Tapia
    */  
    class nfsimReset: public xmlrpc_c::method
    {
    public:
        nfsimReset(NFinput::XMLStructures*, NFinput::XMLFlags);
        void execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP);
    private:
        NFinput::XMLStructures* xmlStructures;
        NFinput::XMLFlags xmlflags;
    };

    //! This class receives an XMLStructures* parameter with only the ListOfSpecies field defined
    /*!
        @author Jose Juan Tapia
    */  
    class nfsimInit: public xmlrpc_c::method
    {
    public:
        nfsimInit();
        void execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP);
    };

    //! Performs one simulation step on the global XMLRPC system object
    /*!
        @author Jose Juan Tapia
    */  

    class nfsimStep: public xmlrpc_c::method
    {
    public:
        nfsimStep();
        void execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP);
        ReactionClass* getNextReaction();
    private:
        std::map<Complex*, vector<ReactionClass*>> molMembership;
        std::set<ReactionClass*> activeRxnList;

    };

    //! Returns the system species state space as a list of species-rate tuples
    /*!
        @author Jose Juan Tapia
    */  
    class nfsimSpecies: public xmlrpc_c::method
    {
    public:
        nfsimSpecies();
        void execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP);
    };


}

#endif /*XMLRPC_HH_*/

#ifndef NFPROPERTY_HH_
#define NFPROPERTY_HH_

#include <vector>
#include <string>
#include <algorithm>

//Include the core NFsim objects
#include "../NFcore/NFcore.hh"

using namespace std;

namespace NFcore{
    class DiffusionClass;
    class EisteinStokesDiffusion;
    class ConstantDiffusion;


    class DiffusionClass: public GenericProperty{
        public:
            public DiffusionClassProperty(string key, string value): GenericProperty(key, value);
            virtual void getValue(string& value);
            virtual double getDiffusion(map<string, double> params);
    };

    class ConstantDiffusionProperty:public DiffusionClass{
        public:
            public ConstantDiffusionProperty(string key, string value): DiffusionClass(key, value);
            virtual void getValue(string& value);
            virtual void getDiffusion(map<string, double params);
    };


    class EinsteinStokes: public DiffussionClassProperty{
        public:
            public EinsteinStokes(string key, string value): DiffusionClass(key, value);
            virtual void getValue(string& value);
            virtual void getDiffusion(map<string, double params);
    };
}

#endif

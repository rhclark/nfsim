#ifndef NFPROPERTY_HH_
#define NFPROPERTY_HH_

#include <vector>
#include <string>
#include <algorithm>
#include <map>
//Include the core NFsim objects
#include "../NFcore/NFcore.hh"

using namespace std;

namespace NFcore{
    class DiffusionClass;
    class EisteinStokesDiffusion;
    class ConstantDiffusion;


    class DiffusionClass: public GenericProperty{
        public:
            DiffusionClass(string key, string value): GenericProperty(key, value) {};
            virtual void getValue(string& value);
            virtual double getDiffusion();
    };

    class ConstantDiffusion:public DiffusionClass{
        public:
            ConstantDiffusion(string key, string value): DiffusionClass(key, value) {};
            virtual void getValue(string& value);
            virtual double getDiffusion();
    };


    class EinsteinStokes: public DiffusionClass{
        public:
            EinsteinStokes(string key, string value): DiffusionClass(key, value) {};
            virtual void getValue(string& value);
            virtual double getDiffusion();
    };
}

#endif

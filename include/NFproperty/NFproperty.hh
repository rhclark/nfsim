#ifndef NFPROPERTY_HH_
#define NFPROPERTY_HH_

#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "../NFcore/NFcore.hh"

using namespace std;

namespace NFcore{
    class DiffusionClass;
    class EisteinStokesDiffusion;
    class SaffmanDelbruck;
    class ConstantDiffusion;

    class DiffusionClass: public GenericProperty{
        public:
            DiffusionClass(string key, string value): GenericProperty(key, value) {};
            virtual void getValue(string& value);
            virtual double getDiffusionValue();

        protected:
            map<string, double> parameters;
    };

    class ConstantDiffusion:public DiffusionClass{
        public:
            ConstantDiffusion(string key, string value): DiffusionClass(key, value) {};
            virtual void getValue(string& value);
            virtual double getDiffusionValue();
    };


    class EinsteinStokes: public DiffusionClass{
        public:
            EinsteinStokes(string key, string value): DiffusionClass(key, value) {};
            virtual void getValue(string& value);
            virtual double getDiffusionValue();
    };

    class SaffmanDelbruck: public DiffusionClass{
    public:
        SaffmanDelbruck(string key, string value): DiffusionClass(key, value) {};
        virtual double getDiffusionValue();
        virtual void getValue(string&);
        double getViscositySurroundingVolume();
    };

}

#endif
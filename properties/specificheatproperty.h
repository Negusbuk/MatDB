#ifndef SPECIFICHEATPROPERTY_H
#define SPECIFICHEATPROPERTY_H

#include <property.h>

class SpecificHeatProperty : public Property
{
public:
    SpecificHeatProperty(ParameterModel* model, int id);
    SpecificHeatProperty(const SpecificHeatProperty&);

    Property* clone(ParameterModel* model = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);
};

#endif // SPECIFICHEATPROPERTY_H

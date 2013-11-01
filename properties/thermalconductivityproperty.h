#ifndef THERMALCONDUCTIVITYPROPERTY_H
#define THERMALCONDUCTIVITYPROPERTY_H

#include <property.h>

class IsotropicThermalConductivityProperty : public Property
{
public:
    IsotropicThermalConductivityProperty(ParameterModel* model, int id);
    IsotropicThermalConductivityProperty(const IsotropicThermalConductivityProperty&);

    Property* clone(ParameterModel* model = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
};

class OrthotropicThermalConductivityProperty : public Property
{
public:
    OrthotropicThermalConductivityProperty(ParameterModel* model, int id);
    OrthotropicThermalConductivityProperty(const OrthotropicThermalConductivityProperty&);

    Property* clone(ParameterModel* model = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
};

#endif // THERMALCONDUCTIVITYPROPERTY_H

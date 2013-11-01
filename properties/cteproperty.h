#ifndef CTEPROPERTY_H
#define CTEPROPERTY_H

#include <property.h>

class IsotropicCoefficientOfThermalExpansionProperty : public Property
{
public:
    IsotropicCoefficientOfThermalExpansionProperty(ParameterModel* model, int id);
    IsotropicCoefficientOfThermalExpansionProperty(const IsotropicCoefficientOfThermalExpansionProperty&);

    Property* clone(ParameterModel* model = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
};

class OrthotropicCoefficientOfThermalExpansionProperty : public Property
{
public:
    OrthotropicCoefficientOfThermalExpansionProperty(ParameterModel* model, int id);
    OrthotropicCoefficientOfThermalExpansionProperty(const OrthotropicCoefficientOfThermalExpansionProperty&);

    Property* clone(ParameterModel* model);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
};

#endif // CTEPROPERTY_H

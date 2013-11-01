#ifndef RESISTIVITYPROPERTY_H
#define RESISTIVITYPROPERTY_H

#include <property.h>

class IsotropicResistivityProperty : public Property
{
public:
    IsotropicResistivityProperty(ParameterModel* model, int id);
    IsotropicResistivityProperty(const IsotropicResistivityProperty&);

    Property* clone(ParameterModel* model = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
};

class OrthotropicResistivityProperty : public Property
{
public:
    OrthotropicResistivityProperty(ParameterModel* model, int id);
    OrthotropicResistivityProperty(const OrthotropicResistivityProperty&);

    Property* clone(ParameterModel* model = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
};

#endif // RESISTIVITYPROPERTY_H

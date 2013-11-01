#ifndef DENSITYPROPERTY_H
#define DENSITYPROPERTY_H

#include <property.h>

class DensityProperty : public Property
{
public:
    DensityProperty(ParameterModel* model, int id);
    DensityProperty(const DensityProperty&);

    virtual Property* clone(ParameterModel* model = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
    virtual void writeXMLData(QXmlStreamWriter& stream);
};

#endif // DENSITYPROPERTY_H

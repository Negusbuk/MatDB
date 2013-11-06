#ifndef VISCOSITYPROPERTY_H
#define VISCOSITYPROPERTY_H

#include <property.h>

class ViscosityProperty : public Property
{
public:
    ViscosityProperty(ParameterModel* model, int id);
    ViscosityProperty(const ViscosityProperty&);

    virtual Property* clone(ParameterModel* model = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
    virtual void writeXMLData(QXmlStreamWriter& stream);
};

#endif // ViscosityPROPERTY_H

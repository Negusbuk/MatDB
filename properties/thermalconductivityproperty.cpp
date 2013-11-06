#include <iostream>
#include <cmath>

#include <QRadioButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

#include "thermalconductivityproperty.h"

IsotropicThermalConductivityProperty::IsotropicThermalConductivityProperty(ParameterModel* model, int id) :
    Property(id)
{
    setName("Isotropic Thermal Conductivity");
    setCategory(ThermalProperty);
    setType(ThermalConductivity);
    setBehavior(Isotropic);
    Parameter *par = model->getParameter("Thermal Conductivity");
    addParameter(par->clone());
}

IsotropicThermalConductivityProperty::IsotropicThermalConductivityProperty(const IsotropicThermalConductivityProperty& property) :
    Property(property.getId())
{
    setName("Isotropic Thermal Conductivity");
    setCategory(ThermalProperty);
    setType(ThermalConductivity);
    setBehavior(Isotropic);
    const Parameter *par = property.getParameter("Thermal Conductivity");
    addParameter(par->clone());
}

Property* IsotropicThermalConductivityProperty::clone(ParameterModel* model)
{
    IsotropicThermalConductivityProperty* prop;
    if (model) {
        prop = new IsotropicThermalConductivityProperty(model, getId());
    } else {
        prop = new IsotropicThermalConductivityProperty(*this);
    }

    prop->setSorting(getSorting());

    return prop;
}

//<PropertyData property="pr1">
//  <Data format="string">-</Data>
//  <Qualifier name="Behavior">Isotropic</Qualifier>
//  <ParameterValue parameter="pa1" format="float">
//    <Data>3.6e-05</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa0" format="float">
//    <Data>7.88860905221012e-31</Data>
//    <Qualifier name="Variable Type">Independent</Qualifier>
//  </ParameterValue>
//</PropertyData>
void IsotropicThermalConductivityProperty::apply(PropertyData& data,
                                                 PropertyDetail& detail,
                                                 std::map<QString,ParameterDetail> paramMap)
{
    //std::cout << "IsotropicThermalConductivityProperty::apply" << std::endl;

    PValue pv = data.pvalues.front();
    std::vector<double> values;
    PValue pt = data.pvalues.back();
    std::vector<double> temperatures;

    if (pv.data.contains(',')) {
        QStringList l = pv.data.split(',');
        for (QStringList::Iterator it = l.begin();
             it!=l.end();
             ++it) {
            values.push_back((*it).toDouble());
        }
    } else {
        values.push_back(pv.data.toDouble());
    }

    if (pt.data.contains(',')) {
        QStringList l = pt.data.split(',');
        for (QStringList::Iterator it = l.begin();
             it!=l.end();
             ++it) {
            temperatures.push_back((*it).toDouble());
        }
    } else {
        temperatures.push_back(pt.data.toDouble());
    }

    ParameterDetail paramDetail = paramMap[pv.parameter];

    Parameter * param = getParameter(paramDetail.name);
    param->setValueUnit(paramDetail.unit);

    std::vector<double>::iterator itt = temperatures.begin();
    for (std::vector<double>::iterator itv = values.begin();
         itv!=values.end() && itt!=temperatures.end();
         ++itv,++itt) {
        double temp = *itt;
        double value = *itv;
        if (value!=undefindedIdentifyer()) {
            value = param->getValueUnit()->convertToPreffered(*itv);
            if (temp==undefindedIdentifyer()) {
                param->addValue(value);
            } else {
                param->addValue(temp, value);
            }
        }
    }

    param->setPrefferedValueUnit();
}

void IsotropicThermalConductivityProperty::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Thermal Conductivity");
    stream.writeEndElement();
}

OrthotropicThermalConductivityProperty::OrthotropicThermalConductivityProperty(ParameterModel* model, int id) :
    Property(id)
{
    setName("Orthotropic Thermal Conductivity");
    setCategory(ThermalProperty);
    setType(ThermalConductivity);
    setBehavior(Orthotropic);
    Parameter *par;
    par = model->getParameter("Thermal Conductivity X direction");
    addParameter(par->clone());
    par = model->getParameter("Thermal Conductivity Y direction");
    addParameter(par->clone());
    par = model->getParameter("Thermal Conductivity Z direction");
    addParameter(par->clone());
}

OrthotropicThermalConductivityProperty::OrthotropicThermalConductivityProperty(const OrthotropicThermalConductivityProperty& property) :
    Property(property.getId())
{
    setName("Orthotropic Thermal Conductivity");
    setCategory(ThermalProperty);
    setType(ThermalConductivity);
    setBehavior(Orthotropic);
    const Parameter *par1 = property.getParameter("Thermal Conductivity X direction");
    addParameter(par1->clone());
    const Parameter *par2 = property.getParameter("Thermal Conductivity Y direction");
    addParameter(par2->clone());
    const Parameter *par3 = property.getParameter("Thermal Conductivity Z direction");
    addParameter(par3->clone());
}

Property* OrthotropicThermalConductivityProperty::clone(ParameterModel* model)
{
    OrthotropicThermalConductivityProperty* prop;
    if (model) {
        prop = new OrthotropicThermalConductivityProperty(model, getId());
    } else {
        prop = new OrthotropicThermalConductivityProperty(*this);
    }

    prop->setSorting(getSorting());

    return prop;
}

//<PropertyData property="pr1">
//  <Data format="string">-</Data>
//  <Qualifier name="Behavior">Orthotropic</Qualifier>
//  <ParameterValue parameter="pa2" format="float">
//    <Data>0.23</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa3" format="float">
//    <Data>0.23</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa4" format="float">
//    <Data>0.12</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa0" format="float">
//    <Data>7.88860905221012e-31</Data>
//    <Qualifier name="Variable Type">Independent</Qualifier>
//  </ParameterValue>
//</PropertyData>
void OrthotropicThermalConductivityProperty::apply(PropertyData& data,
                                                   PropertyDetail& detail,
                                                   std::map<QString,ParameterDetail> paramMap)
{
    // std::cout << "OrthotropicThermalConductivityProperty::apply" << std::endl;

    ParameterDetail details[4];
    Parameter *param[4];
    std::vector<double> values[4];
    std::vector<double> vtt;

    int i=0;
    for (std::vector<PValue>::iterator it = data.pvalues.begin();
         it!=data.pvalues.end();
         ++it) {
        details[i] = paramMap[it->parameter];
        param[i] = getParameter(details[i].name);
        //param[i]->setId(details[i].id);

        if (it->data.contains(',')) {
            QStringList l = it->data.split(',');
            for (QStringList::Iterator itl = l.begin();
                 itl!=l.end();
                 ++itl) {
                values[i].push_back((*itl).toDouble());
            }
        } else {
            values[i].push_back(it->data.toDouble());
        }

        if (param[i]==0) vtt = values[i];

        ++i;
    }

    for (i=0;i<4;++i) {
        if (!param[i]) continue;

        param[i]->setValueUnit(details[i].unit);

        std::vector<double>::iterator itt = vtt.begin();
        for (std::vector<double>::iterator itv = values[i].begin();
             itv!=values[i].end() && itt!=vtt.end();
             ++itv,++itt) {
            double temp = *itt;
            double value = *itv;
            if (value!=undefindedIdentifyer()) {
                value = param[i]->getValueUnit()->convertToPreffered(*itv);
                if (temp==undefindedIdentifyer()) {
                    param[i]->addValue(value);
                } else {
                    param[i]->addValue(temp, value);
                }
            }
        }

        param[i]->setPrefferedValueUnit();
    }
}

void OrthotropicThermalConductivityProperty::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Thermal Conductivity");
    stream.writeEndElement();
}

#include <iostream>
#include <cmath>

#include <QRadioButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

#include "cteproperty.h"

IsotropicCoefficientOfThermalExpansionProperty::IsotropicCoefficientOfThermalExpansionProperty(ParameterModel* model,
                                                                                               int id) :
    Property(id)
{
    setName("Isotropic Instantaneous Coefficient of Thermal Expansion");
    setCategory(PhysicalProperty);
    setType(CoefficientOfThermalExpansion);
    setBehavior(Isotropic);
    setDefinition(Instantaneous);
    Parameter *par = model->getParameter("Coefficient of Thermal Expansion");
    addParameter(par->clone());
}

IsotropicCoefficientOfThermalExpansionProperty::IsotropicCoefficientOfThermalExpansionProperty(const IsotropicCoefficientOfThermalExpansionProperty& property) :
    Property(property.getId())
{
    setName("Isotropic Instantaneous Coefficient of Thermal Expansion");
    setCategory(PhysicalProperty);
    setType(CoefficientOfThermalExpansion);
    setBehavior(Isotropic);
    setDefinition(Instantaneous);
    const Parameter *par = property.getParameter("Coefficient of Thermal Expansion");
    addParameter(par->clone());
}

Property* IsotropicCoefficientOfThermalExpansionProperty::clone(ParameterModel* model)
{
    IsotropicCoefficientOfThermalExpansionProperty* prop;
    if (model) {
        prop = new IsotropicCoefficientOfThermalExpansionProperty(model, getId());
    } else {
        prop = new IsotropicCoefficientOfThermalExpansionProperty(*this);
    }

    prop->setSorting(getSorting());

    return prop;
}

//<PropertyData property="pr2">
//  <Data format="string">-</Data>
//  <Qualifier name="Definition">Instantaneous</Qualifier>
//  <Qualifier name="Behavior">Isotropic</Qualifier>
//  <ParameterValue parameter="pa3" format="float">
//    <Data>3e-06</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa0" format="float">
//    <Data>7.88860905221012e-31</Data>
//    <Qualifier name="Variable Type">Independent</Qualifier>
//  </ParameterValue>
//</PropertyData>
void IsotropicCoefficientOfThermalExpansionProperty::apply(PropertyData& data,
                                                           PropertyDetail& detail,
                                                           std::map<QString,ParameterDetail> paramMap)
{
    //std::cout << "IsotropicCoefficientOfThermalExpansionProperty::apply" << std::endl;

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

void IsotropicCoefficientOfThermalExpansionProperty::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Coefficient of Thermal Expansion");
    stream.writeEndElement();
}

OrthotropicCoefficientOfThermalExpansionProperty::OrthotropicCoefficientOfThermalExpansionProperty(ParameterModel* model,
                                                                                                   int id) :
    Property(id)
{
    setName("Orthotropic Instantaneous Coefficient of Thermal Expansion");
    setCategory(PhysicalProperty);
    setType(CoefficientOfThermalExpansion);
    setBehavior(Orthotropic);
    setDefinition(Instantaneous);
    Parameter *par;
    par = model->getParameter("Coefficient of Thermal Expansion X direction");
    addParameter(par->clone());
    par = model->getParameter("Coefficient of Thermal Expansion Y direction");
    addParameter(par->clone());
    par = model->getParameter("Coefficient of Thermal Expansion Z direction");
    addParameter(par->clone());
}

OrthotropicCoefficientOfThermalExpansionProperty::OrthotropicCoefficientOfThermalExpansionProperty(const OrthotropicCoefficientOfThermalExpansionProperty& property) :
    Property(property.getId())
{
    setName("Orthotropic Instantaneous Coefficient of Thermal Expansion");
    setCategory(PhysicalProperty);
    setType(CoefficientOfThermalExpansion);
    setBehavior(Orthotropic);
    setDefinition(Instantaneous);
    const Parameter *par1 = property.getParameter("Coefficient of Thermal Expansion X direction");
    addParameter(par1->clone());
    const Parameter *par2 = property.getParameter("Coefficient of Thermal Expansion Y direction");
    addParameter(par2->clone());
    const Parameter *par3 = property.getParameter("Coefficient of Thermal Expansion Z direction");
    addParameter(par3->clone());
}

Property* OrthotropicCoefficientOfThermalExpansionProperty::clone(ParameterModel* model)
{
    OrthotropicCoefficientOfThermalExpansionProperty* prop;
    if (model) {
        prop = new OrthotropicCoefficientOfThermalExpansionProperty(model, getId());
    } else {
        prop = new OrthotropicCoefficientOfThermalExpansionProperty(*this);
    }

    prop->setSorting(getSorting());

    return prop;
}

//<PropertyData property="pr2">
//  <Data format="string">-</Data>
//  <Qualifier name="Definition">Secant</Qualifier>
//  <Qualifier name="Behavior">Orthotropic</Qualifier>
//  <ParameterValue parameter="pa5" format="float">
//    <Data>4e-06</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa6" format="float">
//    <Data>4e-06</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa7" format="float">
//    <Data>2.4e-05</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa0" format="float">
//    <Data>7.88860905221012e-31</Data>
//    <Qualifier name="Variable Type">Independent</Qualifier>
//  </ParameterValue>
//</PropertyData>
void OrthotropicCoefficientOfThermalExpansionProperty::apply(PropertyData& data,
                                                             PropertyDetail& detail,
                                                             std::map<QString,ParameterDetail> paramMap)
{
    //std::cout << "OrthotropicCoefficientOfThermalExpansionProperty::apply" << std::endl;

    ParameterDetail details[4];
    Parameter *param[4];
    std::vector<double> values[4];
    std::vector<double> vtt;

    int i=0;
    for (std::vector<PValue>::iterator it = data.pvalues.begin();
         it!=data.pvalues.end();
         ++it) {
        details[i] = paramMap[it->parameter];
        //std::cout << details[i].name.toStdString() << std::endl;
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

void OrthotropicCoefficientOfThermalExpansionProperty::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Coefficient of Thermal Expansion");
    stream.writeEndElement();
}

IsotropicSecantCoefficientOfThermalExpansionProperty::IsotropicSecantCoefficientOfThermalExpansionProperty(ParameterModel* model,
                                                                                               int id) :
    Property(id)
{
    setName("Isotropic Secant Coefficient of Thermal Expansion");
    setCategory(PhysicalProperty);
    setType(CoefficientOfThermalExpansion);
    setBehavior(Isotropic);
    setDefinition(Secant);
    Parameter *par;
    par = model->getParameter("Coefficient of Thermal Expansion");
    addParameter(par->clone());
    par = model->getParameter("Reference Temperature");
    addParameter(par->clone());
}

IsotropicSecantCoefficientOfThermalExpansionProperty::IsotropicSecantCoefficientOfThermalExpansionProperty(const IsotropicSecantCoefficientOfThermalExpansionProperty& property) :
    Property(property.getId())
{
    setName("Isotropic Secant Coefficient of Thermal Expansion");
    setCategory(PhysicalProperty);
    setType(CoefficientOfThermalExpansion);
    setBehavior(Isotropic);
    setDefinition(Secant);
    const Parameter *par1 = property.getParameter("Coefficient of Thermal Expansion");
    addParameter(par1->clone());
    const Parameter *par2 = property.getParameter("Reference Temperature");
    addParameter(par2->clone());
}

Property* IsotropicSecantCoefficientOfThermalExpansionProperty::clone(ParameterModel* model)
{
    IsotropicSecantCoefficientOfThermalExpansionProperty* prop;
    if (model) {
        prop = new IsotropicSecantCoefficientOfThermalExpansionProperty(model, getId());
    } else {
        prop = new IsotropicSecantCoefficientOfThermalExpansionProperty(*this);
    }

    prop->setSorting(getSorting());

    return prop;
}

//<PropertyData property="pr2">
//  <Data format="string">-</Data>
//  <Qualifier name="Definition">Instantaneous</Qualifier>
//  <Qualifier name="Behavior">Isotropic</Qualifier>
//  <ParameterValue parameter="pa3" format="float">
//    <Data>3e-06</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa0" format="float">
//    <Data>7.88860905221012e-31</Data>
//    <Qualifier name="Variable Type">Independent</Qualifier>
//  </ParameterValue>
//</PropertyData>
void IsotropicSecantCoefficientOfThermalExpansionProperty::apply(PropertyData& data,
                                                           PropertyDetail& detail,
                                                           std::map<QString,ParameterDetail> paramMap)
{
    //std::cout << "IsotropicSecantCoefficientOfThermalExpansionProperty::apply" << std::endl;

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

void IsotropicSecantCoefficientOfThermalExpansionProperty::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Coefficient of Thermal Expansion");
    stream.writeEndElement();
}

OrthotropicSecantCoefficientOfThermalExpansionProperty::OrthotropicSecantCoefficientOfThermalExpansionProperty(ParameterModel* model,
                                                                                                   int id) :
    Property(id)
{
    setName("Orthotropic Secant Coefficient of Thermal Expansion");
    setCategory(PhysicalProperty);
    setType(CoefficientOfThermalExpansion);
    setBehavior(Orthotropic);
    setDefinition(Secant);
    Parameter *par;
    par = model->getParameter("Coefficient of Thermal Expansion X direction");
    addParameter(par->clone());
    par = model->getParameter("Coefficient of Thermal Expansion Y direction");
    addParameter(par->clone());
    par = model->getParameter("Coefficient of Thermal Expansion Z direction");
    addParameter(par->clone());
    par = model->getParameter("Reference Temperature");
    addParameter(par->clone());
}

OrthotropicSecantCoefficientOfThermalExpansionProperty::OrthotropicSecantCoefficientOfThermalExpansionProperty(const OrthotropicSecantCoefficientOfThermalExpansionProperty& property) :
    Property(property.getId())
{
    setName("Orthotropic Secant Coefficient of Thermal Expansion");
    setCategory(PhysicalProperty);
    setType(CoefficientOfThermalExpansion);
    setBehavior(Orthotropic);
    setDefinition(Secant);
    const Parameter *par1 = property.getParameter("Coefficient of Thermal Expansion X direction");
    addParameter(par1->clone());
    const Parameter *par2 = property.getParameter("Coefficient of Thermal Expansion Y direction");
    addParameter(par2->clone());
    const Parameter *par3 = property.getParameter("Coefficient of Thermal Expansion Z direction");
    addParameter(par3->clone());
    const Parameter *par4 = property.getParameter("Reference Temperature");
    addParameter(par4->clone());
}

Property* OrthotropicSecantCoefficientOfThermalExpansionProperty::clone(ParameterModel* model)
{
    OrthotropicSecantCoefficientOfThermalExpansionProperty* prop;
    if (model) {
        prop = new OrthotropicSecantCoefficientOfThermalExpansionProperty(model, getId());
    } else {
        prop = new OrthotropicSecantCoefficientOfThermalExpansionProperty(*this);
    }

    prop->setSorting(getSorting());

    return prop;
}

//<PropertyData property="pr2">
//  <Data format="string">-</Data>
//  <Qualifier name="Definition">Secant</Qualifier>
//  <Qualifier name="Behavior">Orthotropic</Qualifier>
//  <ParameterValue parameter="pa5" format="float">
//    <Data>4e-06</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa6" format="float">
//    <Data>4e-06</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa7" format="float">
//    <Data>2.4e-05</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa0" format="float">
//    <Data>7.88860905221012e-31</Data>
//    <Qualifier name="Variable Type">Independent</Qualifier>
//  </ParameterValue>
//</PropertyData>
void OrthotropicSecantCoefficientOfThermalExpansionProperty::apply(PropertyData& data,
                                                             PropertyDetail& detail,
                                                             std::map<QString,ParameterDetail> paramMap)
{
    //std::cout << "OrthotropicSecantCoefficientOfThermalExpansionProperty::apply" << std::endl;

    ParameterDetail details[4];
    Parameter *param[4];
    std::vector<double> values[4];
    std::vector<double> vtt;

    int i=0;
    for (std::vector<PValue>::iterator it = data.pvalues.begin();
         it!=data.pvalues.end();
         ++it) {
        details[i] = paramMap[it->parameter];
        //std::cout << details[i].name.toStdString() << std::endl;
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

void OrthotropicSecantCoefficientOfThermalExpansionProperty::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Coefficient of Thermal Expansion");
    stream.writeEndElement();
}

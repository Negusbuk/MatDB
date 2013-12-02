/****************************************************************************
 **
 **  Copyright (C) 2013 Andreas Mussgiller
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **
 **
 ****************************************************************************/

#include <cmath>

#include <QRadioButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

#include <nqlogger.h>

#include <cteproperty.h>

IsotropicInstantaneousCoefficientOfThermalExpansion::IsotropicInstantaneousCoefficientOfThermalExpansion(PropertyModel* propmodel,
                                                                                                         ParameterModel* paramodel,
                                                                                               int id) :
    Property(id)
{
    setName("Isotropic Instantaneous Coefficient of Thermal Expansion");
    setDisplayName(QObject::tr("Isotropic Instantaneous Coefficient of Thermal Expansion"));
    setCategory(PhysicalProperty);
    setType(CoefficientOfThermalExpansion);
    setBehavior(Isotropic);
    setDefinition(Instantaneous);
    Parameter *par = paramodel->getParameter("Coefficient of Thermal Expansion");
    addParameter(par->clone());
}

IsotropicInstantaneousCoefficientOfThermalExpansion::IsotropicInstantaneousCoefficientOfThermalExpansion(const IsotropicInstantaneousCoefficientOfThermalExpansion& property) :
    Property(property.getId())
{
    setName("Isotropic Instantaneous Coefficient of Thermal Expansion");
    setDisplayName(QObject::tr("Isotropic Instantaneous Coefficient of Thermal Expansion"));
    setCategory(PhysicalProperty);
    setType(CoefficientOfThermalExpansion);
    setBehavior(Isotropic);
    setDefinition(Instantaneous);
    const Parameter *par = property.getParameter("Coefficient of Thermal Expansion");
    addParameter(par->clone());
}

Property* IsotropicInstantaneousCoefficientOfThermalExpansion::clone(PropertyModel *propmodel,
                                                                     ParameterModel* paramodel)
{
    IsotropicInstantaneousCoefficientOfThermalExpansion* prop;
    if (propmodel && paramodel) {
        prop = new IsotropicInstantaneousCoefficientOfThermalExpansion(propmodel, paramodel, getId());
    } else {
        prop = new IsotropicInstantaneousCoefficientOfThermalExpansion(*this);
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
void IsotropicInstantaneousCoefficientOfThermalExpansion::apply(PropertyData& data,
                                                           PropertyDetail& /* detail */,
                                                           std::map<QString,ParameterDetail> paramMap)
{
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

void IsotropicInstantaneousCoefficientOfThermalExpansion::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Coefficient of Thermal Expansion");
    stream.writeEndElement();
}

OrthotropicInstantaneousCoefficientOfThermalExpansion::OrthotropicInstantaneousCoefficientOfThermalExpansion(PropertyModel *propmodel,
                                                                                                             ParameterModel* paramodel,
                                                                                                   int id) :
    Property(id)
{
    setName("Orthotropic Instantaneous Coefficient of Thermal Expansion");
    setDisplayName(QObject::tr("Orthotropic Instantaneous Coefficient of Thermal Expansion"));
    setCategory(PhysicalProperty);
    setType(CoefficientOfThermalExpansion);
    setBehavior(Orthotropic);
    setDefinition(Instantaneous);
    Parameter *par;
    par = paramodel->getParameter("Coefficient of Thermal Expansion X direction");
    addParameter(par->clone());
    par = paramodel->getParameter("Coefficient of Thermal Expansion Y direction");
    addParameter(par->clone());
    par = paramodel->getParameter("Coefficient of Thermal Expansion Z direction");
    addParameter(par->clone());
}

OrthotropicInstantaneousCoefficientOfThermalExpansion::OrthotropicInstantaneousCoefficientOfThermalExpansion(const OrthotropicInstantaneousCoefficientOfThermalExpansion& property) :
    Property(property.getId())
{
    setName("Orthotropic Instantaneous Coefficient of Thermal Expansion");
    setDisplayName(QObject::tr("Orthotropic Instantaneous Coefficient of Thermal Expansion"));
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

Property* OrthotropicInstantaneousCoefficientOfThermalExpansion::clone(PropertyModel *propmodel,
                                                                       ParameterModel* paramodel)
{
    OrthotropicInstantaneousCoefficientOfThermalExpansion* prop;
    if (propmodel && paramodel) {
        prop = new OrthotropicInstantaneousCoefficientOfThermalExpansion(propmodel, paramodel, getId());
    } else {
        prop = new OrthotropicInstantaneousCoefficientOfThermalExpansion(*this);
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
void OrthotropicInstantaneousCoefficientOfThermalExpansion::apply(PropertyData& data,
                                                             PropertyDetail& /* detail */,
                                                             std::map<QString,ParameterDetail> paramMap)
{
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

void OrthotropicInstantaneousCoefficientOfThermalExpansion::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Coefficient of Thermal Expansion");
    stream.writeEndElement();
}

void OrthotropicInstantaneousCoefficientOfThermalExpansion::writeHTML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("tr");

    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBTitle");
    stream.writeAttribute("valign", "top");
    stream.writeCharacters(getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("colspan", "2");
    stream.writeAttribute("align", "right");

    stream.writeStartElement("table");
    //stream.writeAttribute("style", "border-top:1px solid #000;");
    stream.writeAttribute("rules", "groups");
    //stream.writeAttribute("class", "MatDBTable");

    Parameter* parameter;

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    parameter = getParameter("Coefficient of Thermal Expansion X direction");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters("CTE X");
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    parameter->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    parameter = getParameter("Coefficient of Thermal Expansion Y direction");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters("CTE Y");
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    parameter->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    parameter = getParameter("Coefficient of Thermal Expansion Z direction");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters("CTE Z");
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    parameter->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeEndElement(); // table

    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
}

IsotropicSecantCoefficientOfThermalExpansion::IsotropicSecantCoefficientOfThermalExpansion(PropertyModel* propmodel,
                                                                                           ParameterModel* paramodel,
                                                                                           int id) :
    Property(id)
{
    setName("Isotropic Secant Coefficient of Thermal Expansion");
    setDisplayName(QObject::tr("Isotropic Secant Coefficient of Thermal Expansion"));
    setCategory(PhysicalProperty);
    setType(CoefficientOfThermalExpansion);
    setBehavior(Isotropic);
    setDefinition(Secant);
    Parameter *par;
    par = paramodel->getParameter("Coefficient of Thermal Expansion");
    addParameter(par->clone());
    referenceTemperatureProperty_ = dynamic_cast<ReferenceTemperatureProperty*>(propmodel->getProperty("Reference Temperature"));
    referenceTemperatureProperty_->setBehavior(Isotropic);
    referenceTemperatureProperty_->setDefinition(Secant);
    referenceTemperatureProperty_->setMaterialProperty("Coefficient of Thermal Expansion");
    par = referenceTemperatureProperty_->getParameter("Reference Temperature");
    addParameter(par);
}

IsotropicSecantCoefficientOfThermalExpansion::IsotropicSecantCoefficientOfThermalExpansion(const IsotropicSecantCoefficientOfThermalExpansion& property) :
    Property(property.getId())
{
    setName("Isotropic Secant Coefficient of Thermal Expansion");
    setDisplayName(QObject::tr("Isotropic Secant Coefficient of Thermal Expansion"));
    setCategory(PhysicalProperty);
    setType(CoefficientOfThermalExpansion);
    setBehavior(Isotropic);
    setDefinition(Secant);
    const Parameter *par1 = property.getParameter("Coefficient of Thermal Expansion");
    addParameter(par1->clone());
    referenceTemperatureProperty_ = dynamic_cast<ReferenceTemperatureProperty*>(property.referenceTemperatureProperty_->clone());
    referenceTemperatureProperty_->setBehavior(Isotropic);
    referenceTemperatureProperty_->setDefinition(Secant);
    referenceTemperatureProperty_->setMaterialProperty("Coefficient of Thermal Expansion");
    addParameter(referenceTemperatureProperty_->getParameter("Reference Temperature"));
}

Property* IsotropicSecantCoefficientOfThermalExpansion::clone(PropertyModel *propmodel,
                                                              ParameterModel* paramodel)
{
    IsotropicSecantCoefficientOfThermalExpansion* prop;
    if (propmodel && paramodel) {
        prop = new IsotropicSecantCoefficientOfThermalExpansion(propmodel, paramodel, getId());
    } else {
        prop = new IsotropicSecantCoefficientOfThermalExpansion(*this);
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
void IsotropicSecantCoefficientOfThermalExpansion::apply(PropertyData& data,
                                                                 PropertyDetail& /* detail */,
                                                                 std::map<QString,ParameterDetail> paramMap)
{
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

void IsotropicSecantCoefficientOfThermalExpansion::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Coefficient of Thermal Expansion");
    stream.writeEndElement();
}

void IsotropicSecantCoefficientOfThermalExpansion::writeXMLData(QXmlStreamWriter& stream)
{
    NQLog("IsotropicSecantCoefficientOfThermalExpansion", NQLog::Spam) << "  XML write data for property " << getName()
                                   << " (" << getIdString().toStdString() << ")";

    stream.writeStartElement("PropertyData");
    stream.writeAttribute("property", getIdString());

    stream.writeStartElement("Data");
    stream.writeAttribute("format", "string");
    stream.writeCharacters("-");
    stream.writeEndElement(); // Data

    if (Definition_!=UnknownDefinition) {
        stream.writeStartElement("Qualifier");
        stream.writeAttribute("name", "Definition");
        stream.writeCharacters(getDefinitionAsString());
        stream.writeEndElement(); // Qualifier
    }

    if (Behavior_!=UnknownBehavior) {
        stream.writeStartElement("Qualifier");
        stream.writeAttribute("name", "Behavior");
        stream.writeCharacters(getBehaviorAsString());
        stream.writeEndElement(); // Qualifier
    }

    Parameter * parameter = getParameter("Coefficient of Thermal Expansion");
    Unit::VUnit * vunit = parameter->getValueUnit();

    stream.writeStartElement("ParameterValue");
    stream.writeAttribute("parameter", parameter->getIdString());
    stream.writeAttribute("format", "float");

    QString values;
    for (std::vector<ParameterValue>::const_iterator it=parameter->getValues().begin();
         it!=parameter->getValues().end();
         ++it) {

        const ParameterValue& pv = *it;
        if (it!=parameter->getValues().begin()) values += ",";
        if (pv.isValueValid()) {
            if (vunit->hasXMLExportUnit()) {
                values += QString::number(vunit->convertToXMLExport(pv.getValue()), 'e', 6);
            } else {
                values += QString::number(pv.getValue(), 'e', 6);
            }
        } else {
            values += undefindedIdentifyerAsString();
        }
    }
    if (parameter->getValues().size()==0) values = undefindedIdentifyerAsString();
    stream.writeTextElement("Data", values);

    stream.writeStartElement("Qualifier");
    stream.writeAttribute("name", "Variable Type");
    stream.writeCharacters("Dependent");
    stream.writeEndElement(); // Qualifier

    stream.writeEndElement(); // ParameterValue

    stream.writeStartElement("ParameterValue");
    stream.writeAttribute("parameter", "pa0");
    stream.writeAttribute("format", "float");

    values = "";
    for (std::vector<ParameterValue>::const_iterator it=parameter->getValues().begin();
         it!=parameter->getValues().end();
         ++it) {

        const ParameterValue& pv = *it;
        if (it!=parameter->getValues().begin()) values += ",";
        if (pv.isTemperatureValid()) {
            values += QString::number(pv.getTemperature(), 'e', 6);
        } else {
            values += undefindedIdentifyerAsString();
        }
    }
    if (parameter->getValues().size()==0) values = undefindedIdentifyerAsString();
    stream.writeTextElement("Data", values);

    stream.writeStartElement("Qualifier");
    stream.writeAttribute("name", "Variable Type");
    stream.writeCharacters("Independent");
    stream.writeEndElement(); // Qualifier

    stream.writeEndElement(); // ParameterValue

    stream.writeEndElement(); // PropertyData

    referenceTemperatureProperty_->writeXMLData(stream);
}

void IsotropicSecantCoefficientOfThermalExpansion::writeHTML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("tr");

    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBTitle");
    stream.writeAttribute("valign", "top");
    stream.writeCharacters(getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("colspan", "2");
    stream.writeAttribute("align", "right");

    stream.writeStartElement("table");
    //stream.writeAttribute("style", "border-top:1px solid #000;");
    stream.writeAttribute("rules", "groups");
    //stream.writeAttribute("class", "MatDBTable");

    Parameter* parameter;

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    parameter = getParameter("Coefficient of Thermal Expansion");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters("CTE");
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    parameter->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    parameter = getParameter("Reference Temperature");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters(parameter->getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    parameter->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeEndElement(); // table

    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
}

OrthotropicSecantCoefficientOfThermalExpansion::OrthotropicSecantCoefficientOfThermalExpansion(PropertyModel *propmodel,
                                                                                               ParameterModel* paramodel,
                                                                                                   int id) :
    Property(id)
{
    setName("Orthotropic Secant Coefficient of Thermal Expansion");
    setDisplayName(QObject::tr("Orthotropic Secant Coefficient of Thermal Expansion"));
    setCategory(PhysicalProperty);
    setType(CoefficientOfThermalExpansion);
    setBehavior(Orthotropic);
    setDefinition(Secant);
    Parameter *par;
    par = paramodel->getParameter("Coefficient of Thermal Expansion X direction");
    addParameter(par->clone());
    par = paramodel->getParameter("Coefficient of Thermal Expansion Y direction");
    addParameter(par->clone());
    par = paramodel->getParameter("Coefficient of Thermal Expansion Z direction");
    addParameter(par->clone());
    referenceTemperatureProperty_ = dynamic_cast<ReferenceTemperatureProperty*>(propmodel->getProperty("Reference Temperature"));
    referenceTemperatureProperty_->setBehavior(Isotropic);
    referenceTemperatureProperty_->setDefinition(Secant);
    referenceTemperatureProperty_->setMaterialProperty("Coefficient of Thermal Expansion");
    par = referenceTemperatureProperty_->getParameter("Reference Temperature");
    addParameter(par);
}

OrthotropicSecantCoefficientOfThermalExpansion::OrthotropicSecantCoefficientOfThermalExpansion(const OrthotropicSecantCoefficientOfThermalExpansion& property) :
    Property(property.getId())
{
    setName("Orthotropic Secant Coefficient of Thermal Expansion");
    setDisplayName(QObject::tr("Orthotropic Secant Coefficient of Thermal Expansion"));
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
    referenceTemperatureProperty_ = dynamic_cast<ReferenceTemperatureProperty*>(property.referenceTemperatureProperty_->clone());
    referenceTemperatureProperty_->setBehavior(Isotropic);
    referenceTemperatureProperty_->setDefinition(Secant);
    referenceTemperatureProperty_->setMaterialProperty("Coefficient of Thermal Expansion");
    addParameter(referenceTemperatureProperty_->getParameter("Reference Temperature"));
}

Property* OrthotropicSecantCoefficientOfThermalExpansion::clone(PropertyModel *propmodel,
                                                                ParameterModel* paramodel)
{
    OrthotropicSecantCoefficientOfThermalExpansion* prop;
    if (propmodel && paramodel) {
        prop = new OrthotropicSecantCoefficientOfThermalExpansion(propmodel, paramodel, getId());
    } else {
        prop = new OrthotropicSecantCoefficientOfThermalExpansion(*this);
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
void OrthotropicSecantCoefficientOfThermalExpansion::apply(PropertyData& data,
                                                                   PropertyDetail& /* detail */,
                                                                   std::map<QString,ParameterDetail> paramMap)
{
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

void OrthotropicSecantCoefficientOfThermalExpansion::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Coefficient of Thermal Expansion");
    stream.writeEndElement();
}

void OrthotropicSecantCoefficientOfThermalExpansion::writeXMLData(QXmlStreamWriter& stream)
{
    NQLog("OrthotropicSecantCoefficientOfThermalExpansion", NQLog::Spam) << "  XML write data for property " << getName()
                                   << " (" << getIdString().toStdString() << ")";

    stream.writeStartElement("PropertyData");
    stream.writeAttribute("property", getIdString());

    stream.writeStartElement("Data");
    stream.writeAttribute("format", "string");
    stream.writeCharacters("-");
    stream.writeEndElement(); // Data

    if (Definition_!=UnknownDefinition) {
        stream.writeStartElement("Qualifier");
        stream.writeAttribute("name", "Definition");
        stream.writeCharacters(getDefinitionAsString());
        stream.writeEndElement(); // Qualifier
    }

    if (Behavior_!=UnknownBehavior) {
        stream.writeStartElement("Qualifier");
        stream.writeAttribute("name", "Behavior");
        stream.writeCharacters(getBehaviorAsString());
        stream.writeEndElement(); // Qualifier
    }

    Parameter * parameter = getParameter("Coefficient of Thermal Expansion");
    Unit::VUnit * vunit = parameter->getValueUnit();

    stream.writeStartElement("ParameterValue");
    stream.writeAttribute("parameter", parameter->getIdString());
    stream.writeAttribute("format", "float");

    QString values;
    for (std::vector<ParameterValue>::const_iterator it=parameter->getValues().begin();
         it!=parameter->getValues().end();
         ++it) {

        const ParameterValue& pv = *it;
        if (it!=parameter->getValues().begin()) values += ",";
        if (pv.isValueValid()) {
            if (vunit->hasXMLExportUnit()) {
                values += QString::number(vunit->convertToXMLExport(pv.getValue()), 'e', 6);
            } else {
                values += QString::number(pv.getValue(), 'e', 6);
            }
        } else {
            values += undefindedIdentifyerAsString();
        }
    }
    if (parameter->getValues().size()==0) values = undefindedIdentifyerAsString();
    stream.writeTextElement("Data", values);

    stream.writeStartElement("Qualifier");
    stream.writeAttribute("name", "Variable Type");
    stream.writeCharacters("Dependent");
    stream.writeEndElement(); // Qualifier

    stream.writeEndElement(); // ParameterValue

    stream.writeStartElement("ParameterValue");
    stream.writeAttribute("parameter", "pa0");
    stream.writeAttribute("format", "float");

    values = "";
    for (std::vector<ParameterValue>::const_iterator it=parameter->getValues().begin();
         it!=parameter->getValues().end();
         ++it) {

        const ParameterValue& pv = *it;
        if (it!=parameter->getValues().begin()) values += ",";
        if (pv.isTemperatureValid()) {
            values += QString::number(pv.getTemperature(), 'e', 6);
        } else {
            values += undefindedIdentifyerAsString();
        }
    }
    if (parameter->getValues().size()==0) values = undefindedIdentifyerAsString();
    stream.writeTextElement("Data", values);

    stream.writeStartElement("Qualifier");
    stream.writeAttribute("name", "Variable Type");
    stream.writeCharacters("Independent");
    stream.writeEndElement(); // Qualifier

    stream.writeEndElement(); // ParameterValue

    stream.writeEndElement(); // PropertyData

    referenceTemperatureProperty_->writeXMLData(stream);
}

void OrthotropicSecantCoefficientOfThermalExpansion::writeHTML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("tr");

    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBTitle");
    stream.writeAttribute("valign", "top");
    stream.writeCharacters(getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("colspan", "2");
    stream.writeAttribute("align", "right");

    stream.writeStartElement("table");
    //stream.writeAttribute("style", "border-top:1px solid #000;");
    stream.writeAttribute("rules", "groups");
    //stream.writeAttribute("class", "MatDBTable");

    Parameter* parameter;

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    parameter = getParameter("Coefficient of Thermal Expansion X direction");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters("CTE X");
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    parameter->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    parameter = getParameter("Coefficient of Thermal Expansion Y direction");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters("CTE Y");
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    parameter->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    parameter = getParameter("Coefficient of Thermal Expansion Z direction");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters("CTE Z");
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    parameter->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    parameter = getParameter("Reference Temperature");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters(parameter->getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    parameter->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeEndElement(); // table

    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
}

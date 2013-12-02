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

#include <iostream>
#include <cmath>

#include <QRadioButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

#include <nqlogger.h>

#include "referencetemperatureproperty.h"

ReferenceTemperatureProperty::ReferenceTemperatureProperty(PropertyModel *propmodel,
                                                           ParameterModel* paramodel, int id) :
    Property(id)
{
    setName("Reference Temperature");
    setDisplayName(QObject::tr("Reference Temperature"));
    setCategory(NoCategory);
    setType(ReferenceTemperature);
    setBehavior(Isotropic);
    Parameter *par;
    par = paramodel->getParameter("Reference Temperature");
    addParameter(par->clone());
    par = paramodel->getParameter("Material Property");
    addParameter(par->clone());
}

ReferenceTemperatureProperty::ReferenceTemperatureProperty(const ReferenceTemperatureProperty& property) :
    Property(property.getId())
{
    setName("Reference Temperature");
    setDisplayName(QObject::tr("Reference Temperature"));
    setCategory(NoCategory);
    setType(ReferenceTemperature);
    setBehavior(Isotropic);
    const Parameter *par1 = property.getParameter("Reference Temperature");
    addParameter(par1->clone());
    const Parameter *par2 = property.getParameter("Material Property");
    addParameter(par2->clone());
}

Property* ReferenceTemperatureProperty::clone(PropertyModel *propmodel,
                                              ParameterModel* paramodel)
{
    ReferenceTemperatureProperty* prop;
    if (propmodel && paramodel) {
        prop = new ReferenceTemperatureProperty(propmodel, paramodel, getId());
    } else {
        prop = new ReferenceTemperatureProperty(*this);
    }

    prop->setSorting(getSorting());

    return prop;
}

//<PropertyData property="pr12">
//  <Data format="float">-140.85</Data>
//  <Qualifier name="Variable Type">Dependent</Qualifier>
//</PropertyData>
void ReferenceTemperatureProperty::apply(PropertyData& data,
                                PropertyDetail& detail,
                                std::map<QString,ParameterDetail> /* paramMap */)
{
    Parameter * param = getParameter(detail.name);
    param->setValueUnit(detail.unit);

    double value;
    bool ok;
    value = data.data.toDouble(&ok);
    if (ok) {
        param->addValue(param->getValueUnit()->convertToPreffered(value));
    }

    param->setPrefferedValueUnit();
}

void ReferenceTemperatureProperty::writeXML(QXmlStreamWriter& stream)
{
    Parameter* parameter = Parameters_.begin()->second;

    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());

    parameter->getValueUnit()->writeXML(stream);

    stream.writeTextElement("Name", parameter->getName());

    stream.writeEndElement();
}

void ReferenceTemperatureProperty::writeXMLData(QXmlStreamWriter& stream)
{
    NQLog("ReferenceTemperatureProperty", NQLog::Spam) << "  XML write data for property " << getName()
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

    QString values("");
    Parameter * parameter = Parameters_.begin()->second;
    for (std::vector<ParameterValue>::const_iterator it=parameter->getValues().begin();
         it!=parameter->getValues().end();
         ++it) {

        const ParameterValue& pv = *it;
        if (it!=parameter->getValues().begin()) values += ",";
        values += QString::number(pv.getValue());
    }
    stream.writeCharacters(values);

    stream.writeStartElement("Qualifier");
    stream.writeAttribute("name", "Variable Type");
    stream.writeCharacters("Dependent");
    stream.writeEndElement(); // Qualifier

    stream.writeEndElement(); // Data

    stream.writeEndElement(); // PropertyData
}

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

#include "criticaltemperatureproperty.h"

CriticalTemperatureProperty::CriticalTemperatureProperty(PropertyModel *propmodel,
                                                         ParameterModel* paramodel, int id) :
    Property(id)
{
    setName("Critical Temperature");
    setDisplayName(QObject::tr("Critical Temperature"));
    setCategory(FluidProperty);
    setType(CriticalTemperature);
    setBehavior(Isotropic);
    Parameter *par = paramodel->getParameter("Critical Temperature");
    addParameter(par->clone());
}

CriticalTemperatureProperty::CriticalTemperatureProperty(const CriticalTemperatureProperty& property) :
    Property(property.getId())
{
    setName("Critical Temperature");
    setDisplayName(QObject::tr("Critical Temperature"));
    setCategory(PhysicalProperty);
    setType(CriticalTemperature);
    setBehavior(Isotropic);
    const Parameter *par = property.getParameter("Critical Temperature");
    addParameter(par->clone());
}

Property* CriticalTemperatureProperty::clone(PropertyModel* propmodel,
                                             ParameterModel* paramodel)
{
    CriticalTemperatureProperty* prop;
    if (propmodel && paramodel) {
       prop = new CriticalTemperatureProperty(propmodel, paramodel, getId());
    } else {
        prop = new CriticalTemperatureProperty(*this);
    }

    prop->setSorting(getSorting());

    return prop;
}

//<PropertyData property="pr12">
//  <Data format="float">-140.85</Data>
//  <Qualifier name="Variable Type">Dependent</Qualifier>
//</PropertyData>
void CriticalTemperatureProperty::apply(PropertyData& data,
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

void CriticalTemperatureProperty::writeXML(QXmlStreamWriter& stream)
{
    Parameter* parameter = Parameters_.begin()->second;

    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());

    parameter->getValueUnit()->writeXML(stream);

    stream.writeTextElement("Name", parameter->getName());

    stream.writeEndElement();
}

void CriticalTemperatureProperty::writeXMLData(QXmlStreamWriter& stream)
{
    NQLog("CriticalTemperature", NQLog::Spam) << "  XML write data for property " << getName()
                                          << " (" << getIdString().toStdString() << ")";

    stream.writeStartElement("PropertyData");
    stream.writeAttribute("property", getIdString());

    stream.writeStartElement("Data");
    stream.writeAttribute("format", "float");

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
    stream.writeEndElement(); // Data

    stream.writeStartElement("Qualifier");
    stream.writeAttribute("name", "Variable Type");
    stream.writeCharacters("Dependent");
    stream.writeEndElement(); // Qualifier

    stream.writeEndElement(); // PropertyData
}

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

#include "criticalpressureproperty.h"

CriticalPressureProperty::CriticalPressureProperty(ParameterModel* model, int id) :
    Property(id)
{
    setName("Critical Pressure");
    setDisplayName(QObject::tr("Critical Pressure"));
    setCategory(FluidProperty);
    setType(CriticalPressure);
    setBehavior(Isotropic);
    Parameter *par = model->getParameter("Critical Pressure");
    addParameter(par->clone());
}

CriticalPressureProperty::CriticalPressureProperty(const CriticalPressureProperty& property) :
    Property(property.getId())
{
    setName("Critical Pressure");
    setDisplayName(QObject::tr("Critical Pressure"));
    setCategory(PhysicalProperty);
    setType(CriticalPressure);
    setBehavior(Isotropic);
    const Parameter *par = property.getParameter("Critical Pressure");
    addParameter(par->clone());
}

Property* CriticalPressureProperty::clone(ParameterModel* model)
{
    CriticalPressureProperty* prop;
    if (model) {
       prop = new CriticalPressureProperty(model, getId());
    } else {
        prop = new CriticalPressureProperty(*this);
    }

    prop->setSorting(getSorting());

    return prop;
}

//<PropertyData property="pr11">
//  <Data format="float">3758000</Data>
//  <Qualifier name="Variable Type">Dependent</Qualifier>
//</PropertyData>
void CriticalPressureProperty::apply(PropertyData& data,
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

void CriticalPressureProperty::writeXML(QXmlStreamWriter& stream)
{
    Parameter* parameter = Parameters_.begin()->second;

    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());

    parameter->getValueUnit()->writeXML(stream);

    stream.writeTextElement("Name", parameter->getName());

    stream.writeEndElement();
}

void CriticalPressureProperty::writeXMLData(QXmlStreamWriter& stream)
{
    NQLog("CriticalPressure", NQLog::Spam) << "  XML write data for property " << getName()
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

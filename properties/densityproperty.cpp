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

#include "densityproperty.h"

DensityProperty::DensityProperty(ParameterModel* model, int id) :
    Property(id)
{
    setName("Density");
    setDisplayName(QObject::tr("Density"));
    setCategory(PhysicalProperty);
    setType(Density);
    setBehavior(Isotropic);
    Parameter *par = model->getParameter("Density");
    addParameter(par->clone());
}

DensityProperty::DensityProperty(const DensityProperty& property) :
    Property(property.getId())
{
    setName("Density");
    setDisplayName(QObject::tr("Density"));
    setCategory(PhysicalProperty);
    setType(Density);
    setBehavior(Isotropic);
    const Parameter *par = property.getParameter("Density");
    addParameter(par->clone());
}

Property* DensityProperty::clone(ParameterModel* model)
{
    DensityProperty* prop;
    if (model) {
       prop = new DensityProperty(model, getId());
    } else {
        prop = new DensityProperty(*this);
    }

    prop->setSorting(getSorting());

    return prop;
}

//<PropertyData property="pr0">
//  <Data format="float">6e-08</Data>
//  <Qualifier name="Variable Type">Dependent</Qualifier>
//  <ParameterValue parameter="pa0" format="float">
//    <Data>7.88860905221012e-31</Data>
//    <Qualifier name="Variable Type">Independent</Qualifier>
//  </ParameterValue>
//</PropertyData>
void DensityProperty::apply(PropertyData& data,
                            PropertyDetail& detail,
                            std::map<QString,ParameterDetail> /* paramMap */)
{
    PValue pt = data.pvalues.front();
    std::vector<double> values;
    std::vector<double> temperatures;

    if (data.data.contains(',')) {
        QStringList l = data.data.split(',');
        for (QStringList::Iterator it = l.begin();
             it!=l.end();
             ++it) {
            values.push_back((*it).toDouble());
        }
    } else {
        values.push_back(data.data.toDouble());
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

    Parameter * param = getParameter(detail.name);
    param = getParameter(detail.name);
    param->setValueUnit(detail.unit);

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

void DensityProperty::writeXML(QXmlStreamWriter& stream)
{
    Parameter* parameter = Parameters_.begin()->second;

    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());

    parameter->getValueUnit()->writeXML(stream);

    stream.writeTextElement("Name", parameter->getName());

    stream.writeEndElement();
}

void DensityProperty::writeXMLData(QXmlStreamWriter& stream)
{
    NQLog("DensityProperty", NQLog::Spam) << "  XML write data for property " << getName()
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

    stream.writeStartElement("ParameterValue");
    stream.writeAttribute("parameter", "pa0");
    stream.writeAttribute("format", "float");

    values = "";
    parameter = Parameters_.begin()->second;
    for (std::vector<ParameterValue>::const_iterator it=parameter->getValues().begin();
         it!=parameter->getValues().end();
         ++it) {

        const ParameterValue& pv = *it;
        if (it!=parameter->getValues().begin()) values += ",";
        if (pv.isTemperatureValid()) {
            values += QString::number(pv.getTemperature());
        } else {
            values += "7.88860905221012e-31";
        }
    }
    stream.writeTextElement("Data", values);

    stream.writeStartElement("Qualifier");
    stream.writeAttribute("name", "Variable Type");
    stream.writeCharacters("Independent");
    stream.writeEndElement(); // Qualifier

    stream.writeEndElement(); // ParameterValue

    stream.writeEndElement(); // PropertyData
}

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
#include <unit.h>

#include "property.h"

Property::Property(int id)
    : Behavior_(UnknownBehavior),
      Definition_(UnknownDefinition),
      sorting_(0),
      modified_(false)
{
    Id_ = id;
    IdString_ = "pr";
    IdString_ += QString::number(id);
}

Property::~Property()
{
    for (std::map<QString,Parameter*>::iterator it=Parameters_.begin();
         it!=Parameters_.end();
         ++it) delete it->second;
}

void Property::setName(const QString& name)
{
    Name_ = name;
}

void Property::setDisplayName(const QString& name)
{
    DisplayName_ = name;
}

QString Property::getCategoryName() const
{
    switch (Category_) {
    case PhysicalProperty:
        return QString(QObject::tr("Physical Properties"));
        break;
    case LinearElasticProperty:
        return QString(QObject::tr("Linear Elastic Properties"));
        break;
    case ThermalProperty:
        return QString(QObject::tr("Thermal Properties"));
        break;
    case ElectricalProperty:
        return QString(QObject::tr("Electrical Properties"));
        break;
    case FluidProperty:
        return QString(QObject::tr("Fluid Properties"));
        break;
    default:
        break;
    }
    return QString("Invalid Properties");
}

QString Property::getTypeName() const
{
    switch (Type_) {
    case Density:
        return QString("Density");
        break;
    case Elasticity:
        return QString("Elasticity");
        break;
    case ThermalConductivity:
        return QString("Thermal Conductivity");
        break;
    case CoefficientOfThermalExpansion:
        return QString("Coefficient of Thermal Expansion");
        break;
    case Resistivity:
        return QString("Resistivity");
        break;
    case SpecificHeat:
        return QString("Specific Heat");
        break;
    case Viscosity:
        return QString("Viscosity");
        break;
    case CriticalTemperature:
        return QString("Critical Temperature");
        break;
    case CriticalPressure:
        return QString("Critical Pressure");
        break;
    case BoilingPoint:
        return QString("Boiling Point");
        break;
    default:
        break;
    }
    return QString("Invalid Type");
}

QString Property::getBehaviorAsString() const
{
    switch (Behavior_) {
    case Isotropic:
        return QString("Isotropic");
        break;
    case Orthotropic:
        return QString("Orthotropic");
        break;
    default:
        break;
    }
    return QString("Invalid Behavior");
}

QString Property::getDefinitionAsString() const
{
    switch (Definition_) {
    case Instantaneous:
        return QString("Instantaneous");
        break;
    case Secant:
        return QString("Secant");
        break;
    default:
        break;
    }
    return QString("Invalid Definition");
}

void Property::addParameter(Parameter* parameter)
{
    std::map<QString,Parameter*>::iterator it = Parameters_.find(parameter->getName());
    Parameter * prop = NULL;
    if (it!=Parameters_.end()) prop = it->second;

    Parameters_[parameter->getName()] = parameter;
    OrderedParameters_.push_back(parameter);

    parameter->setProperty(this);

    if (prop) delete prop;
}

Parameter* Property::getParameter(const QString& name)
{
    std::map<QString,Parameter*>::iterator it = Parameters_.find(name);
    if (it!=Parameters_.end()) return it->second;
    return NULL;
}

const Parameter* Property::getParameter(const QString& name) const
{
    std::map<QString,Parameter*>::const_iterator it = Parameters_.find(name);
    if (it!=Parameters_.end()) return it->second;
    return NULL;
}

std::map<QString,Parameter*>& Property::getParameters()
{
    return Parameters_;
}

std::vector<Parameter*>& Property::getOrderedParameters()
{
    return OrderedParameters_;
}

void Property::write(QXmlStreamWriter& stream)
{
    stream.writeTextElement("Name", getName());
    for (std::vector<Parameter*>::iterator it = OrderedParameters_.begin();
         it!=OrderedParameters_.end();
         ++it) {
        Parameter * parameter = *it;
        stream.writeStartElement("Parameter");
        parameter->write(stream);
        stream.writeEndElement();
    }

    modified_ = false;
}

void Property::read(const QDomElement &element)
{
    QDomNodeList parameterElemList = element.elementsByTagName("Parameter");
    for (int i=0;i<parameterElemList.size();++i) {
        QDomElement parameterElem = parameterElemList.at(i).toElement();

        QDomElement name = parameterElem.elementsByTagName("Name").at(0).toElement();

        // NQLog("Property", NQLog::Spam) << "Parameter: " << name.text();

        Parameter * parameter = getParameter(name.text());
        if (parameter) parameter->read(parameterElem);
    }
}

void Property::writeHTML(QXmlStreamWriter& stream)
{
    if (getParameters().size()==1) {
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
        stream.writeAttribute("rules", "groups");
        //stream.writeAttribute("class", "MatDBTable");

        stream.writeStartElement("tr");

        stream.writeStartElement("td");
        stream.writeCharacters("");
        stream.writeEndElement(); // td

        stream.writeStartElement("td");
        stream.writeAttribute("align", "right");
        Parameter* parameter = getParameters().begin()->second;
        parameter->writeHTML(stream);
        stream.writeEndElement(); // td

        stream.writeEndElement(); // tr

        stream.writeEndElement(); // table

        stream.writeEndElement(); // td

        stream.writeEndElement(); // tr
    }
}

void Property::writeXML(QXmlStreamWriter& stream)
{
    Parameter* parameter = Parameters_.begin()->second;

    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());

    parameter->getValueUnit()->writeXML(stream);

    stream.writeTextElement("Name", parameter->getName());

    stream.writeEndElement();
}

void Property::writeXMLData(QXmlStreamWriter& stream)
{
    NQLog("Property", NQLog::Spam) << "  XML write data for property " << getName()
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

    for (std::map<QString,Parameter*>::const_iterator it = Parameters_.begin();
         it!=Parameters_.end();
         ++it) {
        Parameter * parameter = it->second;
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
    }

    stream.writeStartElement("ParameterValue");
    stream.writeAttribute("parameter", "pa0");
    stream.writeAttribute("format", "float");

    QString values;
    Parameter * parameter = Parameters_.begin()->second;
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
}

bool Property::isModified() const
{
    if (modified_) return modified_;

    for (std::map<QString,Parameter*>::const_iterator it = Parameters_.begin();
         it!=Parameters_.end();
         ++it) {
        Parameter * parameter = it->second;
        if (parameter->isModified()) return true;
    }

    return false;
}

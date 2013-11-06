#include <iostream>
#include <cmath>

#include <QRadioButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

#include "property.h"

Property::Property(int id)
    : Behavior_(UnknownBehavior),
      Definition_(UnknownDefinition),
      sorting_(0)
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

QString Property::getCategoryName() const
{
    switch (Category_) {
    case PhysicalProperty:
        return QString("Physical Properties");
        break;
    case LinearElasticProperty:
        return QString("Linear Elastic Properties");
        break;
    case ThermalProperty:
        return QString("Thermal Properties");
        break;
    case ElectricalProperty:
        return QString("Electrical Properties");
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

void Property::writeXML(QXmlStreamWriter& stream)
{

}

void Property::writeXMLData(QXmlStreamWriter& stream)
{
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
                values += QString::number(pv.getValue(), 'e', 6);
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

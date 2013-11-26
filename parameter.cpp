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

#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QStringList>

#include <nqlogger.h>

#include "parameter.h"
#include "property.h"

ParameterValue::ParameterValue() :
    Temperature_(0.0),
    ValidTemperature_(false),
    Value_(0.0),
    ValidValue_(false)
{

}

ParameterValue::ParameterValue(double temperature, double value)
{
    setTemperature(temperature);
    setValue(value);
}

ParameterValue::ParameterValue(const ParameterValue& other) :
    Temperature_(other.Temperature_),
    ValidTemperature_(other.ValidTemperature_),
    Value_(other.Value_),
    ValidValue_(other.ValidValue_)
{

}

const QString ParameterValue::prettyFormat(double value) const
{
    QString s;
    if (value<0) {
        value *= -1.;
        s = "-";
    }
    if (value>1e4) {
        int l1 = std::log10(value)/3;
        value /= std::pow(10., l1*3);
        int l2 = std::log10(value);
        s += QString::number(value, 'f', 3-l2);
        s += "E+";
        s += QString::number(l1*3);
    } else if (value>=0.1) {
        int l = std::log10(value);
        s += QString::number(value, 'f', 3-l);
    } else if (value==0.) {
        s += QString::number(value, 'f', 3);
    } else {
        int l1 = 1 - std::log10(value)/3;
        value *= std::pow(10., l1*3);
        int l2 = std::log10(value);
        s += QString::number(value, 'f', 3-l2);
        s += "E-";
        s += QString::number(l1*3);
    }

    return s;
}

Parameter::Parameter(Unit::VUnit* unit, int id, bool tempDependent) :
    Property_(0),
    TemperatureUnit_(new Unit::Temperature()),
    ValueUnit_(unit),
    Values_(0),
    ReadOnly_(false),
    Dependent_(false),
    TemperatureDependent_(tempDependent),
    modified_(false)
{
    Values_ = new ParameterValueVector;

    Id_ = id;
    IdString_ = "pa";
    IdString_ += QString::number(id);
}

Parameter::~Parameter()
{
    delete TemperatureUnit_;
    delete ValueUnit_;
    if (Values_) delete Values_;
}

Parameter* Parameter::clone() const
{
    Parameter * newParam = new Parameter(ValueUnit_->cloneWithUnitIndex(),
                                         getId(),
                                         TemperatureDependent_);
    newParam->setName(getName());
    newParam->setDisplayName(getDisplayName());

    for (std::vector<ParameterValue>::const_iterator it=getValues().begin();
         it!=getValues().end();
         ++it) {

        const ParameterValue& pv = *it;
        if (pv.isTemperatureValid()) {
            newParam->addValue(pv.getTemperature(), pv.getValue());
        } else {
            newParam->addValue(pv.getValue());
        }
    }

    return newParam;
}

void Parameter::setName(const QString& name)
{
    Name_ = name;
}

void Parameter::setDisplayName(const QString& name)
{
    DisplayName_ = name;
}

void Parameter::setValueUnit(const QString& unit)
{
    modified_ = true;
    ValueUnit_->setCurrentUnit(unit);
}

void Parameter::setTemperatureUnit(const QString& unit)
{
    modified_ = true;
    TemperatureUnit_->setCurrentUnit(unit);
}

void Parameter::setPrefferedValueUnit()
{
    modified_ = true;
    ValueUnit_->setCurrentUnitIndex(ValueUnit_->getPrefferedUnitIndex());
}

void Parameter::setPrefferedTemperatureUnit()
{
    modified_ = true;
    TemperatureUnit_->setCurrentUnitIndex(TemperatureUnit_->getPrefferedUnitIndex());
}

void Parameter::addValue(double value)
{
    ParameterValue v;
    v.setValue(value);
    addValue(v);
}

void Parameter::addValue(double temperature, double value)
{
    ParameterValue v;
    v.setTemperature(temperature);
    v.setValue(value);
    addValue(v);
}

void Parameter::addValue(const ParameterValue& value)
{
    if (!Values_) Values_ = new ParameterValueVector;

    Values_->push_back(ParameterValue(value));
    modified_ = true;
}

int Parameter::getNumberOfValues() const
{
    if (Values_) return Values_->size();
    return 0;
}

std::vector<ParameterValue>& Parameter::getValues()
{
    return *Values_;
}

const std::vector<ParameterValue>& Parameter::getValues() const
{
    return *Values_;
}

void Parameter::clear()
{
    if (Values_)
        Values_->clear();

    modified_ = true;
}

void Parameter::deleteValue(size_t idx)
{
    if (!Values_) return;

    if (idx>=Values_->size()) return;

    Values_->erase(Values_->begin() + idx);

    modified_ = true;
}

void Parameter::deleteTemperature(size_t idx)
{
    if (!Values_) return;

    if (idx>=Values_->size()) return;

    ParameterValue& pvalue = Values_->at(idx);
    pvalue.setTemperature(0);
    pvalue.resetTemperature();

    sort();

    modified_ = true;
}

void Parameter::importValues(const QString& filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);

        clear();

        QRegExp rx("(?:^|,)(?:\\s*)\"(.*)\"(?:\\s*)(?:,|$)|(?:^|,)(?:\\s*)([^\"]*)(?:\\s*)(?:,|$)",
                   Qt::CaseSensitive,
                   QRegExp::RegExp2);
        rx.setMinimal(true);

        double t, v;
        QString line;
        do {
            line = in.readLine();

            QStringList list;
            int pos = 0;

            while ((pos = rx.indexIn(line, pos)) != -1) {
                list << (rx.cap(1).isEmpty() ? rx.cap(2) : rx.cap(1));
                pos += rx.matchedLength()-1;
            }

            // NQLog("Parameter", NQLog::Spam) << line << " " << list.size();

            if (list.size()!=2) continue;

            bool ok;
            t = list.at(0).toDouble(&ok);
            if (!ok) continue;
            v = list.at(1).toDouble(&ok);
            if (!ok) continue;

            addValue(t, v);

       } while (!line.isNull());
    }

    sort();
}

void Parameter::sort()
{
    if (!Values_) return;

#ifdef WIN32GPP
    std::sort(Values_->begin(), Values_->end(),
              [](ParameterValue lhs,ParameterValue rhs) {
        if (lhs.getTemperature()<rhs.getTemperature()) {
            return true;
        } else if(lhs.getTemperature()>rhs.getTemperature()) {
            return false;
        } else if(lhs.getValue()<rhs.getValue()) {
            return true;
        }
        return false;
    });
#else
    std::sort(Values_->begin(), Values_->end(),
              [](ParameterValue& lhs,ParameterValue& rhs) {
        if (lhs.getTemperature()<rhs.getTemperature()) {
            return true;
        } else if(lhs.getTemperature()>rhs.getTemperature()) {
            return false;
        } else if(lhs.getValue()<rhs.getValue()) {
            return true;
        }
        return false;
    });
#endif
}

void Parameter::write(QXmlStreamWriter& stream)
{
    stream.writeTextElement("Name", getName());

    stream.writeStartElement("ValueUnit");
    getValueUnit()->writeXML(stream);
    stream.writeEndElement();

    stream.writeStartElement("TemperatureUnit");
    getTemperatureUnit()->writeXML(stream);
    stream.writeEndElement();

    if (Values_) {
        for (std::vector<ParameterValue>::const_iterator it=getValues().begin();
             it!=getValues().end();
             ++it) {
            const ParameterValue& pv = *it;
            stream.writeStartElement("Value");

            if (pv.isTemperatureValid()) {
                stream.writeAttribute("Temperature", QString::number(pv.getTemperature(), 'e', 6));
            } else {
                stream.writeAttribute("Temperature", Property::undefindedIdentifyerAsString());
            }

            if (pv.isValueValid()) {
                stream.writeCharacters(QString::number(pv.getValue(), 'e', 12));
            } else {
                stream.writeCharacters(Property::undefindedIdentifyerAsString());
            }
            stream.writeEndElement();
        }
    }

    modified_ = false;
}

void Parameter::read(const QDomElement &element)
{
    QDomNodeList unitsElemList = element.elementsByTagName("ValueUnit");
    if (unitsElemList.count()==1) {
        QDomElement unitsElem = unitsElemList.at(0).toElement();

        QDomNodeList unitElemList = unitsElem.elementsByTagName("Unit");
        QString unit;
        for (int i=0;i<unitElemList.size();++i) {
            QDomElement unitElem = unitElemList.at(i).toElement();

            QDomElement name = unitElem.elementsByTagName("Name").at(0).toElement();
            if (i!=0) unit += " ";

            unit += name.text();
            if (unitElem.hasAttribute("power")) {
                unit += "^";
                unit += unitElem.attribute("power", "1");
            }
        }

        // NQLog("Parameter", NQLog::Spam) << "unit: " << unit;

        setValueUnit(unit);
    }

    unitsElemList = element.elementsByTagName("TemperatureUnit");
    if (unitsElemList.count()==1) {
        QDomElement unitsElem = unitsElemList.at(0).toElement();

        QDomNodeList unitElemList = unitsElem.elementsByTagName("Unit");
        QString unit;
        for (int i=0;i<unitElemList.size();++i) {
            QDomElement unitElem = unitElemList.at(i).toElement();

            QDomElement name = unitElem.elementsByTagName("Name").at(0).toElement();
            if (i!=0) unit += " ";

            unit += name.text();
            unit += name.attribute("power", "");
        }

        setTemperatureUnit(unit);
    }

    QDomNodeList valueElemList = element.elementsByTagName("Value");
    for (int i=0;i<valueElemList.size();++i) {
        QDomElement valueElem = valueElemList.at(i).toElement();

        QString svalue = valueElem.text();
        QString stemperature = valueElem.attribute("Temperature", Property::undefindedIdentifyerAsString());

        if (svalue!=Property::undefindedIdentifyerAsString()) {
            if (stemperature!=Property::undefindedIdentifyerAsString()) {
                addValue(stemperature.toDouble(), svalue.toDouble());
            } else {
                addValue(svalue.toDouble());
            }
        }
    }

    modified_ = false;
}

void Parameter::writeXML(QXmlStreamWriter& stream)
{
    if (getId()==-1) return;

    stream.writeStartElement("ParameterDetails");
    stream.writeAttribute("id", getIdString());

    getValueUnit()->writeXML(stream);

    stream.writeTextElement("Name", getName());

    stream.writeEndElement(); // ParameterDetails
}

void Parameter::writeHTML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("table");
    //stream.writeAttribute("class", "MatDBTable");

    if (getNumberOfValues()==0) {
        stream.writeStartElement("tr");
        stream.writeStartElement("td");
        stream.writeAttribute("colspan", "3");
        stream.writeAttribute("align", "right");
        stream.writeAttribute("bgcolor", "red");
        stream.writeCharacters("undefined");
        stream.writeEndElement(); // td
        stream.writeEndElement(); // tr
    } else {
        int idx=0;
        for (ParameterValueVector::iterator it = Values_->begin();
             it!=Values_->end();
             ++it) {
            stream.writeStartElement("tr");

            ParameterValue& pvalue = *it;

            stream.writeStartElement("td");
            stream.writeAttribute("class", "MatDBValue");
            stream.writeAttribute("style", "width:40px;");
            if (Values_->size()>1) {
                stream.writeCharacters(QString::number(idx));
            } else {
                stream.writeCharacters("");
            }
            stream.writeEndElement(); // td

            QString ts = "";
            if (pvalue.isTemperatureValid() && pvalue.isValueValid()) {
                ts += pvalue.prettyTemperature();
                if (idx==0) {
                    ts += " ";
                    ts += getTemperatureUnit()->currentUnitAsString();
                }
            }
            stream.writeStartElement("td");
            stream.writeAttribute("class", "MatDBValue");
            stream.writeAttribute("align", "right");
            stream.writeAttribute("style", "width:120px;");
            stream.writeCharacters(ts);
            stream.writeEndElement(); // td

            QString vs = "";
            if (pvalue.isValueValid()) {
                vs += pvalue.prettyValue();
                if (idx==0 && getValueUnit()->currentUnitAsString()!="1") {
                    vs += " ";
                    vs += getValueUnit()->currentUnitAsString();
                }
            } else {
                vs = "undefined";
            }
            stream.writeStartElement("td");
            stream.writeAttribute("class", "MatDBValue");
            stream.writeAttribute("align", "right");
            stream.writeAttribute("style", "width:120px;");
            stream.writeCharacters(vs);

            stream.writeEndElement(); // td

            stream.writeEndElement(); // tr
        }
    }

    stream.writeEndElement(); // table
}

bool Parameter::isModified() const
{
    if (modified_) return modified_;

    return false;
}

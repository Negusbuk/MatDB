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
    TemperatureDependent_(tempDependent)
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

void Parameter::setValueUnit(const QString& unit)
{
    ValueUnit_->setCurrentUnit(unit);
}

void Parameter::setPrefferedValueUnit()
{
    ValueUnit_->setCurrentUnitIndex(ValueUnit_->getPrefferedUnitIndex());
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
}

void Parameter::deleteValue(size_t idx)
{
    if (!Values_) return;

    if (idx>=Values_->size()) return;

    Values_->erase(Values_->begin() + idx);
}

void Parameter::deleteTemperature(size_t idx)
{
    if (!Values_) return;

    if (idx>=Values_->size()) return;

    ParameterValue& pvalue = Values_->at(idx);
    pvalue.setTemperature(0);
    pvalue.resetTemperature();

    sort();
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

            NQLog("MaterialParameterView", NQLog::Spam) << line << " " << list.size();

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

void Parameter::writeXML(QXmlStreamWriter& stream)
{
    if (getId()==-1) return;

    stream.writeStartElement("ParameterDetails");
    stream.writeAttribute("id", getIdString());

    getValueUnit()->writeXML(stream);

    stream.writeTextElement("Name", getName());

    stream.writeEndElement(); // ParameterDetails
}

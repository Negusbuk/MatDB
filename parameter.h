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

#ifndef PARAMETER_H
#define PARAMETER_H

#include <vector>

#include <QString>
#include <QXmlStreamWriter>
#include <QDomElement>

#include <unit.h>

class ParameterValue {
public:
    ParameterValue();
    ParameterValue(double temperature, double value);
    ParameterValue(const ParameterValue& other);

    double getTemperature() const { return Temperature_; }
    const QString prettyTemperature() const { return prettyFormat(Temperature_); }
    bool isTemperatureValid() const { return ValidTemperature_; }
    void setTemperature(double v) { Temperature_ = v; ValidTemperature_ = true; }
    void resetTemperature() { ValidTemperature_ = false; }

    double getValue() const { return Value_; }
    const QString prettyValue() const { return prettyFormat(Value_); }
    bool isValueValid() const { return ValidValue_; }
    void setValue(double v) { Value_ = v; ValidValue_ = true; }
    void resetValue() { ValidValue_ = false; }

protected:
    double Temperature_;
    bool ValidTemperature_;
    double Value_;
    bool ValidValue_;
    const QString prettyFormat(double value) const;
};

typedef std::vector<ParameterValue> ParameterValueVector;

class Property;
class MaterialPropertyViewParameterItem;

class Parameter
{
public:
    Parameter(Unit::VUnit* unit, int id, bool tempDependent = true);
    virtual ~Parameter();

    Unit::VUnit* getTemperatureUnit() const { return TemperatureUnit_; }
    Unit::VUnit* getValueUnit() const { return ValueUnit_; }
    void setValueUnit(const QString& unit);
    void setTemperatureUnit(const QString& unit);
    void setPrefferedValueUnit();
    void setPrefferedTemperatureUnit();

    int getId() const { return Id_; }
    const QString& getIdString() const { return IdString_; }

    const QString& getName() const { return Name_; }
    void setName(const QString& name);

    const QString& getDisplayName() const { return DisplayName_; }
    void setDisplayName(const QString& name);

    void setParameterValues(ParameterValueVector* values) { Values_ = values; }
    void addValue(double value);
    void addValue(double value, const QString& unit);
    void addValue(double temperature, double value);
    void addValue(const ParameterValue& value);
    int getNumberOfValues() const;
    std::vector<ParameterValue>& getValues();
    const std::vector<ParameterValue>& getValues() const;
    void deleteValue(size_t idx);
    void deleteTemperature(size_t idx);
    void importValues(const QString& filename);

    void sort();

    Parameter* clone() const;

    bool isReadOnly() const { return ReadOnly_; }
    void setReadOnly(bool ro) { ReadOnly_ = ro; }

    bool isDependent() const { return Dependent_; }
    void setDependent(bool dependent) { Dependent_ = dependent; }

    bool isTemperatureDependent() const { return TemperatureDependent_; }

    Property* getProperty() const { return Property_; }
    void setProperty(Property * property) { Property_ = property; }

    void setViewItem(MaterialPropertyViewParameterItem* viewItem) { viewItem_ = viewItem; }
    MaterialPropertyViewParameterItem* getViewItem() { return viewItem_; }

    void clear();

    void setModified(bool modified) { modified_ = modified; }
    bool isModified() const;

    virtual void write(QXmlStreamWriter& stream);
    virtual void read(const QDomElement& element);
    virtual void writeHTML(QXmlStreamWriter& stream);
    virtual void writeXML(QXmlStreamWriter& stream);

protected:
    Property* Property_;
    Unit::VUnit* TemperatureUnit_;
    Unit::VUnit* ValueUnit_;
    int Id_;
    QString IdString_;
    QString Name_;
    QString DisplayName_;
    ParameterValueVector* Values_;
    bool ReadOnly_;
    bool Dependent_;
    bool TemperatureDependent_;
    MaterialPropertyViewParameterItem* viewItem_;
    bool modified_;
};

#endif // PARAMETER_H

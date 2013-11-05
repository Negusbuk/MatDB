#ifndef PARAMETER_H
#define PARAMETER_H

#include <vector>

#include <QString>
#include <QXmlStreamWriter>

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
    Parameter(Unit::VUnit* unit, int id);
    virtual ~Parameter();

    Unit::VUnit* getTemperatureUnit() const { return TemperatureUnit_; }
    Unit::VUnit* getValueUnit() const { return ValueUnit_; }
    void setValueUnit(const QString& unit);
    void setPrefferedValueUnit();

    int getId() const { return Id_; }
    const QString& getIdString() const { return IdString_; }

    const QString& getName() const { return Name_; }
    void setName(const QString& name);

    void setParameterValues(ParameterValueVector* values) { Values_ = values; }
    void addValue(double value);
    void addValue(double temperature, double value);
    void addValue(const ParameterValue& value);
    int getNumberOfValues() const;
    std::vector<ParameterValue>& getValues();
    const std::vector<ParameterValue>& getValues() const;

    Parameter* clone() const;

    bool isReadOnly() const { return ReadOnly_; }
    void setReadOnly(bool ro) { ReadOnly_ = ro; }

    bool isDependent() const { return Dependent_; }
    void setDependent(bool dependent) { Dependent_ = dependent; }

    Property* getProperty() const { return Property_; }
    void setProperty(Property * property) { Property_ = property; }

    void setViewItem(MaterialPropertyViewParameterItem* viewItem) { viewItem_ = viewItem; }
    MaterialPropertyViewParameterItem* getViewItem() { return viewItem_; }

    void clear();

    virtual void writeXML(QXmlStreamWriter& stream);

protected:
    Property* Property_;
    Unit::VUnit* TemperatureUnit_;
    Unit::VUnit* ValueUnit_;
    int Id_;
    QString IdString_;
    QString Name_;
    ParameterValueVector* Values_;
    bool ReadOnly_;
    bool Dependent_;
    MaterialPropertyViewParameterItem* viewItem_;
};

#endif // PARAMETER_H

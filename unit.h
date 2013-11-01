#ifndef UNIT_H
#define UNIT_H

#include <functional>
#include <vector>
#include <map>

#include <QDoubleValidator>
#include <QString>
#include <QXmlStreamWriter>

namespace Unit {

class UnitEntry
{
public:
    UnitEntry();
    UnitEntry(const QString& name, double minValue, double maxValue,
              std::function<double (double)> funcToBaseUnit,
              std::function<double (double)> funcFromBaseUnit,
              bool isPrefferedUnit = false);

    int UnitIndex_;
    QString Name_;
    double MinValue_;
    double MaxValue_;
    std::function<double (double)> funcToBaseUnit_;
    std::function<double (double)> funcFromBaseUnit_;
    bool isPrefferedUnit_;
};

class VUnit : public QDoubleValidator
{
public:
    VUnit();

    const std::vector<QString> getUnits() const { return Units_; }
    int currentUnit() const { return CurrentUnit_; }
    const QString& currentUnitAsString() const { return Units_[CurrentUnit_]; }

    int getUnitIndex(const QString& unit) const;
    int getPrefferedUnitIndex() const { return PrefferedUnitIndex_; }
    const UnitEntry* getUnitEntry(int idx) const;
    int getCurrentUnitIndex() const { return CurrentUnit_; }
    void setCurrentUnitIndex(int index) { CurrentUnit_ = index; }
    void setCurrentUnit(const QString& unit);

    double convert(double value, const QString& unit);
    virtual double convert(double value, int unitIndex);
    virtual double convertToPrefferedUnit(double value);
    virtual VUnit* clone() const = 0;
    virtual VUnit* cloneWithUnitIndex() const;

    virtual void writeXML(QXmlStreamWriter& stream);

protected:
    std::vector<QString> Units_;
    std::map<QString,UnitEntry> UnitsMap_;
    int CurrentUnit_;
    int PrefferedUnitIndex_;

    void addUnit(const QString& unit, double minValue, double maxValue,
                 std::function<double (double)> funcToBaseUnit,
                 std::function<double (double)> funcFromBaseUnit,
                 bool isPrefferedUnit = false);
};

class Unitless : public VUnit
{
public:
    Unitless();
    VUnit* clone() const { return new Unitless(); }
    virtual void writeXML(QXmlStreamWriter& stream);
};

class Temperature : public VUnit
{
public:
    Temperature();
    VUnit* clone() const { return new Temperature(); }
};

class Density : public VUnit
{
public:
    Density();
    VUnit* clone() const { return new Density(); }
};

class Pressure : public VUnit
{
public:
    Pressure();
    VUnit* clone() const { return new Pressure(); }
};

class ThermalConductivity : public VUnit
{
public:
    ThermalConductivity();
    VUnit* clone() const { return new ThermalConductivity(); }
};

class CoefficientOfThermalExpansion : public VUnit
{
public:
    CoefficientOfThermalExpansion();
    VUnit* clone() const { return new CoefficientOfThermalExpansion(); }
};

class Resistivity : public VUnit
{
public:
    Resistivity();
    VUnit* clone() const { return new Resistivity(); }
};

class SpecificHeat : public VUnit
{
public:
    SpecificHeat();
    VUnit* clone() const { return new SpecificHeat(); }
};

};

#endif // UNIT_H

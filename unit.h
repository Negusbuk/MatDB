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
              bool isPrefferedUnit = false,
              bool isXMLexportUnit = false);

    int UnitIndex_;
    QString Name_;
    double MinValue_;
    double MaxValue_;
    std::function<double (double)> funcToBaseUnit_;
    std::function<double (double)> funcFromBaseUnit_;
    bool isPrefferedUnit_;
    bool isXMLexportUnit_;
};

class VUnit : public QDoubleValidator
{
public:
    VUnit();

    const std::vector<QString> getUnits() const { return Units_; }
    int currentUnit() const { return CurrentUnit_; }
    const QString& currentUnitAsString() const { return Units_[CurrentUnit_]; }
    const QString& xmlExportUnitAsString() const;

    int getUnitIndex(const QString& unit) const;
    int getPrefferedUnitIndex() const { return PrefferedUnitIndex_; }
    const UnitEntry* getUnitEntry(int idx) const;
    int getCurrentUnitIndex() const { return CurrentUnit_; }
    void setCurrentUnitIndex(int index) { CurrentUnit_ = index; }
    void setCurrentUnit(const QString& unit);
    void setPrefferedUnit() { CurrentUnit_ = PrefferedUnitIndex_; }

    double convert(double value, const QString& unit);
    virtual double convert(double value, int unitIndex);
    virtual double convertToBase(double value);
    virtual double convertToPreffered(double value);
    virtual double convertToXMLExport(double value);
    virtual double convertToCurrent(double value);
    virtual double convertToCurrent(double value, const QString& unit);
    virtual VUnit* clone() const = 0;
    virtual VUnit* cloneWithUnitIndex() const;

    bool hasXMLExportUnit() const { return XMLExportUnitIndex_!=-1; }

    virtual void writeXML(QXmlStreamWriter& stream);
    virtual void writeXMLexport(QXmlStreamWriter& stream);

protected:
    std::vector<QString> Units_;
    std::map<QString,UnitEntry> UnitsMap_;
    int CurrentUnit_;
    int PrefferedUnitIndex_;
    int XMLExportUnitIndex_;

    void addUnit(const QString& unit, double minValue, double maxValue,
                 std::function<double (double)> funcToBaseUnit,
                 std::function<double (double)> funcFromBaseUnit,
                 bool isPrefferedUnit = false,
                 bool isXMLExportUnit = false);
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

class Viscosity : public VUnit
{
public:
    Viscosity();
    VUnit* clone() const { return new Viscosity(); }
};

};

#endif // UNIT_H

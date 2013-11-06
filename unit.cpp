#include <iostream>
#include <limits>

#include <QStringList>

#include "unit.h"

namespace Unit {

UnitEntry::UnitEntry()
{

}

UnitEntry::UnitEntry(const QString& name, double minValue, double maxValue,
                     std::function<double (double)> funcToBaseUnit,
                     std::function<double (double)> funcFromBaseUnit,
                     bool isPrefferedUnit) :
    Name_(name),
    MinValue_(minValue),
    MaxValue_(maxValue),
    funcToBaseUnit_(funcToBaseUnit),
    funcFromBaseUnit_(funcFromBaseUnit),
    isPrefferedUnit_(isPrefferedUnit)
{
    UnitIndex_ = 0;
}

VUnit::VUnit()
{
    setNotation(QDoubleValidator::ScientificNotation);

    CurrentUnit_ = 0;
    PrefferedUnitIndex_ = 0;
}

double VUnit::convert(double value, const QString& unit)
{
    int unitIndex = getUnitIndex(unit);
    return convert(value, unitIndex);
}

void VUnit::addUnit(const QString& unit, double minValue, double maxValue,
                    std::function<double (double)> funcToBaseUnit,
                    std::function<double (double)> funcFromBaseUnit,
                    bool isPrefferedUnit)
{
    UnitEntry entry(unit, minValue, maxValue,
                    funcToBaseUnit, funcFromBaseUnit);
    entry.UnitIndex_ = Units_.size();
    if (isPrefferedUnit) PrefferedUnitIndex_ = entry.UnitIndex_;
    UnitsMap_[unit] = entry;
    Units_.push_back(unit);
}

int VUnit::getUnitIndex(const QString& unit) const
{
    std::map<QString,UnitEntry>::const_iterator it = UnitsMap_.find(unit);
    if (it==UnitsMap_.end()) return -1;
    return it->second.UnitIndex_;
}

const UnitEntry* VUnit::getUnitEntry(int idx) const
{
    for (std::map<QString,UnitEntry>::const_iterator it = UnitsMap_.begin();
         it!=UnitsMap_.end();
         ++it) {
        if (it->second.UnitIndex_==idx) return &(it->second);
    }
    return 0;
}

void VUnit::setCurrentUnit(const QString& unit) {
    int idx = getUnitIndex(unit);
    if (idx==-1) return;
    setCurrentUnitIndex(idx);
}

double VUnit::convert(double value, int unitIndex)
{
    int newIndex = unitIndex;
    if (CurrentUnit_==newIndex) return value;

    const UnitEntry * cUnit = getUnitEntry(CurrentUnit_);
    const UnitEntry * nUnit = getUnitEntry(newIndex);

    double newValue = cUnit->funcToBaseUnit_(value);
    newValue = nUnit->funcFromBaseUnit_(newValue);

    return newValue;
}

double VUnit::convertToBase(double value)
{
    const UnitEntry * unit = getUnitEntry(CurrentUnit_);
    double newValue = unit->funcToBaseUnit_(value);
    return newValue;
}

double VUnit::convertToPreffered(double value)
{
    return convert(value, PrefferedUnitIndex_);
}

double VUnit::convertToCurrent(double value)
{
    const UnitEntry * unit = getUnitEntry(CurrentUnit_);
    double newValue = unit->funcFromBaseUnit_(value);
    return newValue;
}

VUnit* VUnit::cloneWithUnitIndex() const
{
    VUnit* unit = this->clone();
    unit->setCurrentUnitIndex(this->getCurrentUnitIndex());
    return unit;
}

void VUnit::writeXML(QXmlStreamWriter& stream)
{
    QString unit = currentUnitAsString();
    QStringList l = unit.split(" ");

    stream.writeStartElement("Units");

    for (QStringList::Iterator it = l.begin();
         it!=l.end();
         ++it) {
        QString u = *it;
        int power = 0;
        if (u.contains('^')) {
            power = u.section('^', 1, 1).toInt();
            u = u.section('^', 0, 0);
        }

        stream.writeStartElement("Unit");
        if (power!=0) stream.writeAttribute("power", QString::number(power));
        stream.writeTextElement("Name", u);
        stream.writeEndElement(); // Unit
    }

    stream.writeEndElement(); // Units
}

Unitless::Unitless() :
    VUnit()
{
    addUnit("1", -std::numeric_limits<double>::max(), std::numeric_limits<double>::max(),
            [&] (double value) {
               return value;
            },
            [&] (double value) {
               return value;
            });
}

void Unitless::writeXML(QXmlStreamWriter& stream)
{
    stream.writeEmptyElement("Unitless");
}

Temperature::Temperature() :
    VUnit()
{
    addUnit("C", -273.15, std::numeric_limits<double>::max(),
            [&] (double value) {
               return value;
            },
            [&] (double value) {
               return value;
            },
            true);

    addUnit("K", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return value - 273.15;
            },
            [&] (double value) {
               return value + 273.15;
            });

    addUnit("F", -459.67, std::numeric_limits<double>::max(),
            [&] (double value) {
               return (value-32.0)/1.8;
            },
            [&] (double value) {
               return value * 1.8 + 32.0;
            });
}

Density::Density() :
    VUnit()
{
    addUnit("kg m^-3", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return value;
            },
            [&] (double value) {
               return value;
            });

    addUnit("kg cm^-3", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e6*value;
            },
            [&] (double value) {
               return 1.e-6*value;
            });

    addUnit("kg mm^-3", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e9*value;
            },
            [&] (double value) {
               return 1.e-9*value;
            });

    addUnit("g cm^-3", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e3*value;
            },
            [&] (double value) {
               return 1.e-3*value;
            },
            true);

    addUnit("lb in^-3", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 27679.904710191*value;
            },
            [&] (double value) {
               return value/27679.904710191;
            });

    addUnit("lb ft^-3", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 16.018463374*value;
            },
            [&] (double value) {
               return value/16.018463374;
            });
}

Pressure::Pressure() :
    VUnit()
{
    addUnit("Pa", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return value;
            },
            [&] (double value) {
               return value;
            });

    addUnit("hPa", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e2*value;
            },
            [&] (double value) {
               return 1.e-2*value;
            });

    addUnit("kPa", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e3*value;
            },
            [&] (double value) {
               return 1.e-3*value;
            });

    addUnit("MPa", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e6*value;
            },
            [&] (double value) {
               return 1.e-6*value;
            });

    addUnit("GPa", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e9*value;
            },
            [&] (double value) {
               return 1.e-9*value;
            },
            true);

    addUnit("bar", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e5*value;
            },
            [&] (double value) {
               return 1.e-5*value;
            });

    addUnit("mbar", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e2*value;
            },
            [&] (double value) {
               return 1.e-2*value;
            });

    addUnit("psi", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 6894.757293178*value;
            },
            [&] (double value) {
               return value/6894.757293178;
            });

    addUnit("torr", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 133.322368421*value;
            },
            [&] (double value) {
               return value/133.322368421;
            });

    addUnit("atm", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 101325*value;
            },
            [&] (double value) {
               return value/101325;
            });
}

ThermalConductivity::ThermalConductivity() :
    VUnit()
{
    addUnit("W m^-1 K^-1", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return value;
            },
            [&] (double value) {
               return value;
            },
            true);

    addUnit("W m^-1 C^-1", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return value;
            },
            [&] (double value) {
               return value;
            });

    addUnit("W cm^-1 K^-1", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e2*value;
            },
            [&] (double value) {
               return 1.e-2*value;
            });

    addUnit("W cm^-1 C^-1", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e2*value;
            },
            [&] (double value) {
               return 1.e-2*value;
            });

    addUnit("W mm^-1 K^-1", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e3*value;
            },
            [&] (double value) {
               return 1.e-3*value;
            });

    addUnit("W mm^-1 C^-1", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e3*value;
            },
            [&] (double value) {
               return 1.e-3*value;
            });

    addUnit("Btu h^-1 ft^-1 F^-1", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.730734666*value;
            },
            [&] (double value) {
               return value/1.730734666;
            });
}

CoefficientOfThermalExpansion::CoefficientOfThermalExpansion() :
    VUnit()
{
    addUnit("ppm K^-1", -std::numeric_limits<double>::max(), std::numeric_limits<double>::max(),
            [&] (double value) {
               return value;
            },
            [&] (double value) {
               return value;
            },
            true);

    addUnit("ppm C^-1", -std::numeric_limits<double>::max(), std::numeric_limits<double>::max(),
            [&] (double value) {
               return value;
            },
            [&] (double value) {
               return value;
            });

    addUnit("K^-1", -std::numeric_limits<double>::max(), std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e6*value;
            },
            [&] (double value) {
               return 1.e-6*value;
            });

    addUnit("C^-1", -std::numeric_limits<double>::max(), std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e6*value;
            },
            [&] (double value) {
               return 1.e-6*value;
            });
}

Resistivity::Resistivity() :
    VUnit()
{
    addUnit("Ohm m", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return value;
            },
            [&] (double value) {
               return value;
            },
            true);

    addUnit("Ohm cm", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e-2*value;
            },
            [&] (double value) {
               return 1.e+2*value;
            });

    addUnit("Ohm mm", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e-3*value;
            },
            [&] (double value) {
               return 1.e+3*value;
            });
}

SpecificHeat::SpecificHeat() :
    VUnit()
{
    addUnit("J kg^-1 K^-1", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return value;
            },
            [&] (double value) {
               return value;
            },
            true);

    addUnit("kJ kg^-1 K^-1", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e-3*value;
            },
            [&] (double value) {
               return 1.e+3*value;
            });

    addUnit("J g^-1 K^-1", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 1.e-3*value;
            },
            [&] (double value) {
               return 1.e+3*value;
            });
}

Viscosity::Viscosity() :
    VUnit()
{
    addUnit("Pa s", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return value;
            },
            [&] (double value) {
               return value;
            },
            true);

    addUnit("kg m^-1 s^-1", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return value;
            },
            [&] (double value) {
               return value;
            });

    addUnit("g cm^-1 s^-1", 0, std::numeric_limits<double>::max(),
            [&] (double value) {
               return 0.1*value;
            },
            [&] (double value) {
               return 10.0*value;
            });
}

};

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

#include <parametermodel.h>

ParameterModel::ParameterModel(QObject *parent) :
    QObject(parent)
{
    build();
}

Parameter * ParameterModel::getParameter(const char* name) const
{
    std::map<QString,Parameter*>::const_iterator it = Parameters_.find(QString(name));
    if (it!=Parameters_.end()) return it->second;
    return NULL;
}

void ParameterModel::addParameter(Parameter* parameter)
{
    std::map<QString,Parameter*>::iterator it = Parameters_.find(parameter->getName());
    Parameter * par = NULL;
    if (it!=Parameters_.end()) par = it->second;

    Parameters_[parameter->getName()] = parameter;
    ParametersById_[parameter->getId()] = parameter;

    if (par) delete par;
}

void ParameterModel::build()
{
    Parameter * par;
    int id = 0;
    int mid = -1;

    par = new Parameter(new Unit::Temperature(), id++);
    par->setName("Temperature");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Temperature(), id++, false);
    par->setName("Reference Temperature");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Density(), mid--);
    par->setName("Density");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Viscosity(), mid--);
    par->setName("Viscosity");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Pressure(), id++);
    par->setName("Young's Modulus");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Pressure(), id++);
    par->setName("Young's Modulus X direction");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Pressure(), id++);
    par->setName("Young's Modulus Y direction");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Pressure(), id++);
    par->setName("Young's Modulus Z direction");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Unitless(), id++);
    par->setName("Poisson's Ratio");
    addParameter(par);

    par = new Parameter(new Unit::Unitless(), id++);
    par->setName("Poisson's Ratio XY");
    addParameter(par);

    par = new Parameter(new Unit::Unitless(), id++);
    par->setName("Poisson's Ratio YZ");
    addParameter(par);

    par = new Parameter(new Unit::Unitless(), id++);
    par->setName("Poisson's Ratio XZ");
    addParameter(par);

    par = new Parameter(new Unit::Pressure(), id++);
    par->setName("Shear Modulus");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Pressure(), id++);
    par->setName("Shear Modulus XY");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Pressure(), id++);
    par->setName("Shear Modulus YZ");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Pressure(), id++);
    par->setName("Shear Modulus XZ");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Pressure(), id++);
    par->setName("Bulk Modulus");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::ThermalConductivity(), id++);
    par->setName("Thermal Conductivity");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::ThermalConductivity(), id++);
    par->setName("Thermal Conductivity X direction");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::ThermalConductivity(), id++);
    par->setName("Thermal Conductivity Y direction");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::ThermalConductivity(), id++);
    par->setName("Thermal Conductivity Z direction");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::CoefficientOfThermalExpansion(), id++);
    par->setName("Coefficient of Thermal Expansion");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::CoefficientOfThermalExpansion(), id++);
    par->setName("Coefficient of Thermal Expansion X direction");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::CoefficientOfThermalExpansion(), id++);
    par->setName("Coefficient of Thermal Expansion Y direction");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::CoefficientOfThermalExpansion(), id++);
    par->setName("Coefficient of Thermal Expansion Z direction");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Resistivity(), id++);
    par->setName("Resistivity");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Resistivity(), id++);
    par->setName("Resistivity X direction");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Resistivity(), id++);
    par->setName("Resistivity Y direction");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Resistivity(), id++);
    par->setName("Resistivity Z direction");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::SpecificHeat(), id++);
    par->setName("Specific Heat");
    par->setPrefferedValueUnit();
    addParameter(par);
}

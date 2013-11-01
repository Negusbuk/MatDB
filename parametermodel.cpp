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

    par = new Parameter(new Unit::Temperature(), id++);
    par->setName("Temperature");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Density(), -1);
    par->setName("Density");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Pressure(), id++);
    par->setName("Young's Modulus");
    par->setPrefferedValueUnit();
    addParameter(par);

    par = new Parameter(new Unit::Unitless(), id++);
    par->setName("Poisson's Ratio");
    addParameter(par);

    par = new Parameter(new Unit::Pressure(), id++);
    par->setName("Shear Modulus");
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

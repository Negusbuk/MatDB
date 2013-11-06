#include <iostream>
#include <cmath>

#include <QRadioButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

#include "specificheatproperty.h"

SpecificHeatProperty::SpecificHeatProperty(ParameterModel* model, int id) :
    Property(id)
{
    setName("Specific Heat");
    setCategory(ThermalProperty);
    setType(SpecificHeat);
    setBehavior(Isotropic);
    Parameter *par = model->getParameter("Specific Heat");
    addParameter(par->clone());
}

SpecificHeatProperty::SpecificHeatProperty(const SpecificHeatProperty& property) :
    Property(property.getId())
{
    setName("Specific Heat");
    setCategory(ThermalProperty);
    setType(SpecificHeat);
    setBehavior(Isotropic);
    const Parameter *par = property.getParameter("Specific Heat");
    addParameter(par->clone());
}

Property* SpecificHeatProperty::clone(ParameterModel* model)
{
    SpecificHeatProperty* prop;
    if (model) {
        prop = new SpecificHeatProperty(model, getId());
    } else {
        prop = new SpecificHeatProperty(*this);
    }

    prop->setSorting(getSorting());

    return prop;
}

//<PropertyData property="pr0">
//  <Data format="float">6e-08</Data>
//  <Qualifier name="Variable Type">Dependent</Qualifier>
//  <ParameterValue parameter="pa0" format="float">
//    <Data>7.88860905221012e-31</Data>
//    <Qualifier name="Variable Type">Independent</Qualifier>
//  </ParameterValue>
//</PropertyData>
void SpecificHeatProperty::apply(PropertyData& data,
                            PropertyDetail& detail,
                            std::map<QString,ParameterDetail> paramMap)
{
    std::cout << "SpecificHeatProperty::apply" << std::endl;

    PValue pt = data.pvalues.front();
    std::vector<double> values;
    std::vector<double> temperatures;

    if (data.data.contains(',')) {
        QStringList l = data.data.split(',');
        for (QStringList::Iterator it = l.begin();
             it!=l.end();
             ++it) {
            values.push_back((*it).toDouble());
        }
    } else {
        values.push_back(data.data.toDouble());
    }

    if (pt.data.contains(',')) {
        QStringList l = pt.data.split(',');
        for (QStringList::Iterator it = l.begin();
             it!=l.end();
             ++it) {
            temperatures.push_back((*it).toDouble());
        }
    } else {
        temperatures.push_back(pt.data.toDouble());
    }

    Parameter * param = getParameter(detail.name);
    param = getParameter(detail.name);
    param->setValueUnit(detail.unit);

    std::vector<double>::iterator itt = temperatures.begin();
    for (std::vector<double>::iterator itv = values.begin();
         itv!=values.end() && itt!=temperatures.end();
         ++itv,++itt) {
        double temp = *itt;
        double value = *itv;
        if (value!=undefindedIdentifyer()) {
            value = param->getValueUnit()->convertToPreffered(*itv);
            if (temp==undefindedIdentifyer()) {
                param->addValue(value);
            } else {
                param->addValue(temp, value);
            }
        }
    }

    param->setPrefferedValueUnit();
}

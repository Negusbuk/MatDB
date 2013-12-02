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

#ifndef ELASTICITYPROPERTY_H
#define ELASTICITYPROPERTY_H

#include <QString>
#include <QButtonGroup>

#include <propertyspecialwidget.h>
#include <property.h>

class IsotropicElasticityPropertyWidget;

class IsotropicElasticityProperty : public Property
{
public:

    enum CalculationMode {
        CalcFromYoungsModulusAndPoissonsRatio  = 0,
        CalcFromYoungsModulusAndShearModulus  = 1,
        CalcFromPoissonsRatioAndShearModulus   = 2
    };

    IsotropicElasticityProperty(PropertyModel* propmodel,
                                ParameterModel* paramodel, int id);
    IsotropicElasticityProperty(const IsotropicElasticityProperty&);

    bool hasSpecialWidget() { return true; }
    void fillSpecialWidget();
    PropertySpecialWidget * getSpecialWidget(QWidget * parent=0);

    CalculationMode getCalculationMode() const { return CalculationMode_; }
    void setCalculationMode(CalculationMode mode);
    void recalculate();

    Property* clone(PropertyModel* propmodel = 0,
                    ParameterModel* paramodel = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
    virtual void writeHTML(QXmlStreamWriter& stream);

protected:
    IsotropicElasticityPropertyWidget* widget_;
    CalculationMode CalculationMode_;
    void recalculateFromYoungsModulusAndPoissonsRatio();
    void recalculateFromYoungsModulusAndShearModulus();
    void recalculateFromPoissonsRatioAndShearModulus();
};

class IsotropicElasticityPropertyWidget : public PropertySpecialWidget
{
    Q_OBJECT
public:
    explicit IsotropicElasticityPropertyWidget(QWidget * parent=0);
    IsotropicElasticityProperty* Property_;
public slots:
    void modeChanged(int id);
    void updateContents();
protected:
    QButtonGroup * buttonGroup_;
};

class OrthotropicElasticityProperty : public Property
{
public:
    OrthotropicElasticityProperty(PropertyModel* propmodel,
                                  ParameterModel* paramodel, int id);
    OrthotropicElasticityProperty(const OrthotropicElasticityProperty&);

    Property* clone(PropertyModel* propmodel = 0,
                    ParameterModel* paramodel = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
    virtual void writeHTML(QXmlStreamWriter& stream);
};

#endif // ELASTICITYPROPERTY_H

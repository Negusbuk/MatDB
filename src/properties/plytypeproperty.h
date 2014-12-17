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

#ifndef PLYTYPEPROPERTY_H
#define PLYTYPEPROPERTY_H

#include <QString>
#include <QComboBox>

#include <propertyspecialwidget.h>
#include <property.h>

class PlyTypePropertyWidget;

class PlyTypeProperty : public Property
{
public:

    enum PlyTypeId {
        Regular                    = 0,
        Woven                      = 1,
        Isotropic                  = 2,
        IsotropicHomogeneousCore   = 3,
        OrthotropicHomogeneousCore = 4,
        HoneycombCore              = 5
    };

    PlyTypeProperty(PropertyModel* propmodel,
                    ParameterModel* paramodel, int id);
    PlyTypeProperty(const PlyTypeProperty&);

    bool hasSpecialWidget() { return true; }
    void fillSpecialWidget();
    PropertySpecialWidget * getSpecialWidget(QWidget * parent=0);

    PlyTypeId getPlyType() const { return PlyType_; }
    void setPlyType(PlyTypeId type) { PlyType_ = type; }

    Property* clone(PropertyModel* propmodel = 0,
                    ParameterModel* paramodel = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
    virtual void writeHTML(QXmlStreamWriter& stream);

protected:
    PlyTypePropertyWidget* widget_;
    PlyTypeId PlyType_;
};

class PlyTypePropertyWidget : public PropertySpecialWidget
{
    Q_OBJECT
public:
    explicit PlyTypePropertyWidget(QWidget * parent=0);
    PlyTypeProperty* Property_;
public slots:
    void typeChanged(int id);
    void updateContents();
protected:
    QComboBox * typeComboBox_;
};

#endif // PLYTYPEPROPERTY_H

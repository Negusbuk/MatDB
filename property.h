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

#ifndef PROPERTY_H
#define PROPERTY_H

#include <map>

#include <QString>
#include <QWidget>
#include <QGroupBox>
#include <QButtonGroup>
#include <QDomDocument>
#include <QDomElement>
#include <QXmlStreamWriter>

#include <parameter.h>
#include <parametermodel.h>

#include <propertyspecialwidget.h>

struct PropertyDetail {
    QString id;
    QString name;
    QString unit;
};

struct ParameterDetail {
    QString id;
    QString name;
    QString unit;
};

struct PValue {
    QString parameter;
    QString format;
    QString data;
    QString qualifiertype;
    QString qualifier;
};

struct PropertyData {
    QString property;
    QString dataformat;
    QString data;
    std::map<QString,QString> qualifiers;
    std::vector<PValue> pvalues;
};

class PropertyModel;

class Property
{
public:
    Property(int id);
    virtual ~Property();

    enum Category {
        InvalidProperty           = 0,
        PhysicalProperty          = 1,
        LinearElasticProperty     = 2,
        ThermalProperty           = 3,
        ElectricalProperty        = 4,
        FluidProperty             = 5,
        NoCategory
    };

    enum Type {
        InvalidType                      =     0,
        ReferenceTemperature             =   100,

        Density                          =  1001,
        CoefficientOfThermalExpansion    =  1002,

        Elasticity                       =  2001,

        ThermalConductivity              =  3001,
        SpecificHeat                     =  3003,

        Resistivity                      =  4001,

        Viscosity                        =  5001,
        CriticalTemperature              =  5002,
        CriticalPressure                 =  5003,
        BoilingPoint                     =  5004
    };

    enum Behavior {
        Isotropic     = 0x0001,
        Orthotropic   = 0x0002,
        UnknownBehavior
    };

    enum Definition {
        Instantaneous = 0x0001,
        Secant        = 0x0002,
        UnknownDefinition
    };

    int getId() const { return Id_; }
    const QString& getIdString() const { return IdString_; }

    const QString& getName() const { return Name_; }
    void setName(const QString& name);

    const QString& getDisplayName() const { return DisplayName_; }
    void setDisplayName(const QString& name);

    Category getCategory() const { return Category_; }
    QString getCategoryName() const;
    void setCategory(Category cat) { Category_ = cat; }

    Type getType() const { return Type_; }
    QString getTypeName() const;
    void setType(Type type) { Type_ = type; }

    Behavior getBehavior() const { return Behavior_; }
    QString getBehaviorAsString() const;
    void setBehavior(Behavior behavior) { Behavior_ = behavior; }

    Definition getDefinition() const { return Definition_; }
    QString getDefinitionAsString() const;
    void setDefinition(Definition definition) { Definition_ = definition; }

    void addParameter(Parameter* parameter);
    Parameter* getParameter(const QString& name);
    const Parameter* getParameter(const QString& name) const;
    std::map<QString,Parameter*>& getParameters();
    std::vector<Parameter*>& getOrderedParameters();

    virtual bool hasSpecialWidget() { return false; }
    virtual void fillSpecialWidget() { }
    virtual PropertySpecialWidget * getSpecialWidget(QWidget * /*parent=0*/) { return 0; }
    virtual void recalculate() { }

    virtual Property* clone(PropertyModel* propmodel = 0,
                            ParameterModel* paramodel = 0) = 0;

    virtual void apply(PropertyData& /* data */,
                       PropertyDetail& /* detail */,
                       std::map<QString,ParameterDetail> /* paramMap */) { }

    void setSorting(int sorting) { sorting_ = sorting; }
    int getSorting() const { return sorting_; }

    void setModified() { modified_ = true; }
    bool isModified() const;

    virtual void write(QXmlStreamWriter& stream);
    virtual void read(const QDomElement& element);

    virtual void writeXML(QXmlStreamWriter& stream);
    virtual void writeHTML(QXmlStreamWriter& stream);
    virtual void writeXMLData(QXmlStreamWriter& stream);

    static double undefindedIdentifyer() { return 7.88860905221012e-31; }
    static QString undefindedIdentifyerAsString() { return "7.88860905221012e-31"; }

protected:
    int Id_;
    QString IdString_;
    QString Name_;
    QString DisplayName_;
    Category Category_;
    Type Type_;
    Behavior Behavior_;
    Definition Definition_;
    std::map<QString,Parameter*> Parameters_;
    std::vector<Parameter*> OrderedParameters_;
    int sorting_;
    bool modified_;
};

#endif // PROPERTY_H

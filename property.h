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
        ElectricalProperty        = 4
    };

    enum Type {
        InvalidType                      =     0,

        Density                          =  1001,

        Elasticity                       =  2001,

        ThermalConductivity              =  3001,
        CoefficientOfThermalExpansion    =  3002,
        SpecificHeat                     =  3003,

        Resistivity                      =  4001,
    };

    enum Behavior {
        Isotropic     = 0x0001,
        Orthotropic   = 0x0002,
        UnknownBehavior
    };

    enum Definition {
        Instantaneous = 0x0001,
        UnknownDefinition
    };

    int getId() const { return Id_; }
    const QString& getIdString() const { return IdString_; }

    const QString& getName() const { return Name_; }
    void setName(const QString& name);

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

    virtual bool hasSpecialWidget() { return false; }
    virtual void fillSpecialWidget() { }
    virtual QWidget * getSpecialWidget(QWidget * /*parent=0*/) { return 0; }
    virtual void recalculate() { }

    virtual Property* clone(ParameterModel* model = 0) = 0;

    virtual void apply(PropertyData& /* data */,
                       PropertyDetail& /* detail */,
                       std::map<QString,ParameterDetail> /* paramMap */) { }

    void setSorting(int sorting) { sorting_ = sorting; }
    int getSorting() const { return sorting_; }

    virtual void writeXML(QXmlStreamWriter& stream);
    virtual void writeXMLData(QXmlStreamWriter& stream);

    static double undefindedIdentifyer() { return 7.88860905221012e-31; }
    static QString undefindedIdentifyerAsString() { return "7.88860905221012e-31"; }

protected:
    int Id_;
    QString IdString_;
    QString Name_;
    Category Category_;
    Type Type_;
    Behavior Behavior_;
    Definition Definition_;
    std::map<QString,Parameter*> Parameters_;
    int sorting_;
};

#endif // PROPERTY_H

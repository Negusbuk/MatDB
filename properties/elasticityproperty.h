#ifndef ELASTICITYPROPERTY_H
#define ELASTICITYPROPERTY_H

#include <QString>
#include <QWidget>
#include <QGroupBox>
#include <QButtonGroup>

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

    IsotropicElasticityProperty(ParameterModel* model, int id);
    IsotropicElasticityProperty(const IsotropicElasticityProperty&);

    bool hasSpecialWidget() { return true; }
    void fillSpecialWidget();
    QWidget * getSpecialWidget(QWidget * parent=0);

    CalculationMode getCalculationMode() const { return CalculationMode_; }
    void setCalculationMode(CalculationMode mode) { CalculationMode_ = mode; }
    void recalculate();

    Property* clone(ParameterModel* model = 0);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);

protected:
    IsotropicElasticityPropertyWidget* widget_;
    CalculationMode CalculationMode_;
    void recalculateFromYoungsModulusAndPoissonsRatio();
    void recalculateFromYoungsModulusAndShearModulus();
    void recalculateFromPoissonsRatioAndShearModulus();
};

class IsotropicElasticityPropertyWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit IsotropicElasticityPropertyWidget(QWidget * parent=0);
    IsotropicElasticityProperty* Property_;
public slots:
    void modeChanged(int id);
};

class OrthotropicElasticityProperty : public Property
{
public:
    OrthotropicElasticityProperty(ParameterModel* model, int id);
    OrthotropicElasticityProperty(const OrthotropicElasticityProperty&);

    Property* clone(ParameterModel* model);

    virtual void apply(PropertyData& data,
                       PropertyDetail& detail,
                       std::map<QString,ParameterDetail> paramMap);

    virtual void writeXML(QXmlStreamWriter& stream);
};

#endif // ELASTICITYPROPERTY_H

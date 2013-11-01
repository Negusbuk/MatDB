#include <iostream>
#include <cmath>

#include <QRadioButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

#include "elasticityproperty.h"

IsotropicElasticityProperty::IsotropicElasticityProperty(ParameterModel* model, int id) :
    Property(id)
{
    setName("Isotropic Elasticity");
    setCategory(LinearElasticProperty);
    setType(Elasticity);
    setBehavior(Isotropic);
    Parameter *par;
    par = model->getParameter("Young's Modulus");
    addParameter(par->clone());
    par = model->getParameter("Poisson's Ratio");
    addParameter(par->clone());
    par = model->getParameter("Shear Modulus");
    addParameter(par->clone());
    par = model->getParameter("Bulk Modulus");
    addParameter(par->clone());

    widget_ = 0;
    CalculationMode_ = CalcFromYoungsModulusAndPoissonsRatio;
}


IsotropicElasticityProperty::IsotropicElasticityProperty(const IsotropicElasticityProperty& property) :
    Property(property.getId())
{
    setName("Isotropic Elasticity");
    setCategory(LinearElasticProperty);
    setType(Elasticity);
    setBehavior(Isotropic);
    const Parameter *par1 = property.getParameter("Young's Modulus");
    addParameter(par1->clone());
    const Parameter *par2 = property.getParameter("Poisson's Ratio");
    addParameter(par2->clone());
    const Parameter *par3 = property.getParameter("Shear Modulus");
    addParameter(par3->clone());
    const Parameter *par4 = property.getParameter("Bulk Modulus");
    addParameter(par4->clone());

    widget_ = 0;
    CalculationMode_ = CalcFromYoungsModulusAndPoissonsRatio;
}

Property* IsotropicElasticityProperty::clone(ParameterModel* model)
{
    IsotropicElasticityProperty* prop;
    if (model) {
        prop = new IsotropicElasticityProperty(model, getId());
    } else {
        prop = new IsotropicElasticityProperty(*this);
    }
    return prop;
}

//<PropertyData property="pr5">
//  <Data format="string">-</Data>
//  <Qualifier name="Behavior">Isotropic</Qualifier>
//  <Qualifier name="Derive from">Young's Modulus and Poisson's Ratio</Qualifier>
//  <ParameterValue parameter="pa11" format="float">
//    <Data>98000000000</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa12" format="float">
//    <Data>0.29</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa13" format="float">
//    <Data>77777777777.7778</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa14" format="float">
//    <Data>37984496124.031</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa0" format="float">
//    <Data>7.88860905221012e-31</Data>
//    <Qualifier name="Variable Type">Independent</Qualifier>
//  </ParameterValue>
//</PropertyData>
void IsotropicElasticityProperty::apply(PropertyData& data,
                                        PropertyDetail& detail,
                                        std::map<QString,ParameterDetail> paramMap)
{
    // std::cout << "IsotropicElasticityProperty::apply" << std::endl;

    ParameterDetail details[5];
    Parameter *param[5];
    std::vector<double> values[5];
    std::vector<double> vtt;

    std::map<QString,QString>::iterator itFind = data.qualifiers.find("Derive from");
    if (itFind!=data.qualifiers.end()) {
        //std::cout << itFind->second.toStdString() << std::endl;
        if (itFind->second=="Young's Modulus and Poisson's Ratio") {
            CalculationMode_ = CalcFromYoungsModulusAndPoissonsRatio;
        } else {
            //std::cout << "XXXXXXXXXXXXXX HELP ME" << std::endl;
        }
    } else {
        CalculationMode_ = CalcFromYoungsModulusAndPoissonsRatio;
    }

    int i=0;
    for (std::vector<PValue>::iterator it = data.pvalues.begin();
         it!=data.pvalues.end();
         ++it) {
        details[i] = paramMap[it->parameter];
        param[i] = getParameter(details[i].name);
        //param[i]->setId(details[i].id);

        if (it->data.contains(',')) {
            QStringList l = it->data.split(',');
            for (QStringList::Iterator itl = l.begin();
                 itl!=l.end();
                 ++itl) {
                values[i].push_back((*itl).toDouble());
            }
        } else {
            values[i].push_back(it->data.toDouble());
        }

        if (param[i]==0) vtt = values[i];

        ++i;
    }

    for (i=0;i<5;++i) {
        if (!param[i]) continue;

        param[i]->setValueUnit(details[i].unit);

        std::vector<double>::iterator itt = vtt.begin();
        for (std::vector<double>::iterator itv = values[i].begin();
             itv!=values[i].end() && itt!=vtt.end();
             ++itv,++itt) {
            double temp = *itt;
            double value = *itv;
            if (value!=undefindedIdentifyer()) {
                value = param[i]->getValueUnit()->convertToPrefferedUnit(*itv);
                if (temp==undefindedIdentifyer()) {
                    param[i]->addValue(value);
                } else {
                    param[i]->addValue(temp, value);
                }
            }
        }

        param[i]->setPrefferedValueUnit();
    }
}

void IsotropicElasticityProperty::fillSpecialWidget()
{
    if (!widget_) return;
    widget_->Property_ = this;
}

QWidget * IsotropicElasticityProperty::getSpecialWidget(QWidget * parent)
{
    if (!widget_) {
        widget_ = new IsotropicElasticityPropertyWidget(parent);
    }
    widget_->Property_ = this;

    return widget_;
}

void IsotropicElasticityProperty::recalculate()
{
    std::cout << "IsotropicElasticityProperty::recalculate()" << std::endl;

    if (CalculationMode_==CalcFromYoungsModulusAndPoissonsRatio) {
        recalculateFromYoungsModulusAndPoissonsRatio();
    } else if (CalculationMode_==CalcFromYoungsModulusAndShearModulus) {
        recalculateFromYoungsModulusAndShearModulus();
    } else if (CalculationMode_==CalcFromPoissonsRatioAndShearModulus) {
        recalculateFromPoissonsRatioAndShearModulus();
    }
}

void IsotropicElasticityProperty::recalculateFromYoungsModulusAndPoissonsRatio()
{
    std::cout << "IsotropicElasticityProperty::recalculateFromYoungsModulusAndPoissonsRatio()" << std::endl;

    Parameter * parY = getParameter("Young's Modulus");
    Parameter * parP = getParameter("Poisson's Ratio");
    Parameter * parS = getParameter("Shear Modulus");

    if (!(parY && parP && parS)) return;

    parS->clear();

    int nY = parY->getNumberOfValues();
    int nP = parP->getNumberOfValues();
    for (int i=0;i<nY&&i<nP;i++) {
        const ParameterValue& pY = parY->getValues().at(i);
        const ParameterValue& pP = parP->getValues().at(i);
        double Y = parY->getValueUnit()->convert(pY.getValue(), 0);
        double P = pP.getValue();
        double S = Y * P;

        if (pY.isTemperatureValid() && pP.isTemperatureValid()) {
            parS->addValue(pY.getTemperature(), S);
        } else {
            parS->addValue(S);
        }
        parS->getValueUnit()->setCurrentUnitIndex(0);
    }
}

void IsotropicElasticityProperty::recalculateFromYoungsModulusAndShearModulus()
{
    std::cout << "IsotropicElasticityProperty::recalculateFromYoungsModulusAndShearModulus()" << std::endl;

    Parameter * parY = getParameter("Young's Modulus");
    Parameter * parP = getParameter("Poisson's Ratio");
    Parameter * parS = getParameter("Shear Modulus");
}

void IsotropicElasticityProperty::recalculateFromPoissonsRatioAndShearModulus()
{
    std::cout << "IsotropicElasticityProperty::recalculateFromPoissonRatioAndShearModulus()" << std::endl;

    Parameter * parY = getParameter("Young's Modulus");
    Parameter * parP = getParameter("Poisson's Ratio");
    Parameter * parS = getParameter("Shear Modulus");
}

void IsotropicElasticityProperty::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Elasticity");
    stream.writeEndElement();
}

IsotropicElasticityPropertyWidget::IsotropicElasticityPropertyWidget(QWidget * parent) :
    QGroupBox(parent)
{
    QButtonGroup * bg = new QButtonGroup(this);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    QRadioButton * rb;
    rb = new QRadioButton("Calculated from Young's Modulus and Poisson Ratio", this);
    vbox->addWidget(rb);
    bg->addButton(rb, 0);
    rb->setChecked(true);
    rb = new QRadioButton("Calculated from Young's Modulus and Shear Modulus", this);
    vbox->addWidget(rb);
    bg->addButton(rb, 1);
    rb = new QRadioButton("Calculated from Poisson Ratio and Shear Modulus", this);
    vbox->addWidget(rb);
    bg->addButton(rb, 2);

    vbox->addStretch(1);

    setLayout(vbox);

    connect(bg,SIGNAL(buttonClicked(int)),
            this,SLOT(modeChanged(int)));
}

void IsotropicElasticityPropertyWidget::modeChanged(int id)
{
    std::cout << "clicked " << id << std::endl;
    Property_->setCalculationMode(static_cast<IsotropicElasticityProperty::CalculationMode>(id));
    Property_->recalculate();
}

OrthotropicElasticityProperty::OrthotropicElasticityProperty(ParameterModel* model, int id) :
    Property(id)
{
    setName("Orthotropic Elasticity");
    setCategory(LinearElasticProperty);
    setType(Elasticity);
    setBehavior(Orthotropic);
    Parameter *par;
    par = model->getParameter("Young's Modulus");
    addParameter(par->clone());
    par = model->getParameter("Poisson's Ratio");
    addParameter(par->clone());
    par = model->getParameter("Shear Modulus");
    addParameter(par->clone());
    par = model->getParameter("Bulk Modulus");
    addParameter(par->clone());
}

OrthotropicElasticityProperty::OrthotropicElasticityProperty(const OrthotropicElasticityProperty& property) :
    Property(property.getId())
{
    setName("Orthotropic Elasticity");
    setCategory(LinearElasticProperty);
    setType(Elasticity);
    setBehavior(Orthotropic);
    const Parameter *par1 = property.getParameter("Young's Modulus");
    addParameter(par1->clone());
    const Parameter *par2 = property.getParameter("Poisson's Ratio");
    addParameter(par2->clone());
    const Parameter *par3 = property.getParameter("Shear Modulus");
    addParameter(par3->clone());
    const Parameter *par4 = property.getParameter("Bulk Modulus");
    addParameter(par4->clone());
}

Property* OrthotropicElasticityProperty::clone(ParameterModel* model)
{
    OrthotropicElasticityProperty* prop = new OrthotropicElasticityProperty(model, getId());
    if (model) {
        prop = new OrthotropicElasticityProperty(model, getId());
    } else {
        prop = new OrthotropicElasticityProperty(*this);
    }
    prop->setSorting(getSorting());
    return prop;
}

void OrthotropicElasticityProperty::apply(PropertyData& data,
                                          PropertyDetail& detail,
                                          std::map<QString,ParameterDetail> paramMap)
{

}

void OrthotropicElasticityProperty::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Elasticity");
    stream.writeEndElement();
}

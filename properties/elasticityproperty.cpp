#include <iostream>
#include <cmath>

#include <QRadioButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

#include "elasticityproperty.h"
#include <nqlogger.h>


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
    setCalculationMode(CalcFromYoungsModulusAndPoissonsRatio);
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
    setCalculationMode(CalcFromYoungsModulusAndPoissonsRatio);
}

Property* IsotropicElasticityProperty::clone(ParameterModel* model)
{
    IsotropicElasticityProperty* prop;
    if (model) {
        prop = new IsotropicElasticityProperty(model, getId());
    } else {
        prop = new IsotropicElasticityProperty(*this);
    }

    prop->widget_ = widget_;

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
                value = param[i]->getValueUnit()->convertToPreffered(*itv);
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
    widget_->updateContents();
}

PropertySpecialWidget * IsotropicElasticityProperty::getSpecialWidget(QWidget * parent)
{
    if (!widget_) {
        widget_ = new IsotropicElasticityPropertyWidget(parent);
    }
    widget_->Property_ = this;

    return widget_;
}

void IsotropicElasticityProperty::setCalculationMode(CalculationMode mode)
{
    CalculationMode_ = mode;

    Parameter * parE = getParameter("Young's Modulus");
    Parameter * parNu = getParameter("Poisson's Ratio");
    Parameter * parG = getParameter("Shear Modulus");
    Parameter * parK = getParameter("Bulk Modulus");

    parE->setDependent(true);
    parNu->setDependent(true);
    parG->setDependent(true);
    parK->setDependent(true);

    if (CalculationMode_==CalcFromYoungsModulusAndPoissonsRatio) {
        parE->setDependent(false);
        parNu->setDependent(false);
    } else if (CalculationMode_==CalcFromYoungsModulusAndShearModulus) {
        parE->setDependent(false);
        parG->setDependent(false);
    } else if (CalculationMode_==CalcFromPoissonsRatioAndShearModulus) {
        parNu->setDependent(false);
        parG->setDependent(false);
    }
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
    NQLog("IsotropicElasticityProperty", NQLog::Spam) << "void recalculateFromYoungsModulusAndPoissonsRatio()";

    Parameter * parE = getParameter("Young's Modulus");
    Parameter * parNu = getParameter("Poisson's Ratio");
    Parameter * parG = getParameter("Shear Modulus");
    Parameter * parK = getParameter("Bulk Modulus");

    if (!(parE && parNu && parG && parK)) return;

    parG->clear();
    parK->clear();

    int nE = parE->getNumberOfValues();
    int nNu = parNu->getNumberOfValues();

    for (int i=0;i<nE&&i<nNu;i++) {
        const ParameterValue& pE = parE->getValues().at(i);
        const ParameterValue& pNu = parNu->getValues().at(i);
        double E = parE->getValueUnit()->convertToBase(pE.getValue());
        double Nu = parNu->getValueUnit()->convertToBase(pNu.getValue());

        NQLog("IsotropicElasticityProperty", NQLog::Spam) << "E =  " << E;
        NQLog("IsotropicElasticityProperty", NQLog::Spam) << "Nu = " << Nu;

        double G = parG->getValueUnit()->convertToCurrent(E/2/(1+Nu));
        double K = parK->getValueUnit()->convertToCurrent(E/3/(1-2*Nu));

        NQLog("IsotropicElasticityProperty", NQLog::Spam) << "G =  " << G;
        NQLog("IsotropicElasticityProperty", NQLog::Spam) << "K =  " << K;

        if (pE.isTemperatureValid() && pNu.isTemperatureValid()) {
            parG->addValue(pE.getTemperature(), G);
            parK->addValue(pE.getTemperature(), K);
        } else {
            parG->addValue(G);
            parK->addValue(K);
        }
    }
}

void IsotropicElasticityProperty::recalculateFromYoungsModulusAndShearModulus()
{
    NQLog("IsotropicElasticityProperty", NQLog::Spam) << "void recalculateFromYoungsModulusAndShearModulus()";

    Parameter * parE = getParameter("Young's Modulus");
    Parameter * parNu = getParameter("Poisson's Ratio");
    Parameter * parG = getParameter("Shear Modulus");
    Parameter * parK = getParameter("Bulk Modulus");

    if (!(parE && parNu && parG && parK)) return;

    parNu->clear();
    parK->clear();

    int nE = parE->getNumberOfValues();
    int nG = parG->getNumberOfValues();

    for (int i=0;i<nE&&i<nG;i++) {
        const ParameterValue& pE = parE->getValues().at(i);
        const ParameterValue& pG = parG->getValues().at(i);
        double E = parE->getValueUnit()->convertToBase(pE.getValue());
        double G = parG->getValueUnit()->convertToBase(pG.getValue());

        NQLog("IsotropicElasticityProperty", NQLog::Spam) << "E =  " << E;
        NQLog("IsotropicElasticityProperty", NQLog::Spam) << "G =  " << G;

        double Nu = parNu->getValueUnit()->convertToCurrent(E/(2*G)-1);
        double K = parK->getValueUnit()->convertToCurrent(E*G/(3*(3*G-E)));

        NQLog("IsotropicElasticityProperty", NQLog::Spam) << "Nu = " << Nu;
        NQLog("IsotropicElasticityProperty", NQLog::Spam) << "K =  " << K;

        if (pE.isTemperatureValid() && pG.isTemperatureValid()) {
            parNu->addValue(pE.getTemperature(), Nu);
            parK->addValue(pE.getTemperature(), K);
        } else {
            parNu->addValue(Nu);
            parK->addValue(K);
        }
    }
}

void IsotropicElasticityProperty::recalculateFromPoissonsRatioAndShearModulus()
{
    NQLog("IsotropicElasticityProperty", NQLog::Spam) << "void recalculateFromPoissonRatioAndShearModulus()";

    Parameter * parE = getParameter("Young's Modulus");
    Parameter * parNu = getParameter("Poisson's Ratio");
    Parameter * parG = getParameter("Shear Modulus");
    Parameter * parK = getParameter("Bulk Modulus");

    if (!(parE && parNu && parG && parK)) return;

    parE->clear();
    parK->clear();

    int nNu = parNu->getNumberOfValues();
    int nG = parG->getNumberOfValues();

    for (int i=0;i<nNu&&i<nG;i++) {
        const ParameterValue& pNu = parNu->getValues().at(i);
        const ParameterValue& pG = parG->getValues().at(i);
        double Nu = parNu->getValueUnit()->convertToBase(pNu.getValue());
        double G = parG->getValueUnit()->convertToBase(pG.getValue());

        NQLog("IsotropicElasticityProperty", NQLog::Spam) << "Nu = " << Nu;
        NQLog("IsotropicElasticityProperty", NQLog::Spam) << "G =  " << G;

        double E = parE->getValueUnit()->convertToCurrent(2*G*(1+Nu));
        double K = parK->getValueUnit()->convertToCurrent(2*G*(1+Nu)/(3*(1-2*Nu)));

        NQLog("IsotropicElasticityProperty", NQLog::Spam) << "E =  " << E;
        NQLog("IsotropicElasticityProperty", NQLog::Spam) << "K =  " << K;

        if (pNu.isTemperatureValid() && pG.isTemperatureValid()) {
            parE->addValue(pNu.getTemperature(), E);
            parK->addValue(pNu.getTemperature(), K);
        } else {
            parE->addValue(E);
            parK->addValue(K);
        }
    }
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
    PropertySpecialWidget(parent)
{
    buttonGroup_ = new QButtonGroup(this);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    QRadioButton * rb;
    rb = new QRadioButton("Calculated from Young's Modulus and Poisson Ratio", this);
    vbox->addWidget(rb);
    buttonGroup_->addButton(rb, 0);
    rb->setChecked(true);
    rb = new QRadioButton("Calculated from Young's Modulus and Shear Modulus", this);
    vbox->addWidget(rb);
    buttonGroup_->addButton(rb, 1);
    rb = new QRadioButton("Calculated from Poisson Ratio and Shear Modulus", this);
    vbox->addWidget(rb);
    buttonGroup_->addButton(rb, 2);

    vbox->addStretch(1);

    setLayout(vbox);

    connect(buttonGroup_, SIGNAL(buttonClicked(int)),
            this, SLOT(modeChanged(int)));
}

void IsotropicElasticityPropertyWidget::modeChanged(int id)
{
    Property_->setCalculationMode(static_cast<IsotropicElasticityProperty::CalculationMode>(id));
    Property_->recalculate();

    emit modified();
}

void IsotropicElasticityPropertyWidget::updateContents()
{
    int mode = Property_->getCalculationMode();
    buttonGroup_->button(mode)->setChecked(true);
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
    OrthotropicElasticityProperty* prop;
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

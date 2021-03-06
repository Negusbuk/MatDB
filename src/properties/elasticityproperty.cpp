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

#include <cmath>

#include <QRadioButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

#include <nqlogger.h>

#include <elasticityproperty.h>

IsotropicElasticityProperty::IsotropicElasticityProperty(PropertyModel * /* propmodel */,
                                                         ParameterModel* paramodel, int id) :
    Property(id)
{
    setName("Isotropic Elasticity");
    setDisplayName(QObject::tr("Isotropic Elasticity"));
    setCategory(LinearElasticProperty);
    setType(Elasticity);
    setBehavior(Isotropic);
    Parameter *par;
    par = paramodel->getParameter("Young's Modulus");
    addParameter(par->clone());
    par = paramodel->getParameter("Poisson's Ratio");
    addParameter(par->clone());
    par = paramodel->getParameter("Shear Modulus");
    addParameter(par->clone());
    par = paramodel->getParameter("Bulk Modulus");
    addParameter(par->clone());

    widget_ = 0;
    setCalculationMode(CalcFromYoungsModulusAndPoissonsRatio);
}

IsotropicElasticityProperty::IsotropicElasticityProperty(const IsotropicElasticityProperty& property) :
    Property(property.getId())
{
    setName("Isotropic Elasticity");
    setDisplayName(QObject::tr("Isotropic Elasticity"));
    setCategory(LinearElasticProperty);
    setType(Elasticity);
    setBehavior(Isotropic);
    const Parameter *par1 = property.getParameter("Young's Modulus");
    addParameter(par1->cloneWithData());
    const Parameter *par2 = property.getParameter("Poisson's Ratio");
    addParameter(par2->cloneWithData());
    const Parameter *par3 = property.getParameter("Shear Modulus");
    addParameter(par3->cloneWithData());
    const Parameter *par4 = property.getParameter("Bulk Modulus");
    addParameter(par4->cloneWithData());

    widget_ = 0;
    setCalculationMode(CalcFromYoungsModulusAndPoissonsRatio);
}

Property* IsotropicElasticityProperty::clone(PropertyModel* propmodel,
                                             ParameterModel* paramodel)
{
    IsotropicElasticityProperty* prop;
    if (propmodel && paramodel) {
        prop = new IsotropicElasticityProperty(propmodel, paramodel, getId());
    } else {
        prop = new IsotropicElasticityProperty(*this);
    }

    prop->widget_ = widget_;

    prop->setSorting(getSorting());

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
                                        PropertyDetail& /* detail */,
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
    NQLog("Property", NQLog::Message) << "IsotropicElasticityProperty::recalculate()";

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


void IsotropicElasticityProperty::writeHTML(QXmlStreamWriter& stream)
{
    Parameter * parE = getParameter("Young's Modulus");
    Parameter * parNu = getParameter("Poisson's Ratio");
    Parameter * parG = getParameter("Shear Modulus");
    Parameter * parK = getParameter("Bulk Modulus");

    stream.writeStartElement("tr");

    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBTitle");
    stream.writeAttribute("valign", "top");
    stream.writeCharacters(getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("colspan", "2");
    stream.writeAttribute("align", "right");

    stream.writeStartElement("table");
    //stream.writeAttribute("style", "border-top:1px solid #000;");
    stream.writeAttribute("rules", "groups");
    //stream.writeAttribute("class", "MatDBTable");

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters(parE->getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    parE->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters(parNu->getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    parNu->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters(parG->getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    parG->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters(parK->getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    parK->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr

    stream.writeEndElement(); // table

    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
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

OrthotropicElasticityProperty::OrthotropicElasticityProperty(PropertyModel* /* propmodel */,
                                                             ParameterModel* paramodel, int id) :
    Property(id)
{
    setName("Orthotropic Elasticity");
    setDisplayName(QObject::tr("Orthotropic Elasticity"));
    setCategory(LinearElasticProperty);
    setType(Elasticity);
    setBehavior(Orthotropic);
    Parameter *par;
    par = paramodel->getParameter("Young's Modulus X direction");
    addParameter(par->clone());
    par = paramodel->getParameter("Young's Modulus Y direction");
    addParameter(par->clone());
    par = paramodel->getParameter("Young's Modulus Z direction");
    addParameter(par->clone());
    par = paramodel->getParameter("Poisson's Ratio XY");
    addParameter(par->clone());
    par = paramodel->getParameter("Poisson's Ratio YZ");
    addParameter(par->clone());
    par = paramodel->getParameter("Poisson's Ratio XZ");
    addParameter(par->clone());
    par = paramodel->getParameter("Shear Modulus XY");
    addParameter(par->clone());
    par = paramodel->getParameter("Shear Modulus YZ");
    addParameter(par->clone());
    par = paramodel->getParameter("Shear Modulus XZ");
    addParameter(par->clone());
}

OrthotropicElasticityProperty::OrthotropicElasticityProperty(const OrthotropicElasticityProperty& property) :
    Property(property.getId())
{
    setName("Orthotropic Elasticity");
    setDisplayName(QObject::tr("Orthotropic Elasticity"));
    setCategory(LinearElasticProperty);
    setType(Elasticity);
    setBehavior(Orthotropic);
    const Parameter *par1x = property.getParameter("Young's Modulus X direction");
    addParameter(par1x->clone());
    const Parameter *par1y = property.getParameter("Young's Modulus Y direction");
    addParameter(par1y->clone());
    const Parameter *par1z = property.getParameter("Young's Modulus Z direction");
    addParameter(par1z->clone());
    const Parameter *par2xy = property.getParameter("Poisson's Ratio XY");
    addParameter(par2xy->clone());
    const Parameter *par2yz = property.getParameter("Poisson's Ratio YZ");
    addParameter(par2yz->clone());
    const Parameter *par2xz = property.getParameter("Poisson's Ratio XZ");
    addParameter(par2xz->clone());
    const Parameter *par3xy = property.getParameter("Shear Modulus XY");
    addParameter(par3xy->clone());
    const Parameter *par3yz = property.getParameter("Shear Modulus YZ");
    addParameter(par3yz->clone());
    const Parameter *par3xz = property.getParameter("Shear Modulus XZ");
    addParameter(par3xz->clone());
}

Property* OrthotropicElasticityProperty::clone(PropertyModel* propmodel,
                                               ParameterModel* paramodel)
{
    OrthotropicElasticityProperty* prop;
    if (propmodel && paramodel) {
        prop = new OrthotropicElasticityProperty(propmodel, paramodel, getId());
    } else {
        prop = new OrthotropicElasticityProperty(*this);
    }

    prop->setSorting(getSorting());

    return prop;
}

//<PropertyData property="pr3">
//  <Data format="string">-</Data>
//  <Qualifier name="Behavior">Orthotropic</Qualifier>
//  <ParameterValue parameter="pa7" format="float">
//    <Data>50000000000</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa8" format="float">
//    <Data>50000000000</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa9" format="float">
//    <Data>1000000</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa10" format="float">
//    <Data>0.19</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa11" format="float">
//    <Data>0.19</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa12" format="float">
//    <Data>0.19</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa13" format="float">
//    <Data>420200</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa14" format="float">
//    <Data>21010000000</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa15" format="float">
//    <Data>21010000000</Data>
//    <Qualifier name="Variable Type">Dependent</Qualifier>
//  </ParameterValue>
//  <ParameterValue parameter="pa0" format="float">
//    <Data>7.88860905221012e-31</Data>
//    <Qualifier name="Variable Type">Independent</Qualifier>
//  </ParameterValue>
//</PropertyData>
void OrthotropicElasticityProperty::apply(PropertyData& data,
                                          PropertyDetail& /* detail */,
                                          std::map<QString,ParameterDetail> paramMap)
{
    // std::cout << "IsotropicElasticityProperty::apply" << std::endl;

    ParameterDetail details[10];
    Parameter *param[10];
    std::vector<double> values[10];
    std::vector<double> vtt;

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

    for (i=0;i<10;++i) {
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

void OrthotropicElasticityProperty::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Elasticity");
    stream.writeEndElement();
}

void OrthotropicElasticityProperty::writeHTML(QXmlStreamWriter& stream)
{
    Parameter * par;

    stream.writeStartElement("tr");

    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBTitle");
    stream.writeAttribute("valign", "top");
    stream.writeCharacters(getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("colspan", "2");
    stream.writeAttribute("align", "right");

    stream.writeStartElement("table");
    //stream.writeAttribute("style", "border-top:1px solid #000;");
    stream.writeAttribute("rules", "groups");
    //stream.writeAttribute("class", "MatDBTable");

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    par = getParameter("Young's Modulus X direction");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters("Young's Modulus X");
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    par->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    par = getParameter("Young's Modulus Y direction");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters("Young's Modulus Y");
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    par->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    par = getParameter("Young's Modulus Z direction");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters("Young's Modulus Z");
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    par->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    par = getParameter("Poisson's Ratio XY");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters(par->getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    par->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    par = getParameter("Poisson's Ratio YZ");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters(par->getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    par->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    par = getParameter("Poisson's Ratio XZ");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters(par->getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    par->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    par = getParameter("Shear Modulus XY");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters(par->getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    par->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    par = getParameter("Shear Modulus YZ");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters(par->getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    par->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeStartElement("tbody");
    stream.writeStartElement("tr");

    par = getParameter("Shear Modulus XZ");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBParameterName");
    stream.writeCharacters(par->getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    par->writeHTML(stream);
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // tbody

    stream.writeEndElement(); // table

    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
}

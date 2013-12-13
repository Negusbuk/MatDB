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

#include <plytypeproperty.h>

PlyTypeProperty::PlyTypeProperty(PropertyModel *propmodel,
                                 ParameterModel* paramodel, int id) :
    Property(id)
{
    setName("Ply Type");
    setDisplayName(QObject::tr("Ply Type"));
    setCategory(LinearElasticProperty);
    setType(PlyType);
    setBehavior(UnknownBehavior);

    widget_ = 0;
    setPlyType(Regular);
}

PlyTypeProperty::PlyTypeProperty(const PlyTypeProperty& property) :
    Property(property.getId())
{
    setName("Ply Type");
    setDisplayName(QObject::tr("Ply Type"));
    setCategory(LinearElasticProperty);
    setType(PlyType);
    setBehavior(UnknownBehavior);

    widget_ = 0;
    setPlyType(Regular);
}

Property* PlyTypeProperty::clone(PropertyModel* propmodel,
                                 ParameterModel* paramodel)
{
    PlyTypeProperty* prop;
    if (propmodel && paramodel) {
        prop = new PlyTypeProperty(propmodel, paramodel, getId());
    } else {
        prop = new PlyTypeProperty(*this);
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
void PlyTypeProperty::apply(PropertyData& data,
                            PropertyDetail& /* detail */,
                            std::map<QString,ParameterDetail> paramMap)
{
    // std::cout << "PlyTypeProperty::apply" << std::endl;

    /*
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
    */
}

void PlyTypeProperty::fillSpecialWidget()
{
    if (!widget_) return;
    widget_->Property_ = this;
    widget_->updateContents();
}

PropertySpecialWidget * PlyTypeProperty::getSpecialWidget(QWidget * parent)
{
    if (!widget_) {
        widget_ = new PlyTypePropertyWidget(parent);
    }
    widget_->Property_ = this;

    return widget_;
}


void PlyTypeProperty::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Elasticity");
    stream.writeEndElement();
}

void PlyTypeProperty::writeHTML(QXmlStreamWriter& stream)
{
    /*
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
    */
}

PlyTypePropertyWidget::PlyTypePropertyWidget(QWidget * parent) :
    PropertySpecialWidget(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout(this);

    typeComboBox_ = new QComboBox(this);
    vbox->addWidget(typeComboBox_);

    typeComboBox_->addItem("Regular",
                           QVariant(PlyTypeProperty::Regular));
    typeComboBox_->addItem("Woven",
                           QVariant(PlyTypeProperty::Woven));
    typeComboBox_->addItem("Isotropic",
                           QVariant(PlyTypeProperty::Isotropic));
    typeComboBox_->addItem("Isotropic Homogeneous Core",
                           QVariant(PlyTypeProperty::IsotropicHomogeneousCore));
    typeComboBox_->addItem("Orthotropic Homogeneous Core",
                           QVariant(PlyTypeProperty::OrthotropicHomogeneousCore));
    typeComboBox_->addItem("Honeycomb Core",
                           QVariant(PlyTypeProperty::HoneycombCore));

    vbox->addStretch(1);

    setLayout(vbox);

    connect(typeComboBox_, SIGNAL(currentIndexChanged(int)),
            this, SLOT(typeChanged(int)));
}

void PlyTypePropertyWidget::typeChanged(int id)
{
    Property_->setPlyType(static_cast<PlyTypeProperty::PlyTypeId>(id));

    emit modified();
}

void PlyTypePropertyWidget::updateContents()
{
    int mode = Property_->getPlyType();
    typeComboBox_->setCurrentIndex(mode);
}


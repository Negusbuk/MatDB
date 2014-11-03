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

#include <stresslimitsproperty.h>

IsotropicStressLimitsProperty::IsotropicStressLimitsProperty(PropertyModel * /* propmodel */,
                                                             ParameterModel* paramodel, int id) :
    Property(id)
{
    setName("Isotropic Stress Limits");
    setDisplayName(QObject::tr("Isotropic Stress Limits"));
    setCategory(LinearElasticProperty);
    setType(StressLimits);
    setBehavior(Isotropic);
    Parameter *par;
    par = paramodel->getParameter("Tensile Stress");
    addParameter(par->clone());
    par = paramodel->getParameter("Compressive Stress");
    addParameter(par->clone());
    par = paramodel->getParameter("Shear Stress");
    addParameter(par->clone());
}

IsotropicStressLimitsProperty::IsotropicStressLimitsProperty(const IsotropicStressLimitsProperty& property) :
    Property(property.getId())
{
    setName("Isotropic Stress Limits");
    setDisplayName(QObject::tr("Isotropic Stress Limits"));
    setCategory(LinearElasticProperty);
    setType(StressLimits);
    setBehavior(Isotropic);
    const Parameter *par1 = property.getParameter("Tensile Stress");
    addParameter(par1->clone());
    const Parameter *par2 = property.getParameter("Compressive Stress");
    addParameter(par2->clone());
    const Parameter *par3 = property.getParameter("Shear Stress");
    addParameter(par3->clone());
}

Property* IsotropicStressLimitsProperty::clone(PropertyModel* propmodel,
                                               ParameterModel* paramodel)
{
    IsotropicStressLimitsProperty* prop;
    if (propmodel && paramodel) {
        prop = new IsotropicStressLimitsProperty(propmodel, paramodel, getId());
    } else {
        prop = new IsotropicStressLimitsProperty(*this);
    }

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
void IsotropicStressLimitsProperty::apply(PropertyData& /* data */,
                                        PropertyDetail& /* detail */,
                                        std::map<QString,ParameterDetail> /* paramMap */)
{
    // std::cout << "IsotropicStressLimitsProperty::apply" << std::endl;

//    ParameterDetail details[5];
//    Parameter *param[5];
//    std::vector<double> values[5];
//    std::vector<double> vtt;

//    std::map<QString,QString>::iterator itFind = data.qualifiers.find("Derive from");
//    if (itFind!=data.qualifiers.end()) {
//        //std::cout << itFind->second.toStdString() << std::endl;
//        if (itFind->second=="Young's Modulus and Poisson's Ratio") {
//            CalculationMode_ = CalcFromYoungsModulusAndPoissonsRatio;
//        } else {
//            //std::cout << "XXXXXXXXXXXXXX HELP ME" << std::endl;
//        }
//    } else {
//        CalculationMode_ = CalcFromYoungsModulusAndPoissonsRatio;
//    }

//    int i=0;
//    for (std::vector<PValue>::iterator it = data.pvalues.begin();
//         it!=data.pvalues.end();
//         ++it) {
//        details[i] = paramMap[it->parameter];
//        param[i] = getParameter(details[i].name);
//        //param[i]->setId(details[i].id);

//        if (it->data.contains(',')) {
//            QStringList l = it->data.split(',');
//            for (QStringList::Iterator itl = l.begin();
//                 itl!=l.end();
//                 ++itl) {
//                values[i].push_back((*itl).toDouble());
//            }
//        } else {
//            values[i].push_back(it->data.toDouble());
//        }

//        if (param[i]==0) vtt = values[i];

//        ++i;
//    }

//    for (i=0;i<5;++i) {
//        if (!param[i]) continue;

//        param[i]->setValueUnit(details[i].unit);

//        std::vector<double>::iterator itt = vtt.begin();
//        for (std::vector<double>::iterator itv = values[i].begin();
//             itv!=values[i].end() && itt!=vtt.end();
//             ++itv,++itt) {
//            double temp = *itt;
//            double value = *itv;
//            if (value!=undefindedIdentifyer()) {
//                value = param[i]->getValueUnit()->convertToPreffered(*itv);
//                if (temp==undefindedIdentifyer()) {
//                    param[i]->addValue(value);
//                } else {
//                    param[i]->addValue(temp, value);
//                }
//            }
//        }

//        param[i]->setPrefferedValueUnit();
//    }
}

void IsotropicStressLimitsProperty::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Elasticity");
    stream.writeEndElement();
}

void IsotropicStressLimitsProperty::writeHTML(QXmlStreamWriter& /* stream */)
{
//    Parameter * parE = getParameter("Young's Modulus");
//    Parameter * parNu = getParameter("Poisson's Ratio");
//    Parameter * parG = getParameter("Shear Modulus");
//    Parameter * parK = getParameter("Bulk Modulus");

//    stream.writeStartElement("tr");

//    stream.writeStartElement("td");
//    stream.writeAttribute("class", "MatDBTitle");
//    stream.writeAttribute("valign", "top");
//    stream.writeCharacters(getName());
//    stream.writeEndElement(); // td

//    stream.writeStartElement("td");
//    stream.writeAttribute("colspan", "2");
//    stream.writeAttribute("align", "right");

//    stream.writeStartElement("table");
//    //stream.writeAttribute("style", "border-top:1px solid #000;");
//    stream.writeAttribute("rules", "groups");
//    //stream.writeAttribute("class", "MatDBTable");

//    stream.writeStartElement("tbody");
//    stream.writeStartElement("tr");

//    stream.writeStartElement("td");
//    stream.writeAttribute("class", "MatDBParameterName");
//    stream.writeCharacters(parE->getName());
//    stream.writeEndElement(); // td

//    stream.writeStartElement("td");
//    stream.writeAttribute("align", "right");
//    parE->writeHTML(stream);
//    stream.writeEndElement(); // td

//    stream.writeEndElement(); // tr
//    stream.writeEndElement(); // tbody

//    stream.writeStartElement("tbody");
//    stream.writeStartElement("tr");

//    stream.writeStartElement("td");
//    stream.writeAttribute("class", "MatDBParameterName");
//    stream.writeCharacters(parNu->getName());
//    stream.writeEndElement(); // td

//    stream.writeStartElement("td");
//    stream.writeAttribute("align", "right");
//    parNu->writeHTML(stream);
//    stream.writeEndElement(); // td

//    stream.writeEndElement(); // tr
//    stream.writeEndElement(); // tbody

//    stream.writeStartElement("tbody");
//    stream.writeStartElement("tr");

//    stream.writeStartElement("td");
//    stream.writeAttribute("class", "MatDBParameterName");
//    stream.writeCharacters(parG->getName());
//    stream.writeEndElement(); // td

//    stream.writeStartElement("td");
//    stream.writeAttribute("align", "right");
//    parG->writeHTML(stream);
//    stream.writeEndElement(); // td

//    stream.writeEndElement(); // tr
//    stream.writeEndElement(); // tbody

//    stream.writeStartElement("tbody");
//    stream.writeStartElement("tr");

//    stream.writeStartElement("td");
//    stream.writeAttribute("class", "MatDBParameterName");
//    stream.writeCharacters(parK->getName());
//    stream.writeEndElement(); // td

//    stream.writeStartElement("td");
//    stream.writeAttribute("align", "right");
//    parK->writeHTML(stream);
//    stream.writeEndElement(); // td

//    stream.writeEndElement(); // tr

//    stream.writeEndElement(); // table

//    stream.writeEndElement(); // td

//    stream.writeEndElement(); // tr
}

OrthotropicStressLimitsProperty::OrthotropicStressLimitsProperty(PropertyModel* /* propmodel */,
                                                                 ParameterModel* paramodel, int id) :
    Property(id)
{
    setName("Orthotropic Stress Limits");
    setDisplayName(QObject::tr("Orthotropic Stress Limits"));
    setCategory(LinearElasticProperty);
    setType(StressLimits);
    setBehavior(Orthotropic);
    Parameter *par;
    par = paramodel->getParameter("Tensile Stress X direction");
    addParameter(par->clone());
    par = paramodel->getParameter("Tensile Stress Y direction");
    addParameter(par->clone());
    par = paramodel->getParameter("Tensile Stress Z direction");
    addParameter(par->clone());
    par = paramodel->getParameter("Compressive Stress X direction");
    addParameter(par->clone());
    par = paramodel->getParameter("Compressive Stress Y direction");
    addParameter(par->clone());
    par = paramodel->getParameter("Compressive Stress Z direction");
    addParameter(par->clone());
    par = paramodel->getParameter("Shear Stress XY");
    addParameter(par->clone());
    par = paramodel->getParameter("Shear Stress YZ");
    addParameter(par->clone());
    par = paramodel->getParameter("Shear Stress XZ");
    addParameter(par->clone());
}

OrthotropicStressLimitsProperty::OrthotropicStressLimitsProperty(const OrthotropicStressLimitsProperty& property) :
    Property(property.getId())
{
    setName("Orthotropic Stress Limits");
    setDisplayName(QObject::tr("Orthotropic Stress Limits"));
    setCategory(LinearElasticProperty);
    setType(StressLimits);
    setBehavior(Orthotropic);
    const Parameter *par1x = property.getParameter("Tensile Stress X direction");
    addParameter(par1x->clone());
    const Parameter *par1y = property.getParameter("Tensile Stress Y direction");
    addParameter(par1y->clone());
    const Parameter *par1z = property.getParameter("Tensile Stress Z direction");
    addParameter(par1z->clone());
    const Parameter *par2xy = property.getParameter("Compressive Stress X direction");
    addParameter(par2xy->clone());
    const Parameter *par2yz = property.getParameter("Compressive Stress Y direction");
    addParameter(par2yz->clone());
    const Parameter *par2xz = property.getParameter("Compressive Stress Z direction");
    addParameter(par2xz->clone());
    const Parameter *par3xy = property.getParameter("Shear Stress XY");
    addParameter(par3xy->clone());
    const Parameter *par3yz = property.getParameter("Shear Stress YZ");
    addParameter(par3yz->clone());
    const Parameter *par3xz = property.getParameter("Shear Stress XZ");
    addParameter(par3xz->clone());
}

Property* OrthotropicStressLimitsProperty::clone(PropertyModel* propmodel,
                                                 ParameterModel* paramodel)
{
    OrthotropicStressLimitsProperty* prop;
    if (propmodel && paramodel) {
        prop = new OrthotropicStressLimitsProperty(propmodel, paramodel, getId());
    } else {
        prop = new OrthotropicStressLimitsProperty(*this);
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
void OrthotropicStressLimitsProperty::apply(PropertyData& /* data */,
                                          PropertyDetail& /* detail */,
                                          std::map<QString,ParameterDetail> /* paramMap */)
{
    // std::cout << "IsotropicStressLimitsProperty::apply" << std::endl;

//    ParameterDetail details[10];
//    Parameter *param[10];
//    std::vector<double> values[10];
//    std::vector<double> vtt;

//    int i=0;
//    for (std::vector<PValue>::iterator it = data.pvalues.begin();
//         it!=data.pvalues.end();
//         ++it) {
//        details[i] = paramMap[it->parameter];
//        param[i] = getParameter(details[i].name);

//        //param[i]->setId(details[i].id);

//        if (it->data.contains(',')) {
//            QStringList l = it->data.split(',');
//            for (QStringList::Iterator itl = l.begin();
//                 itl!=l.end();
//                 ++itl) {
//                values[i].push_back((*itl).toDouble());
//            }
//        } else {
//            values[i].push_back(it->data.toDouble());
//        }

//        if (param[i]==0) vtt = values[i];

//        ++i;
//    }

//    for (i=0;i<10;++i) {
//        if (!param[i]) continue;

//        param[i]->setValueUnit(details[i].unit);

//        std::vector<double>::iterator itt = vtt.begin();
//        for (std::vector<double>::iterator itv = values[i].begin();
//             itv!=values[i].end() && itt!=vtt.end();
//             ++itv,++itt) {
//            double temp = *itt;
//            double value = *itv;
//            if (value!=undefindedIdentifyer()) {
//                value = param[i]->getValueUnit()->convertToPreffered(*itv);
//                if (temp==undefindedIdentifyer()) {
//                    param[i]->addValue(value);
//                } else {
//                    param[i]->addValue(temp, value);
//                }
//            }
//        }

//        param[i]->setPrefferedValueUnit();
//    }
}

void OrthotropicStressLimitsProperty::writeXML(QXmlStreamWriter& stream)
{
    stream.writeStartElement("PropertyDetails");
    stream.writeAttribute("id", getIdString());
    stream.writeEmptyElement("Unitless");
    stream.writeTextElement("Name", "Elasticity");
    stream.writeEndElement();
}

void OrthotropicStressLimitsProperty::writeHTML(QXmlStreamWriter& /* stream */)
{
//    Parameter * par;

//    stream.writeStartElement("tr");

//    stream.writeStartElement("td");
//    stream.writeAttribute("class", "MatDBTitle");
//    stream.writeAttribute("valign", "top");
//    stream.writeCharacters(getName());
//    stream.writeEndElement(); // td

//    stream.writeStartElement("td");
//    stream.writeAttribute("colspan", "2");
//    stream.writeAttribute("align", "right");

//    stream.writeStartElement("table");
//    //stream.writeAttribute("style", "border-top:1px solid #000;");
//    stream.writeAttribute("rules", "groups");
//    //stream.writeAttribute("class", "MatDBTable");

//    stream.writeStartElement("tbody");
//    stream.writeStartElement("tr");

//    par = getParameter("Young's Modulus X direction");
//    stream.writeStartElement("td");
//    stream.writeAttribute("class", "MatDBParameterName");
//    stream.writeCharacters("Young's Modulus X");
//    stream.writeEndElement(); // td

//    stream.writeStartElement("td");
//    stream.writeAttribute("align", "right");
//    par->writeHTML(stream);
//    stream.writeEndElement(); // td

//    stream.writeEndElement(); // tr
//    stream.writeEndElement(); // tbody

//    stream.writeStartElement("tbody");
//    stream.writeStartElement("tr");

//    par = getParameter("Young's Modulus Y direction");
//    stream.writeStartElement("td");
//    stream.writeAttribute("class", "MatDBParameterName");
//    stream.writeCharacters("Young's Modulus Y");
//    stream.writeEndElement(); // td

//    stream.writeStartElement("td");
//    stream.writeAttribute("align", "right");
//    par->writeHTML(stream);
//    stream.writeEndElement(); // td

//    stream.writeEndElement(); // tr
//    stream.writeEndElement(); // tbody

//    stream.writeStartElement("tbody");
//    stream.writeStartElement("tr");

//    par = getParameter("Young's Modulus Z direction");
//    stream.writeStartElement("td");
//    stream.writeAttribute("class", "MatDBParameterName");
//    stream.writeCharacters("Young's Modulus Z");
//    stream.writeEndElement(); // td

//    stream.writeStartElement("td");
//    stream.writeAttribute("align", "right");
//    par->writeHTML(stream);
//    stream.writeEndElement(); // td

//    stream.writeEndElement(); // tr
//    stream.writeEndElement(); // tbody

//    stream.writeStartElement("tbody");
//    stream.writeStartElement("tr");

//    par = getParameter("Poisson's Ratio XY");
//    stream.writeStartElement("td");
//    stream.writeAttribute("class", "MatDBParameterName");
//    stream.writeCharacters(par->getName());
//    stream.writeEndElement(); // td

//    stream.writeStartElement("td");
//    stream.writeAttribute("align", "right");
//    par->writeHTML(stream);
//    stream.writeEndElement(); // td

//    stream.writeEndElement(); // tr
//    stream.writeEndElement(); // tbody

//    stream.writeStartElement("tbody");
//    stream.writeStartElement("tr");

//    par = getParameter("Poisson's Ratio YZ");
//    stream.writeStartElement("td");
//    stream.writeAttribute("class", "MatDBParameterName");
//    stream.writeCharacters(par->getName());
//    stream.writeEndElement(); // td

//    stream.writeStartElement("td");
//    stream.writeAttribute("align", "right");
//    par->writeHTML(stream);
//    stream.writeEndElement(); // td

//    stream.writeEndElement(); // tr
//    stream.writeEndElement(); // tbody

//    stream.writeStartElement("tbody");
//    stream.writeStartElement("tr");

//    par = getParameter("Poisson's Ratio XZ");
//    stream.writeStartElement("td");
//    stream.writeAttribute("class", "MatDBParameterName");
//    stream.writeCharacters(par->getName());
//    stream.writeEndElement(); // td

//    stream.writeStartElement("td");
//    stream.writeAttribute("align", "right");
//    par->writeHTML(stream);
//    stream.writeEndElement(); // td

//    stream.writeEndElement(); // tr
//    stream.writeEndElement(); // tbody

//    stream.writeStartElement("tbody");
//    stream.writeStartElement("tr");

//    par = getParameter("Shear Modulus XY");
//    stream.writeStartElement("td");
//    stream.writeAttribute("class", "MatDBParameterName");
//    stream.writeCharacters(par->getName());
//    stream.writeEndElement(); // td

//    stream.writeStartElement("td");
//    stream.writeAttribute("align", "right");
//    par->writeHTML(stream);
//    stream.writeEndElement(); // td

//    stream.writeEndElement(); // tr
//    stream.writeEndElement(); // tbody

//    stream.writeStartElement("tbody");
//    stream.writeStartElement("tr");

//    par = getParameter("Shear Modulus YZ");
//    stream.writeStartElement("td");
//    stream.writeAttribute("class", "MatDBParameterName");
//    stream.writeCharacters(par->getName());
//    stream.writeEndElement(); // td

//    stream.writeStartElement("td");
//    stream.writeAttribute("align", "right");
//    par->writeHTML(stream);
//    stream.writeEndElement(); // td

//    stream.writeEndElement(); // tr
//    stream.writeEndElement(); // tbody

//    stream.writeStartElement("tbody");
//    stream.writeStartElement("tr");

//    par = getParameter("Shear Modulus XZ");
//    stream.writeStartElement("td");
//    stream.writeAttribute("class", "MatDBParameterName");
//    stream.writeCharacters(par->getName());
//    stream.writeEndElement(); // td

//    stream.writeStartElement("td");
//    stream.writeAttribute("align", "right");
//    par->writeHTML(stream);
//    stream.writeEndElement(); // td

//    stream.writeEndElement(); // tr
//    stream.writeEndElement(); // tbody

//    stream.writeEndElement(); // table

//    stream.writeEndElement(); // td

//    stream.writeEndElement(); // tr
}

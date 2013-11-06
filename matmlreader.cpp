#include <nqlogger.h>

#include "matmlreader.h"

MATMLReader::MATMLReader(MaterialListModel *model,
                         PropertyModel *propmodel,
                         ParameterModel *paramodel,
                         MaterialCategoryModel* categoryModel,
                         QObject *parent) :
    QObject(parent),
    model_(model),
    propmodel_(propmodel),
    paramodel_(paramodel),
    categoryModel_(categoryModel)
{

}

void MATMLReader::findMatMLDocElement(QDomElement& parent, QDomElement& element, bool& found)
{
    if (parent.nodeName()=="MatML_Doc") {
        element = parent;
        found = true;
        return;
    }

    QDomNodeList list = parent.childNodes();
    for (int n=0;n<list.count();++n) {
        QDomElement e = list.at(n).toElement();
        if(!e.isNull()) {
            if (e.nodeName()=="MatML_Doc") {
                element = e;
                found = true;
                return;
            } else {
                findMatMLDocElement(e, element, found);
            }
        }
    }
}

QString MATMLReader::processUnits(QDomElement units)
{
    QString unit("");
    QDomNodeList children = units.childNodes();
    for (int u=0;u<children.count();++u) {
        if (u!=0) unit += " ";
        QDomElement e = children.at(u).toElement();
        int power = e.attribute("power", "1").toInt();
        if (power==1) {
            unit += e.text();
        } else {
            unit += e.text();
            unit += "^";
            unit += QString().setNum(power);
        }
    }
    return unit;
}

void MATMLReader::processPropertyDetail(QDomElement& element)
{
    if (!element.hasAttribute("id")) return;
    PropertyDetail detail;
    detail.id = element.attribute("id");
    detail.name = element.firstChildElement("Name").text();
    detail.unit = processUnits(element.firstChildElement("Units"));
    PropertyDetailMap_[detail.id] = detail;
}

void MATMLReader::processParameterDetail(QDomElement& element)
{
    if (!element.hasAttribute("id")) return;
    ParameterDetail detail;
    detail.id = element.attribute("id");
    detail.name = element.firstChildElement("Name").text();
    detail.unit = processUnits(element.firstChildElement("Units"));
    ParameterDetailMap_[detail.id] = detail;
}

PValue MATMLReader::processParameterValue(QDomElement& element)
{
    QDomElement e;
    PValue pv;
    pv.parameter = element.attribute("parameter");
    pv.format = element.attribute("format");
    e = element.firstChildElement("Data");
    pv.data = e.text();
    e = element.firstChildElement("Qualifier");
    pv.qualifiertype = e.attribute("name");
    pv.qualifier = e.text();
    return pv;
}

PropertyData MATMLReader::processPropertyData(QDomElement& element)
{
    QDomElement e;
    PropertyData data;
    if (!element.hasAttribute("property")) return data;
    data.property = element.attribute("property");
    e = element.firstChildElement("Data");
    data.dataformat = e.attribute("format", "float");
    data.data = e.text();

    QDomNodeList qualifierList = element.childNodes();
    for (int i=0;i<qualifierList.count();++i) {
        e = qualifierList.at(i).toElement();
        if (e.nodeName()=="Qualifier") {
            data.qualifiers[e.attribute("name")] = e.text();
        } else if (e.nodeName()=="ParameterValue") {
            PValue pv = processParameterValue(e);
            data.pvalues.push_back(pv);
        }
    }

    return data;
}

void MATMLReader::processMaterial(QDomElement& matElem, Material* mat)
{
    NQLog("MATMLReader", NQLog::Message) << "  process material " << mat->getName();

    QDomNodeList propertyList = matElem.elementsByTagName("PropertyData");
    for (int i=0;i<propertyList.count();++i) {
        QDomElement e = propertyList.at(i).toElement();
        //if (!e.hasAttribute("property")) continue;

        PropertyData data = processPropertyData(e);
        PropertyDetail propDetail = PropertyDetailMap_[data.property];

        QString propName = "";

        std::map<QString,QString>::iterator itFindB = data.qualifiers.find("Behavior");
        if (itFindB!=data.qualifiers.end()) {
            propName += itFindB->second;
            propName += " ";
        }
        std::map<QString,QString>::iterator itFindD = data.qualifiers.find("Definition");
        if (itFindD!=data.qualifiers.end()) {
            propName += itFindD->second;
            propName += " ";
        }
        propName += propDetail.name;

        NQLog("MATMLReader", NQLog::Message) << "    property " << propName;

        Property * prop = propmodel_->getProperty(propName);
        if (!prop) continue;
        Property * clonedProp = prop->clone(paramodel_);
        clonedProp->apply(data, propDetail, ParameterDetailMap_);
        mat->addProperty(clonedProp);
    }
}

void MATMLReader::read(QIODevice *source)
{
    if (!document_.setContent(source)) return;

    NQLog("MATMLReader", NQLog::Message) << "void read(QIODevice *destination)";

    QDomElement docElem = document_.documentElement();
    QDomElement matmlElem;
    bool found = false;
    findMatMLDocElement(docElem, matmlElem, found);
    if (!found) return;

    QDomNodeList materialsElemList = matmlElem.elementsByTagName("Material");
    QDomNodeList metadataElemList = matmlElem.elementsByTagName("Metadata");
    if (metadataElemList.count()!=1) return;

    PropertyDetailMap_.clear();
    ParameterDetailMap_.clear();

    QDomNodeList propertydetailsList = metadataElemList.at(0).toElement().elementsByTagName("PropertyDetails");
    for (int i=0;i<propertydetailsList.count();++i) {
        QDomElement e = propertydetailsList.at(i).toElement();
        processPropertyDetail(e);
    }

    QDomNodeList parameterdetailsList = metadataElemList.at(0).toElement().elementsByTagName("ParameterDetails");
    for (int i=0;i<parameterdetailsList.count();++i) {
        QDomElement e = parameterdetailsList.at(i).toElement();
        processParameterDetail(e);
    }

    for (int i=0;i<materialsElemList.count();++i) {
        QDomElement e = materialsElemList.at(i).toElement();
        QDomElement bulk = e.elementsByTagName("BulkDetails").at(0).toElement();
        QDomElement name = bulk.elementsByTagName("Name").at(0).toElement();

        Material * mat = new Material();
        mat->setName(name.text());

        if (!bulk.elementsByTagName("Category").isEmpty()) {
            QDomElement category = bulk.elementsByTagName("Category").at(0).toElement();
            mat->setCategory(categoryModel_->getCategory(category.text()));
        }

        QDomNodeList tagList = bulk.elementsByTagName("Tag");
        QStringList tags;
        if (!tagList.isEmpty()) {
            for (int i=0;i<tagList.size();++i) {
                QDomElement tagElement = tagList.at(i).toElement();
                tags << tagElement.text();
            }
        }
        mat->setTags(tags);

        if (!bulk.elementsByTagName("Description").isEmpty()) {
            QDomElement description = bulk.elementsByTagName("Description").at(0).toElement();
            mat->setDescription(description.text());
        }

        QString notes;
        QDomNodeList notesList = bulk.elementsByTagName("Notes");
        if (!notesList.isEmpty()) {
            for (int i=0;i<notesList.size();++i) {
                QDomElement notesElement = notesList.at(i).toElement();
                notes += notesElement.text();
                notes += "\n";
            }
        }
        mat->setNotes(notes);

        processMaterial(bulk, mat);

        model_->addMaterial(mat);
    }

    model_->sort();
}


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

#include <algorithm>

#include <QSettings>
#include <QXmlStreamWriter>
#include <QDomDocument>

#include <nqlogger.h>

#include <materiallistmodel.h>

MaterialListModel::MaterialListModel(MaterialCategoryModel* model,
                                     QObject *parent) :
    QObject(parent),
    CategoryModel_(model),
    isFiltered_(false),
    modified_(false)
{
    currentFilterLogic_ = true;
    currentFilters_ = QStringList();

    MaterialIndexer_ = new MaterialIndexer(this);

    connect(CategoryModel_, SIGNAL(categoriesChanged()),
            this, SLOT(categoriesChanged()));
    connect(CategoryModel_, SIGNAL(categoryChanged(MaterialCategory*)),
            this, SLOT(categoryChanged(MaterialCategory*)));
}

size_t MaterialListModel::getMaterialCount() const
{
    if (isFiltered_) return FilteredMaterialList_.size();
    return MaterialList_.size();
}

void MaterialListModel::addMaterial(Material* material)
{
    QString basename = material->getName();
    int index = 2;
    QString newname = basename;
    while (findMaterial(newname)!=0) {
        newname = basename + " " + QString::number(index);
        index++;
    }
    material->setName(newname);
    MaterialList_.push_back(material);

    sort();

    modified_ = true;

    emit materialListChanged(MaterialList_.size());

    if (currentFilters_.size()>0) {
        MaterialIndexer_->filter(currentFilters_, currentFilterLogic_, FilteredMaterialList_);
        sortFiltered();
        isFiltered_ = (currentFilters_.size()!=0);
    }

    emit materialCountChanged(getMaterialCount());
}

void MaterialListModel::addMaterials(const std::vector<Material*>& materials)
{
    // NQLog("MaterialListModel") << "adding " << materials.size() << " materials";

    for (std::vector<Material*>::const_iterator it = materials.begin();
         it!=materials.end();
         ++it) {
        Material * material = *it;
        QString basename = material->getName();
        int index = 2;
        QString newname = basename;
        while (findMaterial(newname)!=0) {
            newname = basename + " " + QString::number(index);
            index++;
        }
        material->setName(newname);
        MaterialList_.push_back(material);
    }

    sort();

    modified_ = true;

    emit materialListChanged(MaterialList_.size());

    if (currentFilters_.size()>0) {
        MaterialIndexer_->filter(currentFilters_, currentFilterLogic_, FilteredMaterialList_);
        sortFiltered();
        isFiltered_ = (currentFilters_.size()!=0);
    }

    emit materialCountChanged(getMaterialCount());
}

const std::vector<Material*>& MaterialListModel::getMaterials() const
{
    if (isFiltered_) return FilteredMaterialList_;
    return MaterialList_;
}

Material* MaterialListModel::getMaterial(size_t idx)
{
    // NQLog("MaterialListModel", NQLog::Spam) << "void getMaterial(size_t idx) " << idx;

    if (isFiltered_) {
        if (idx>=FilteredMaterialList_.size()) return 0;
        return FilteredMaterialList_[idx];
    } else {
        if (idx>=MaterialList_.size()) return 0;
        return MaterialList_[idx];
    }
}

Material* MaterialListModel::getUnfilteredMaterial(size_t idx)
{
    // NQLog("MaterialListModel", NQLog::Spam) << "void getMaterial(size_t idx) " << idx;

    if (idx>=MaterialList_.size()) return 0;
    return MaterialList_[idx];
}

void MaterialListModel::read(const QDir& dbDir,
                             PropertyModel *propmodel)
{
    NQLog("MaterialListModel", NQLog::Message) << "void read(const QDir& dbDir,...)";
    NQLog("MaterialListModel", NQLog::Message) << dbDir.absolutePath();

    QFile ifile(dbDir.absoluteFilePath("MaterialList.xml"));
    if (ifile.open(QIODevice::ReadOnly)) {

        QDomDocument document;
        if (!document.setContent(&ifile)) {
            ifile.close();
            return;
        }

        QDomElement docElem = document.documentElement();
        QDomNodeList matElemList = docElem.elementsByTagName("Material");

        if (matElemList.count()==0) {
            ifile.close();
            return;
        }

        for (int i=0;i<matElemList.count();++i) {
            QDomElement matElem = matElemList.at(i).toElement();

            QDomElement uuidElem = matElem.elementsByTagName("UUID").at(0).toElement();
            QDomElement nameElem = matElem.elementsByTagName("Name").at(0).toElement();

            NQLog("MaterialListModel", NQLog::Spam) << nameElem.text();

            Material * mat = new Material();
            mat->setName(nameElem.text());
            QString uuid = uuidElem.text();
            mat->setUUID(uuid);

            uuid.remove(0, 1);
            uuid.remove(uuid.length()-1, 1);
            uuid += "_mat.xml";
            QFile ifile2(dbDir.absoluteFilePath(uuid));
            if (ifile2.open(QIODevice::ReadOnly)) {
                if (mat->read(&ifile2,
                              propmodel,
                              CategoryModel_)) {
                    this->addMaterial(mat);
                } else {
                    delete mat;
                }
                ifile2.close();
            }
        }
        ifile.close();
    }
}

void MaterialListModel::write(const QDir& dbDir)
{
    if (modified_) {
        NQLog("MaterialListModel", NQLog::Message) << "void write(const QDir& dbDir)";
        NQLog("MaterialListModel", NQLog::Message) << dbDir.absolutePath();

        QFile ofile(dbDir.absoluteFilePath("MaterialList.xml"));
        if (ofile.open(QIODevice::WriteOnly)) {

            QXmlStreamWriter stream(&ofile);

            stream.setAutoFormatting(true);
            stream.setAutoFormattingIndent(2);

            stream.writeStartDocument();
            stream.writeStartElement("Materials");

            for (std::vector<Material*>::iterator it = MaterialList_.begin();
                 it!=MaterialList_.end();
                 ++it) {
                Material * mat = *it;

                stream.writeStartElement("Material");
                stream.writeTextElement("Name", mat->getName());
                stream.writeTextElement("UUID", mat->getUUID());
                stream.writeEndElement();
            }

            stream.writeEndElement();
            stream.writeEndDocument();

            ofile.close();
        }
    }

    for (std::vector<Material*>::iterator it = MaterialList_.begin();
         it!=MaterialList_.end();
         ++it) {
        Material * mat = *it;
        if (!mat->isModified()) continue;

        QString uuid = mat->getUUID();
        uuid.remove(0, 1);
        uuid.remove(uuid.length()-1, 1);
        uuid += "_mat.xml";
        QFile ofile2(dbDir.absoluteFilePath(uuid));
        if (ofile2.open(QIODevice::WriteOnly)) {
            mat->write(&ofile2);
            ofile2.close();
        }
    }

    modified_ = false;
}

void MaterialListModel::sort()
{
    std::sort(MaterialList_.begin(), MaterialList_.end(),
              [](Material*lhs, Material*rhs) {
        int result = QString::localeAwareCompare(lhs->getName(), rhs->getName());
        return (result<0);
    });
}

void MaterialListModel::sortFiltered()
{
    std::sort(FilteredMaterialList_.begin(), FilteredMaterialList_.end(),
              [](Material*lhs, Material*rhs) {
        int result = QString::localeAwareCompare(lhs->getName(), rhs->getName());
        return (result<0);
    });
}

Material* MaterialListModel::findMaterial(const QString& name)
{
    for (std::vector<Material*>::iterator it = MaterialList_.begin();
         it!=MaterialList_.end();
         ++it) {
        if ((*it)->getName()==name) return *it;
    }
    return 0;
}

void MaterialListModel::removeMaterial(Material* material)
{
    std::vector<Material*>::iterator it = std::find(MaterialList_.begin(),
                                                    MaterialList_.end(),
                                                    material);
    if (it==MaterialList_.end()) return;
    MaterialList_.erase(it);
}

void MaterialListModel::materialMetadataChanged(Material* material)
{
    emit metadataChanged(material);
}

void MaterialListModel::duplicateMaterial(Material* material)
{
    if (material==0) return;

    NQLog("MaterialListModel") << "void MaterialSelectionModel::duplicateMaterial() "
                               << material->getName();

    Material* mat = material->clone();
    addMaterial(mat);
}

void MaterialListModel::deleteMaterial(Material* material)
{
    if (material==0) return;

    NQLog("MaterialListModel") << "void MaterialSelectionModel::deleteMaterial() "
                               << material->getName();

    for (std::vector<Material*>::iterator it = MaterialList_.begin();
         it!=MaterialList_.end();
         ++it) {
        Material * mat = *it;
        if (mat==material) {
            MaterialList_.erase(it);

            QSettings settings;
            QString dbPath = settings.value("dbpath").toString();
            QDir dbDir(dbPath);
            if (!dbDir.exists()) {
                dbDir.mkpath(".");
            }
            QString uuid = mat->getUUID();
            uuid.remove(0, 1);
            uuid.remove(uuid.length()-1, 1);
            uuid += "_mat.xml";
            dbDir.remove(uuid);

            delete mat;

            modified_ = true;

            break;
        }
    }

    emit materialListChanged(MaterialList_.size());

    if (currentFilters_.size()>0) {
        MaterialIndexer_->filter(currentFilters_, currentFilterLogic_, FilteredMaterialList_);
        sortFiltered();
        isFiltered_ = (currentFilters_.size()!=0);
    }

    emit materialCountChanged(getMaterialCount());
}

void MaterialListModel::categoriesChanged()
{
    for (std::vector<Material*>::iterator it = MaterialList_.begin();
         it!=MaterialList_.end();
         ++it) {
        Material * mat = *it;
        if (mat->getCategory()==0) continue;

        if (!CategoryModel_->isCategoryValid(mat->getCategory())) {
            mat->setCategory(0);
            emit metadataChanged(mat);
        }
    }
}

void MaterialListModel::categoryChanged(MaterialCategory* category)
{
    for (std::vector<Material*>::iterator it = MaterialList_.begin();
         it!=MaterialList_.end();
         ++it) {
        Material * mat = *it;
        if (mat->getCategory()==0) continue;

        if (mat->getCategory()==category) emit metadataChanged(mat);
    }
}

void MaterialListModel::filterChanged(const QStringList& filters, bool logic)
{
    // NQLog("MaterialListModel", NQLog::Spam) << "void filterChanged(const QStringList& filters) "
    //                                         << filters.size() << " " << currentFilters_.size();

    if (currentFilters_!=filters || currentFilterLogic_!=logic) {
        MaterialIndexer_->filter(filters, logic, FilteredMaterialList_);
        sortFiltered();
        isFiltered_ = (filters.size()!=0);
        emit materialCountChanged(getMaterialCount());
    }

    currentFilterLogic_ = logic;
    currentFilters_ = filters;
    isFiltered_ = (filters.size()!=0);
}

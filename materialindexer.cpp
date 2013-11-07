#include <QRegExp>

#include <nqlogger.h>

#include <materiallistmodel.h>

#include "materialindexer.h"

MaterialIndexer::MaterialIndexer(MaterialListModel *listmodel,
                                 QObject *parent) :
    QObject(parent),
    ListModel_(listmodel)
{
    connect(ListModel_, SIGNAL(materialCountChanged(int)),
            this, SLOT(materialCountChanged(int)));
    connect(ListModel_, SIGNAL(metadataChanged(Material*)),
            this, SLOT(materialMetadataChanged(Material*)));

    keyMap_.clear();
    materialMap_.clear();
}

void MaterialIndexer::filter(const QString& key, std::vector<Material*>& materials)
{
    std::unordered_set<Material*> temp;

    QRegExp rx(key);
    rx.setPatternSyntax(QRegExp::Wildcard);

    for (auto it = keyMap_.begin();
         it!=keyMap_.end();
         ++it) {

        if (rx.exactMatch(it->first.c_str())) {
            for (auto itM = it->second.begin();
                 itM!=it->second.end();
                 ++itM) {
                temp.emplace(*itM);
            }
        }
    }

    materials.clear();
    for (auto it = temp.begin();
         it!=temp.end();
         ++it) {
        materials.push_back(*it);
    }
}

void MaterialIndexer::materialCountChanged(int count)
{
    NQLog("MaterialIndexer", NQLog::Spam) << "void materialCountChanged(int count) " << count;

    std::string key;
    Material * material;
    for (int i=0;i<count;++i) {
        material = ListModel_->getMaterial(i);

        auto empMaterial = materialMap_.emplace(material, std::unordered_set<std::string>());
        auto itMaterial = empMaterial.first;
        std::unordered_set<std::string>& materialKeys = itMaterial->second;

        key = material->getName().toStdString();
        materialKeys.emplace(key);
        processKey(key, material);

        MaterialCategory* category = material->getCategory();
        if (category) {
            key = category->getName().toStdString();
            materialKeys.emplace(key);
            processKey(key, material);
        }
    }

    dumpIndex();
}

void MaterialIndexer::materialMetadataChanged(Material* material)
{
    NQLog("MaterialIndexer", NQLog::Spam) << "void materialMetadataChanged(Material* material) "
                                          << material->getName();

    auto findMaterial = materialMap_.find(material);
    if (findMaterial!=materialMap_.end()) {
        auto& keys = findMaterial->second;
        for (auto itKey=keys.begin();
             itKey!=keys.end();
             ++itKey) {
            auto findKey = keyMap_.find(*itKey);
            if (findKey!=keyMap_.end()) {
                auto& materials = findKey->second;
                auto findMaterialSet = materials.find(material);
                if (findMaterialSet!=materials.end()) {
                    materials.erase(findMaterialSet);
                }
                if (materials.size()==0) {
                    keyMap_.erase(findKey);
                }
            }
        }
        materialMap_.erase(findMaterial);
    }

    std::string key;
    auto empMaterial = materialMap_.emplace(material, std::unordered_set<std::string>());
    auto itMaterial = empMaterial.first;
    std::unordered_set<std::string>& materialKeys = itMaterial->second;

    key = material->getName().toStdString();
    materialKeys.emplace(key);
    processKey(key, material);

    MaterialCategory* category = material->getCategory();
    if (category) {
        key = category->getName().toStdString();
        materialKeys.emplace(key);
        processKey(key, material);
    }

    dumpIndex();
}

void MaterialIndexer::processKey(const std::string& key, Material* material)
{
    auto empKey = keyMap_.emplace(key, std::unordered_set<Material*>());
    auto itKey = empKey.first;
    auto& keyMaterial = itKey->second;
    keyMaterial.emplace(material);
}

void MaterialIndexer::dumpIndex()
{
    NQLog("MaterialIndexer", NQLog::Spam) << "void dump()";

    for (auto it = keyMap_.begin();
         it != keyMap_.end();
         ++it) {
        NQLog("MaterialIndexer", NQLog::Spam) << "  "
                                              << it->first;

        auto& materials = it->second;
        for (auto itMat=materials.begin();
             itMat!=materials.end();
             ++itMat) {
            NQLog("MaterialIndexer", NQLog::Spam) << "     -> "
                                                  << (*itMat)->getName();
        }
    }

    for (auto it = materialMap_.begin();
         it != materialMap_.end();
         ++it) {
        NQLog("MaterialIndexer", NQLog::Spam) << "  "
                                              << it->first->getName();

        auto& keys = it->second;
        for (auto itKey=keys.begin();
             itKey!=keys.end();
             ++itKey) {
            NQLog("MaterialIndexer", NQLog::Spam) << "     -> "
                                                  << *itKey;
        }
    }
}

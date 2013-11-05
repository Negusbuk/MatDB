#include "materialtagsedit.h"

MaterialTagsEdit::MaterialTagsEdit(MaterialListModel* listModel,
                                   MaterialSelectionModel* selectionModel,
                                   QWidget *parent) :
    NQTokenEdit(parent),
    listModel_(listModel),
    selectionModel_(selectionModel)
{
    connect(selectionModel_, SIGNAL(selectionChanged(Material*)),
            this, SLOT(materialChanged(Material*)));

    connect(this, SIGNAL(tokensChanged(QStringList)),
            this, SLOT(tagsChanged(QStringList)));

    connect(this, SIGNAL(materialMetadataChanged(Material*)),
            listModel_, SLOT(materialMetadataChanged(Material*)));
}

void MaterialTagsEdit::materialChanged(Material* material)
{
    setTokens(material->getTags());
}

void MaterialTagsEdit::tagsChanged(const QStringList& tags)
{
    Material * material = selectionModel_->getSelection();
    if (material) {
        material->setTags(tags);
        emit materialMetadataChanged(material);
    }
}

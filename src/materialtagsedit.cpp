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
    if (!material) {
        setTokens(QStringList());
    } else {
        setTokens(material->getTags());
    }
}

void MaterialTagsEdit::tagsChanged(const QStringList& tags)
{
    Material * material = selectionModel_->getSelection();
    if (material) {
        material->setTags(tags);
        emit materialMetadataChanged(material);
    }
}

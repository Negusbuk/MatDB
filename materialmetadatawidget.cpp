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

#include <QHeaderView>

#include <nqlogger.h>

#include <materialcategorybox.h>
#include <materialtagsedit.h>
#include <materialmetadatawidget.h>

MaterialMetadataWidget::MaterialMetadataWidget(MaterialListModel* listModel,
                                               MaterialSelectionModel* selectionModel,
                                               MaterialCategoryModel* categoryModel,
                                               QWidget *parent) :
    QScrollArea(parent),
    listModel_(listModel),
    selectionModel_(selectionModel),
    categoryModel_(categoryModel)
{
    setWidgetResizable(true);

    content_ = new QWidget(this);
    layout_ = new QFormLayout(content_);

    layout_->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    this->setWidget(content_);

    descriptionEdit_ = new QLineEdit(content_);
    descriptionEdit_->setAttribute(Qt::WA_MacShowFocusRect, 0);
    layout_->addRow("Description:", descriptionEdit_);

    MaterialCategoryBox * categoryBox;
    categoryBox = new MaterialCategoryBox(listModel_, selectionModel_,
                                          categoryModel_, content_);
    layout_->addRow("Category:", categoryBox);

    MaterialTagsEdit * tagsEdit;
    tagsEdit = new MaterialTagsEdit(listModel_, selectionModel_,
                                    content_);
    tagsEdit->setFixedHeight(80);
    layout_->addRow("Tags:", tagsEdit);

    notesEdit_ = new QPlainTextEdit(content_);
    notesEdit_->setFixedHeight(60);
    layout_->addRow("Notes:", notesEdit_);

    connect(selectionModel_, SIGNAL(selectionChanged(Material*)),
            this, SLOT(materialChanged(Material*)));

    updateGeometry();
}

void MaterialMetadataWidget::materialChanged(Material* material)
{
    NQLog("MaterialMetadataWidget", NQLog::Spam) << "void materialChanged()";

    if (material) {
        descriptionEdit_->setText(material->getDescription());
        notesEdit_->setPlainText(material->getNotes());
    } else {
        descriptionEdit_->setText("");
        notesEdit_->setPlainText("");
    }
}

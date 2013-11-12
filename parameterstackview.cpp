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

#include <nqlogger.h>

#include <parameterstackview.h>

ParameterStackView::ParameterStackView(MaterialListModel *listmodel,
                                       MaterialSelectionModel * selectionmodel,
                                       PropertySelectionModel* propertyselectionmodel,
                                       ParameterSelectionModel* parameterselectionmodel,
                                       PropertyModel* propertymodel,
                                       MaterialCategoryModel* categoryModel,
                                       QWidget *parent) :
    QStackedWidget(parent),
    MaterialListModel_(listmodel),
    MaterialSelectionModel_(selectionmodel),
    PropertySelectionModel_(propertyselectionmodel),
    ParameterSelectionModel_(parameterselectionmodel),
    PropertyModel_(propertymodel),
    MaterialCategoryModel_(categoryModel)
{
    Empty_ = new QWidget(this);
    addWidget(Empty_);

    MetadataWidget_ = new MaterialMetadataWidget(MaterialListModel_,
                                                 MaterialSelectionModel_,
                                                 MaterialCategoryModel_,
                                                 this);
    addWidget(MetadataWidget_);

    MaterialParameterView_ = new MaterialParameterView(MaterialListModel_,
                                                       MaterialSelectionModel_,
                                                       PropertySelectionModel_,
                                                       ParameterSelectionModel_,
                                                       this);
    addWidget(MaterialParameterView_);

    for (std::map<QString,Property*>::const_iterator it = PropertyModel_->getProperties().begin();
         it!=PropertyModel_->getProperties().end();
         ++it) {
        QString s = it->first;
        Property * property = it->second;
        if (property->hasSpecialWidget()) {
            PropertySpecialWidget * w = property->getSpecialWidget(this);
            addWidget(w);
            SpecialWidgetMap_[s] = w;

            connect(w, SIGNAL(modified()),
                    PropertySelectionModel_, SLOT(emitPropertyModified()));
        }
    }

    connect(MaterialSelectionModel_, SIGNAL(selectionChanged(Material*)),
            this, SLOT(materialSelectionChanged(Material*)));
    connect(PropertySelectionModel_, SIGNAL(selectionChanged(Property*)),
            this, SLOT(propertySelectionChanged(Property*)));
    connect(ParameterSelectionModel_, SIGNAL(selectionChanged(Parameter*)),
            this, SLOT(parameterSelectionChanged(Parameter*)));
}

void ParameterStackView::materialSelectionChanged(Material* material)
{
    NQLog("ParameterStackView", NQLog::Spam) << "void materialSelectionChanged(Materal* material) "
                                             << material;

    setCurrentWidget(MetadataWidget_);
}

void ParameterStackView::propertySelectionChanged(Property* property)
{
    NQLog("ParameterStackView", NQLog::Spam) << "void propertySelectionChanged(Property* property) "
                                             << property;

    if (!property) return;
    if (!property->hasSpecialWidget()) {
        setCurrentWidget(Empty_);
        return;
    }

    QString s = property->getName();
    NQLog("ParameterStackView", NQLog::Spam) << "property " << s;

    std::map<QString,PropertySpecialWidget*>::iterator it = SpecialWidgetMap_.find(s);
    if (it==SpecialWidgetMap_.end()) {
        setCurrentWidget(Empty_);
        return;
    } else {
        PropertySpecialWidget * w = it->second;
        setCurrentWidget(w);
        property->fillSpecialWidget();
    }
}

void ParameterStackView::parameterSelectionChanged(Parameter* parameter)
{
    NQLog("ParameterStackView", NQLog::Spam) << "void parameterSelectionChanged(Parameter* parameter) "
                                             << parameter;

    if (parameter) {
        setCurrentWidget(MaterialParameterView_);
    } else {
        //setCurrentWidget(Empty_);
    }
}

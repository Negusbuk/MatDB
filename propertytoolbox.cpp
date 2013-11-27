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

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDrag>
#include <QMimeData>

#include "propertytoolbox.h"

PropertyToolBoxItem::PropertyToolBoxItem(const QString &name,
                                         Property *property,
                                         QWidget *parent) :
    QLabel(name, parent),
    Property_(property)
{
    setFrameShadow(Plain);
}

void PropertyToolBoxItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QDrag *drag = new QDrag(this);

        QMimeData *mimeData = new QMimeData;
        mimeData->setText(Property_->getName());
        drag->setMimeData(mimeData);

        drag->exec(Qt::CopyAction, Qt::CopyAction);
    }
}

PropertyToolBox::PropertyToolBox(PropertyModel *model,
                                 QWidget *parent) :
    QToolBox(parent),
    PropertyModel_(model)
{
    build();

    setMinimumWidth(350);
    setMaximumWidth(500);
    setMinimumHeight(300);
}

void PropertyToolBox::build()
{
    QWidget * box;
    PropertyToolBoxItem * item;
    std::map<QString,QWidget*> categoryWidgetMap;
    const std::vector<QString>& categories = PropertyModel_->getCategories();
    for (std::vector<QString>::const_iterator it = categories.begin();
         it!=categories.end();
         ++it) {
        QString s = *it;
    }

    const std::map<QString,std::vector<Property*> >& m = PropertyModel_->getPropertiesByCategory();
    for (std::map<QString,std::vector<Property*> >::const_iterator itm = m.begin();
         itm!=m.end();
         ++itm) {
        const std::vector<Property*>& v = itm->second;
        QString s = itm->first;

        box = new QWidget(this);
        categoryWidgetMap[s] = box;
        QVBoxLayout * layout = new QVBoxLayout(box);
        box->setLayout(layout);

        for (std::vector<Property*>::const_iterator itv = v.begin();
             itv!=v.end();
             ++itv) {
            item = new PropertyToolBoxItem((*itv)->getDisplayName(), (*itv), box);
            layout->addWidget(item);
        }
        layout->addStretch(1);
    }

    for (std::vector<QString>::const_iterator it = categories.begin();
         it!=categories.end();
         ++it) {
        QString s = *it;

        box = categoryWidgetMap[s];
        if (!box) continue;
        addItem(box, s);
    }
}

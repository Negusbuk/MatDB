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

#include <QPixmap>
#include <QPainter>

#include <materialcategory.h>

QIcon* MaterialCategory::emptyIcon_ = 0;

MaterialCategory::MaterialCategory(const QString& name,
                                   const QColor& color,
                                   bool readonly):
    isReadOnly_(readonly),
    name_(name),
    color_(color)
{
    if (emptyIcon_==0) {
        QPixmap e(8, 8);
        e.fill(QColor(242, 142, 0, 0));
        emptyIcon_ = new QIcon(e);
    }

    QPixmap pm(8, 8);
    pm.fill(color_);
    icon_ = QIcon(pm);
}

void MaterialCategory::setColor(const QColor& color)
{
    color_ = color;
    QPixmap pm(8, 8);
    pm.fill(color_);
    icon_ = QIcon(pm);
}

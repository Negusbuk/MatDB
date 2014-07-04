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

#ifndef MATERIALCATEGORY_H
#define MATERIALCATEGORY_H

#include <QString>
#include <QColor>
#include <QIcon>

class MaterialCategory
{
public:
    explicit MaterialCategory(const QString& name,
                              const QColor& bgColor,
                              bool readonly);

    const QString& getUUID() const { return uuid_; }
    bool isReadOnly() const { return isReadOnly_; }
    const QString& getName() const { return name_; }
    const QString getDisplayName() const;
    const QColor& getColor() const { return color_; }
    const QIcon& getIcon() const { return icon_; }
    static QIcon& getEmptyIcon() { return *emptyIcon_; }

public slots:

    void setUUID(const QString& uuid) { uuid_ = uuid; }
    void setName(const QString& name) { name_ = name; }
    void setColor(const QColor& color);

protected:

    QString uuid_;
    bool isReadOnly_;
    QString name_;
    QColor color_;
    QIcon icon_;
    static QIcon* emptyIcon_;
};

#endif // MATERIALCATEGORY_H

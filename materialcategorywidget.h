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

#ifndef MATERIALCATEGORYWIDGET_H
#define MATERIALCATEGORYWIDGET_H

#include <QWidget>
#include <QListView>
#include <QPushButton>
#include <QEvent>

#include <materialcategorymodel.h>

class MaterialCategoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MaterialCategoryWidget(MaterialCategoryModel* categoryModel,
                                    QWidget *parent = 0);

signals:

    void categoryChanged(MaterialCategory*);

public slots:

    void addCategory();
    void removeCategory();
    void categoryDoubleClicked(const QModelIndex& index);

protected:

    void changeEvent(QEvent *event);

    MaterialCategoryModel* categoryModel_;
    QListView* categories_;
    QPushButton* addCategoryButton_;
    QPushButton* removeCategoryButton_;
};

#endif // MATERIALCATEGORYWIDGET_H

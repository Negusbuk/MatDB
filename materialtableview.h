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

#ifndef MATERIALTABLEVIEW_H
#define MATERIALTABLEVIEW_H

#include <QTableWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMenu>

#include <materiallistmodel.h>
#include <materialselectionmodel.h>
#include <propertymodel.h>
#include <parametermodel.h>
#include <materialtableitem.h>

class MaterialTableView : public QTableWidget
{
    Q_OBJECT
public:
    explicit MaterialTableView(MaterialListModel *listmodel,
                               MaterialSelectionModel * selectionmodel,
                               PropertyModel *propertymodel,
                               ParameterModel *parametermodel,
                               QWidget *parent = 0);

signals:
    
public slots:

protected slots:
    void fillTable(int count);
    void setSelection(Material* selection);
    void metadataChanged(Material* material);
    void selectionChanged();
    void itemEdited(QTableWidgetItem * item);
    void displayContextMenu(const QPoint& point);

protected:
    MaterialListModel* ListModel_;
    MaterialSelectionModel * SelectionModel_;
    PropertyModel* PropertyModel_;
    ParameterModel* ParameterModel_;

    typedef std::map<const Material*,MaterialTableItem*> MaterialMap;
    MaterialMap indexMap_;

    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    QMenu* ContextMenu_;
};

#endif // MATERIALTABLEVIEW_H

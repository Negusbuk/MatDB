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

#ifndef MATERIALPARAMETERVIEW_H
#define MATERIALPARAMETERVIEW_H

#include <QTableWidget>
#include <QComboBox>
#include <QMenu>
#include <QAction>

#include <materiallistmodel.h>
#include <materialselectionmodel.h>
#include <propertyselectionmodel.h>
#include <parameterselectionmodel.h>
#include <unitcombobox.h>
#include <unit.h>

class MaterialParameterViewItem : public QTableWidgetItem
{
public:
    explicit MaterialParameterViewItem(Parameter *parameter,
                                       ParameterValue* pv,
                                       int column);
    ParameterValue* getParameterValue() { return ParameterValue_; }
    int getColumn() const { return Column_; }
    void update();
    void setData(int role, const QVariant & value);

    bool isEditing() const { return isEditing_; }
    void setEditing(bool edit) { isEditing_ = edit; }

protected:
    Parameter* Parameter_;
    ParameterValue* ParameterValue_;
    int Column_;
    bool isEditing_;
};

class MaterialParameterView : public QWidget
{
    Q_OBJECT
public:
    explicit MaterialParameterView(MaterialListModel *listmodel,
                                   MaterialSelectionModel *selectionmodel,
                                   PropertySelectionModel *propertyselectionmodel,
                                   ParameterSelectionModel *parameterselectionmodel,
                                   QWidget *parent = 0);
    
signals:
    
public slots:
    void materialChanged(Material*);
    void parameterChanged(Parameter*);
    void parameterValueChanged(QTableWidgetItem*);
    void parameterValueSelected(QTableWidgetItem*);
    void temperatureUnitChanged(const QString& name);
    void valueUnitChanged(const QString& name);

protected slots:
    void displayContextMenu(const QPoint& point);
    void import();
    void deleteParameterRow();
    void deleteParameterRow(int row);
    void deleteTemperature();
    void deleteTemperature(int row);

protected:
    MaterialListModel* ListModel_;
    MaterialSelectionModel * SelectionModel_;
    PropertySelectionModel* PropertySelectionModel_;
    ParameterSelectionModel* ParameterSelectionModel_;

    QTableWidget* unitTable_;
    QTableWidget* valueTable_;

    UnitComboBox* TempUnitBox_;
    UnitComboBox* ValueUnitBox_;

    QMenu* ContextMenu_;
    QAction* DeleteAction_;
    QAction* DeleteTempAction_;
};

#endif // MATERIALPARAMETERVIEW_H

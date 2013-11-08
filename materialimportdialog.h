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

#ifndef MATERIALIMPORTDIALOG_H
#define MATERIALIMPORTDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QRadioButton>

#include <materiallistmodel.h>

class MaterialImportDialog : public QDialog
{
    Q_OBJECT
public:

    explicit MaterialImportDialog(MaterialListModel* model,
                                  QWidget *parent = 0);

    const std::vector<Material*>& getSelectedMaterials() const { return selectedMaterials_; }

signals:
    
public slots:
    
    void buildSelectedMaterials();
    void headerViewDoubleClicked(int logicalIndex);

protected:

    MaterialListModel* model_;
    QTableWidget* materialView_;
    std::vector<Material*> selectedMaterials_;
    bool allItemsChecked_;
};

#endif // MATERIALIMPORTDIALOG_H

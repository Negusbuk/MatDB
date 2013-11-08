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

#ifndef MATERIALXMLEXPORTDIALOG_H
#define MATERIALXMLEXPORTDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QRadioButton>

#include <materiallistmodel.h>

class MaterialXMLExportDialog : public QDialog
{
    Q_OBJECT
public:

    enum ExportMode {
        ANSYS           = 1,
        MatML           = 2,
        Unknown
    };

    explicit MaterialXMLExportDialog(MaterialListModel* model,
                                     QWidget *parent = 0);

    const std::vector<Material*>& getSelectedMaterials() const { return selectedMaterials_; }

    ExportMode getExportMode() const { return exportMode_; }

signals:
    
public slots:
    
    void exportMaterials();
    void headerViewDoubleClicked(int logicalIndex);
    void modeChanged(QAbstractButton*);

protected:

    MaterialListModel* model_;
    QTableWidget* materialView_;
    std::vector<Material*> selectedMaterials_;
    bool allItemsChecked_;
    QRadioButton* modeMATMLbutton_;
    QRadioButton* modeANSYSbutton_;
    ExportMode exportMode_;
};

#endif // MATERIALXMLEXPORTDIALOG_H

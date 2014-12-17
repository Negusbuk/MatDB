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

#ifndef MATERIALSELECTIONDIALOG_H
#define MATERIALSELECTIONDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>

#include <materiallistmodel.h>

class MaterialSelectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MaterialSelectionDialog(const QString& buttonText,
                                     MaterialListModel* model,
                                     QWidget *parent = 0);

    const std::vector<Material*>& getSelectedMaterials() const { return selectedMaterials_; }

    void setOkButtonText(const QString& text);

signals:

public slots:

    void okClicked();
    void headerViewDoubleClicked(int logicalIndex);

protected:

    MaterialListModel* model_;
    QTableWidget* materialView_;
    std::vector<Material*> selectedMaterials_;
    bool allItemsChecked_;
    QPushButton* okButton_;
};

#endif // MATERIALSELECTIONDIALOG_H

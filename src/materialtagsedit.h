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

#ifndef MATERIALTAGSEDIT_H
#define MATERIALTAGSEDIT_H

#include <nqtokenedit.h>

#include <materiallistmodel.h>
#include <materialselectionmodel.h>

class MaterialTagsEdit : public NQTokenEdit
{
    Q_OBJECT
public:
    explicit MaterialTagsEdit(MaterialListModel* listModel,
                              MaterialSelectionModel* selectionModel,
                              QWidget *parent = 0);

signals:

    void materialMetadataChanged(Material*);

public slots:

    void materialChanged(Material* material);
    void tagsChanged(const QStringList&);

protected:

    MaterialListModel* listModel_;
    MaterialSelectionModel* selectionModel_;
};

#endif // MATERIALTAGSEDIT_H

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

#ifndef MATERIALINDEXER_H
#define MATERIALINDEXER_H

#include <unordered_set>
#include <unordered_map>
#include <string>

#include <QObject>

#include <material.h>

class MaterialListModel;

class MaterialIndexer : public QObject
{
    Q_OBJECT
public:
    explicit MaterialIndexer(MaterialListModel *listmodel,
                             QObject *parent = 0);

    void filter(const QString& key, std::vector<Material*>& materials);

signals:

public slots:

    void materialCountChanged(int count);
    void materialMetadataChanged(Material*material);

    void dumpIndex();

protected:
    MaterialListModel* ListModel_;
    typedef std::map<std::string,std::unordered_set<Material*> > keyMapType;
    keyMapType keyMap_;
    typedef std::map<Material*,std::unordered_set<std::string> > materialMapType;
    materialMapType materialMap_;

    void processKey(const std::string& key, Material*material);
};

#endif // MATERIALINDEXER_H

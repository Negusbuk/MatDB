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

#include <QXmlStreamWriter>
#include <QFile>

#include <nqlogger.h>

#include "htmlwriter.h"

HTMLWriter::HTMLWriter(const std::vector<Material*>& materials,
                       PropertyModel *propmodel,
                       ParameterModel *paramodel,
                       QObject *parent) :
  QObject(parent),
  materials_(materials),
  propmodel_(propmodel),
  paramodel_(paramodel)
{

}

void HTMLWriter::write(const QDir& destination)
{
    NQLog("HTMLWriter", NQLog::Message) << "void write(const QDir& destination)";
    NQLog("HTMLWriter", NQLog::Message) << destination.absolutePath();

    QFile ofile(destination.absoluteFilePath("index.html"));
    if (ofile.open(QIODevice::WriteOnly)) {

        QXmlStreamWriter stream(&ofile);

        stream.setAutoFormatting(true);
        stream.writeStartDocument();

        for (std::vector<Material*>::const_iterator it = materials_.begin();
             it!=materials_.end();
             ++it) {
            Material* material = *it;
        }

        stream.writeEndDocument();

        ofile.close();
    }
}


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
#include <QTextStream>

#include <nqlogger.h>

#include "matmlwriter.h"

MATMLWriter::MATMLWriter(const std::vector<Material*>& materials,
                         PropertyModel *propmodel,
                         ParameterModel *paramodel,
                         QObject *parent) :
    QObject(parent),
    materials_(materials),
    propmodel_(propmodel),
    paramodel_(paramodel)
{

}

void MATMLWriter::write(QIODevice *destination, ExportMode mode)
{
    NQLog("MATMLWriter", NQLog::Message) << "void write(QIODevice *destination, ExportMode mode)";

    if (mode==Unknown) return;

    QXmlStreamWriter stream(destination);

    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    if (mode==ANSYS) {
        stream.writeStartElement("EngineeringData");
        stream.writeAttribute("version", "14.5.0.529");
        stream.writeAttribute("versiondate", "01.04.2013 14:31:00");

        stream.writeStartElement("Materials");
        stream.writeStartElement("MatML_Doc");
    } else if (mode==MatML) {
        stream.writeStartElement("MatML_Doc");
    }

    for (std::vector<Material*>::const_iterator it = materials_.begin();
         it!=materials_.end();
         ++it) {
        Material* material = *it;
        NQLog("MATMLWriter", NQLog::Message) << "XML write material " << material->getName();
        material->writeXML(stream);
    }

    stream.writeStartElement("Metadata");

    for (std::map<int,Parameter*>::const_iterator it = paramodel_->getParametersById().begin();
         it!=paramodel_->getParametersById().end();
         ++it) {

        Parameter* parameter = it->second;
        if (parameter->getId()<0) continue;
        NQLog("MATMLWriter", NQLog::Spam) << "XML write parameter " << parameter->getName()
                                          << " (" << parameter->getIdString().toStdString() << ")";
        parameter->writeXML(stream);
    }

    for (std::map<int,Property*>::const_iterator it = propmodel_->getPropertiesById().begin();
         it!=propmodel_->getPropertiesById().end();
         ++it) {

        Property* property = it->second;
        NQLog("MATMLWriter", NQLog::Spam) << "XML write property " << property->getName()
                                          << " (" << property->getIdString().toStdString() << ")";
        property->writeXML(stream);
    }

    stream.writeEndElement(); // Metadata

    if (mode==ANSYS) {
        stream.writeEndElement();
        stream.writeEndElement();
        stream.writeEndElement();
    } else if (mode==MatML) {
        stream.writeEndElement();
    }

    stream.writeEndDocument();
}


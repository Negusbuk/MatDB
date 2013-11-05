#include <QXmlStreamWriter>
#include <QTextStream>

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
        material->writeXML(stream);
    }

    stream.writeStartElement("Metadata");

    for (std::map<int,Parameter*>::const_iterator it = paramodel_->getParametersById().begin();
         it!=paramodel_->getParametersById().end();
         ++it) {

        Parameter* parameter = it->second;
        parameter->writeXML(stream);
    }

    for (std::map<int,Property*>::const_iterator it = propmodel_->getPropertiesById().begin();
         it!=propmodel_->getPropertiesById().end();
         ++it) {

        Property* property = it->second;
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


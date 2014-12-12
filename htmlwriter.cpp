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

#include <QFile>
#include <QDateTime>

#include <nqlogger.h>

#include "htmlwriter.h"

HTMLWriter::HTMLWriter(const std::vector<Material*>& materials,
                       PropertyModel *propmodel,
                       ParameterModel *paramodel,
                       MaterialCategoryModel *categorymodel,
                       QObject *parent) :
  QObject(parent),
  materials_(materials),
  propmodel_(propmodel),
  paramodel_(paramodel),
  categorymodel_(categorymodel)
{

}

void HTMLWriter::write(const QDir& destination)
{
    NQLog("HTMLWriter", NQLog::Message) << "void write(const QDir& destination)";
    NQLog("HTMLWriter", NQLog::Message) << destination.absolutePath();

    MaterialCategory* noCategory = categorymodel_->getCategory("No Category");
    map_t map;
    std::map<MaterialCategory*,int> count;

    for (std::vector<MaterialCategory*>::const_iterator itc = categorymodel_->getCategories().begin();
         itc!=categorymodel_->getCategories().end();
         ++itc) {
        count[*itc] = 0;
    }

    for (std::vector<Material*>::const_iterator it = materials_.begin();
         it!=materials_.end();
         ++it) {
        Material* material = *it;
        if (material->getCategory()) {
            map[material->getCategory()].push_back(material);
            count[material->getCategory()]++;
        } else {
            map[noCategory].push_back(material);
            count[noCategory]++;
        }
    }

    QFile hfile(":/html/header.html");
    hfile.open(QFile::ReadOnly);
    QString header(hfile.readAll());
    hfile.close();
    header.replace("@TITLE@", tr(".: MatDB Materials :."));

    QFile ffile(":/html/footer.html");
    ffile.open(QFile::ReadOnly);
    QString footer(ffile.readAll());
    ffile.close();

    QString xml;
    QXmlStreamWriter stream(&xml);
    stream.setAutoFormatting(true);
    stream.setAutoFormattingIndent(2);

    stream.writeStartElement("body");
    stream.writeAttribute("id", "MatDB");

    stream.writeStartElement("div");
    stream.writeAttribute("align", "center");

    stream.writeStartElement("div");
    stream.writeAttribute("class", "MatDBContent");

    //writeMaterialTable(stream, map, destination);
    writeMaterialCloud(stream, map, destination);

    stream.writeStartElement("div");
    stream.writeAttribute("id", "stats");
    stream.writeAttribute("align", "right");

    stream.writeStartElement("table");
    //stream.writeAttribute("class", "MatDBStats");

    for (std::vector<MaterialCategory*>::const_iterator itc = categorymodel_->getCategories().begin();
         itc!=categorymodel_->getCategories().end();
         ++itc) {

        std::map<MaterialCategory*,int>::iterator itfind = count.find(*itc);
        if (itfind!=count.end()) {
            stream.writeStartElement("tr");

            stream.writeStartElement("td");
            stream.writeAttribute("bgcolor", (*itc)->getColor().name());
            stream.writeAttribute("width", "10");
            stream.writeEndElement(); // td

            stream.writeStartElement("td");
            stream.writeAttribute("style", "padding-right: 20px;");
            stream.writeCharacters((*itc)->getName());
            stream.writeEndElement(); // td

            stream.writeStartElement("td");
            stream.writeAttribute("align", "right");
            stream.writeCharacters(QString::number(itfind->second));
            stream.writeEndElement(); // td

            stream.writeEndElement(); // tr
        }
    }

    stream.writeEndElement(); // table
    stream.writeEndElement(); // div

    stream.writeStartElement("div");
    stream.writeAttribute("style", "width:100%;");
    stream.writeAttribute("align", "right");

    QString generator = tr("Generated on");
    generator += " ";
    generator += QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
    generator += " ";
    generator += tr("by");
    stream.writeStartElement("p");
    stream.writeCharacters(generator + " ");
    stream.writeStartElement("a");
    stream.writeAttribute("href", "http://negusbuk.github.io/MatDB/");
    stream.writeCharacters("MatDB");
    stream.writeEndElement(); // a
    stream.writeEndElement(); // p
    stream.writeEndElement(); // div

    stream.writeEndElement();
    stream.writeEndElement();
    stream.writeEndElement();

    QFile ofile(destination.absoluteFilePath("index.html"));
    if (ofile.open(QIODevice::WriteOnly)) {
        QTextStream ts(&ofile);

        ts << header;
        ts << xml;
        ts << footer;
        ofile.close();
    }
}

void HTMLWriter::writeMaterialTable(QXmlStreamWriter& stream, map_t& map, const QDir& destination)
{
    stream.writeStartElement("div");
    stream.writeAttribute("align", "left");

    for (std::vector<MaterialCategory*>::const_iterator itc = categorymodel_->getCategories().begin();
         itc!=categorymodel_->getCategories().end();
         ++itc) {

        std::map<MaterialCategory*,std::vector<Material*> >::iterator itfind = map.find(*itc);
        if (itfind!=map.end()) {
            std::vector<Material*>& mats = itfind->second;

            if (mats.size()==0) continue;

            stream.writeStartElement("table");
            stream.writeAttribute("class", "MatDBHeader");
            stream.writeStartElement("tr");

            stream.writeStartElement("td");
            stream.writeAttribute("bgcolor", (*itc)->getColor().name());
            stream.writeAttribute("width", "20");
            stream.writeEndElement(); // td

            stream.writeStartElement("td");
            stream.writeAttribute("style", "padding-right: 20px;");
            stream.writeCharacters((*itc)->getName());
            stream.writeEndElement(); // td

            stream.writeEndElement(); // tr
            stream.writeEndElement(); // table

            stream.writeStartElement("table");
            //stream.writeAttribute("class", "MatDBTable");

            for (std::vector<Material*>::iterator itm = mats.begin();
                 itm!=mats.end();
                 ++itm) {
                stream.writeStartElement("tr");

                stream.writeStartElement("td");
                stream.writeAttribute("width", "250px");
                stream.writeStartElement("a");
                QString url = (*itm)->getUUID();
                url.remove(0, 1);
                url.remove(url.length()-1, 1);
                url.prepend("./mat_");
                url += ".html";
                stream.writeAttribute("href", url);
                stream.writeCharacters((*itm)->getName());
                stream.writeEndElement(); // a
                stream.writeEndElement(); // td

                stream.writeTextElement("td", (*itm)->getDescription());

                writeMaterial(*itm, destination.absoluteFilePath(url));
            }

            stream.writeEndElement(); // table
        }
    }

    stream.writeEndElement();
}

void HTMLWriter::writeMaterialCloud(QXmlStreamWriter& stream, map_t& map, const QDir& destination)
{
    stream.writeStartElement("div");
    stream.writeAttribute("align", "center");

    QString colorString;
    QString bgcolorString;

    float minLength = 1000;
    float maxLength = 0;
    for (std::vector<Material*>::const_iterator it = materials_.begin();
         it != materials_.end();
         ++it) {
        if ((*it)->getName().size()>maxLength) maxLength = (*it)->getName().size();
        if ((*it)->getName().size()<minLength) minLength = (*it)->getName().size();
    }

    for (std::vector<Material*>::const_iterator it = materials_.begin();
         it != materials_.end();
         ++it) {

        const MaterialCategory* category = (*it)->getCategory();
        if (category==0) {
            colorString = "#000000;";
            bgcolorString = "#FFFFFF;";
        } else {

            const QColor& c = category->getColor();
            QColor bg(255 - c.red(), 255 - c.green(), 255 - c.blue());
            colorString = bg.name() + ";";

            /*
            bgcolorString = "#FFFFFF;";
            qreal lightness = category->getColor().lightnessF();
            if (lightness>0.4) {
                QColor bg(255*(1.4-lightness), 255*(1.4-lightness), 255*(1.4-lightness));
                bgcolorString = bg.name() + ";";
            }
            NQLog("HTMLWriter", NQLog::Message) << "color " << category->getColor().lightnessF();
            */

            bgcolorString = category->getColor().name() + ";";
        }

        int fontsize = 10 + (maxLength-(*it)->getName().size())*8.0/maxLength;

        stream.writeStartElement("span");
        stream.writeAttribute("id", "button");
        stream.writeAttribute("style", QString("background-color:") + bgcolorString);
        stream.writeStartElement("a");
        stream.writeAttribute("id", "float");
        stream.writeAttribute("style", QString("color:") + colorString +
                              QString("font-size:") + QString::number(fontsize) + "px;");
        QString url = (*it)->getUUID();
        url.remove(0, 1);
        url.remove(url.length()-1, 1);
        url.prepend("./mat_");
        url += ".html";
        stream.writeAttribute("href", url);
        stream.writeCharacters((*it)->getName());
        stream.writeEndElement(); // a
        stream.writeEndElement(); // span
        //if (stream.device()) stream.device()->write("&nbsp;");

        writeMaterial(*it, destination.absoluteFilePath(url));
    }

    stream.writeEndElement();
}

void HTMLWriter::writeMaterial(Material* material, const QString& filename)
{
    MaterialCategory* noCategory = categorymodel_->getCategory("No Category");

    QFile hfile(":/html/header.html");
    hfile.open(QFile::ReadOnly);
    QString header(hfile.readAll());
    hfile.close();
    QString title = tr(".: MatDB Material - ");
    title += material->getName();
    title += " :.";
    header.replace("@TITLE@", title);

    QFile ffile(":/html/footer.html");
    ffile.open(QFile::ReadOnly);
    QString footer(ffile.readAll());
    ffile.close();

    QString xml;
    QXmlStreamWriter stream(&xml);
    stream.setAutoFormatting(true);
    stream.setAutoFormattingIndent(2);

    stream.writeStartElement("body");
    stream.writeAttribute("id", "MatDB");

    stream.writeStartElement("div");
    stream.writeAttribute("align", "center");

    stream.writeStartElement("div");
    stream.writeAttribute("class", "MatDBContent");

    stream.writeStartElement("div");
    stream.writeAttribute("align", "left");

    stream.writeStartElement("table");
    //stream.writeAttribute("class", "MatDBTable");
    stream.writeAttribute("style", "width:100%;");
    stream.writeStartElement("tr");

    stream.writeStartElement("td");
    if (material->getCategory()) {
         stream.writeAttribute("bgcolor", material->getCategory()->getColor().name());
    } else {
        stream.writeAttribute("bgcolor", noCategory->getColor().name());
    }
    stream.writeAttribute("width", "40");
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBHeader");
    stream.writeCharacters(material->getName());
    stream.writeEndElement(); // td

    stream.writeStartElement("td");
    stream.writeAttribute("align", "right");
    stream.writeAttribute("valign", "top");
    stream.writeStartElement("a");
    stream.writeAttribute("href", "./index.html");
    stream.writeCharacters(tr("back"));
    stream.writeEndElement(); // a
    stream.writeEndElement(); // td

    stream.writeEndElement(); // tr
    stream.writeEndElement(); // table

    stream.writeStartElement("table");
    //stream.writeAttribute("class", "MatDBTable");
    stream.writeAttribute("style", "width:100%;");

    stream.writeStartElement("tr");
    stream.writeStartElement("td");
    stream.writeAttribute("style", "width:150px;");
    stream.writeAttribute("class", "MatDBTitle");
    stream.writeCharacters(tr("Description"));
    stream.writeEndElement(); // td
    stream.writeTextElement("td", material->getDescription());
    stream.writeEndElement(); // tr

    stream.writeStartElement("tr");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBTitle");
    stream.writeCharacters(tr("Category"));
    stream.writeEndElement(); // td
    if (material->getCategory()) {
         stream.writeTextElement("td", material->getCategory()->getName());
    } else {
        stream.writeTextElement("td", noCategory->getName());
    }
    stream.writeEndElement(); // tr

    stream.writeStartElement("tr");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBTitle");
    stream.writeCharacters(tr("Tags"));
    stream.writeEndElement(); // td
    stream.writeStartElement("td");
    QStringList tags = material->getTags();
    for (QStringList::Iterator it=tags.begin();
         it!=tags.end();
         ++it) {
        stream.writeStartElement("span");
        stream.writeAttribute("class", "MatDBTag");
        stream.writeCharacters(*it);
        stream.writeEndElement(); // span
    }
    stream.writeEndElement(); // td
    stream.writeEndElement(); // tr

    stream.writeStartElement("tr");
    stream.writeStartElement("td");
    stream.writeAttribute("class", "MatDBTitle");
    stream.writeCharacters(tr("Notes"));
    stream.writeEndElement(); // td
    stream.writeStartElement("td");
    QStringList notes = material->getNotes().split("\n");
    for (QStringList::Iterator it=notes.begin();
         it!=notes.end();
         ++it) {
        if ((*it).length()==0) continue;
        stream.writeStartElement("p");
        stream.writeAttribute("id", "MatDBNote");
        stream.writeCharacters(*it);
        stream.writeEndElement(); // p
    }
    stream.writeEndElement(); // td
    stream.writeEndElement(); // tr

    stream.writeEndElement(); // table

    stream.writeStartElement("table");
    stream.writeAttribute("style", "width:100%;border-bottom:1px solid #000;");
    stream.writeAttribute("rules", "groups");

    const std::vector<Property*> properties = material->getSortedProperties();
    for (std::vector<Property*>::const_iterator it=properties.begin();
         it!=properties.end();
         ++it) {
        stream.writeStartElement("tbody");
        (*it)->writeHTML(stream);
        stream.writeEndElement(); // tbody
    }

    stream.writeEndElement(); // table

    stream.writeEndElement(); // div

    stream.writeStartElement("div");
    stream.writeAttribute("style", "width:100%;");
    stream.writeAttribute("align", "right");

    QString generator = tr("Generated on");
    generator += " ";
    generator += QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
    generator += " ";
    generator += tr("by");
    stream.writeStartElement("p");
    stream.writeCharacters(generator + " ");
    stream.writeStartElement("a");
    stream.writeAttribute("href", "http://negusbuk.github.io/MatDB/");
    stream.writeCharacters("MatDB");
    stream.writeEndElement(); // a
    stream.writeEndElement(); // p
    stream.writeEndElement(); // div

    stream.writeEndElement();
    stream.writeEndElement();
    stream.writeEndElement();

    QFile ofile(filename);
    if (ofile.open(QIODevice::WriteOnly)) {
        QTextStream ts(&ofile);

        ts << header;
        ts << xml;
        ts << footer;
        ofile.close();
    }
}

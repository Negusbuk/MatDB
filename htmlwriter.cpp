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
    std::map<MaterialCategory*,std::vector<Material*> > map;
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
    header.replace("@TITLE@", ".: MatDB Materials :.");

    QFile ffile(":/html/footer.html");
    ffile.open(QFile::ReadOnly);
    QString footer(ffile.readAll());
    ffile.close();

    QString xml;
    QXmlStreamWriter stream(&xml);
    stream.setAutoFormatting(true);

    stream.writeStartElement("body");
    stream.writeAttribute("class", "MatDB");

    for (std::vector<MaterialCategory*>::const_iterator itc = categorymodel_->getCategories().begin();
         itc!=categorymodel_->getCategories().end();
         ++itc) {

        std::map<MaterialCategory*,std::vector<Material*> >::iterator itfind = map.find(*itc);
        if (itfind!=map.end()) {
            std::vector<Material*>& mats = itfind->second;

            if (mats.size()==0) continue;

            stream.writeTextElement("h1", (*itc)->getName());

            for (std::vector<Material*>::iterator itm = mats.begin();
                 itm!=mats.end();
                 ++itm) {
                stream.writeTextElement("h3", (*itm)->getName());
            }
        }
    }

    for (std::vector<MaterialCategory*>::const_iterator itc = categorymodel_->getCategories().begin();
         itc!=categorymodel_->getCategories().end();
         ++itc) {

        std::map<MaterialCategory*,int>::iterator itfind = count.find(*itc);
        if (itfind!=count.end()) {
            stream.writeTextElement("h3", (*itc)->getName());
            stream.writeTextElement("h3", QString::number(itfind->second));
        }
    }

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


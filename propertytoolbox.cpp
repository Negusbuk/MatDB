#include <iostream>

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include "propertytoolbox.h"

PropertyToolBoxItem::PropertyToolBoxItem(const QString &name,
                                         Property *property,
                                         QWidget *parent) :
    QLabel(name, parent),
    Property_(property)
{
    //setFrameShape(Box);
    setFrameShadow(Plain);
}

void PropertyToolBoxItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QDrag *drag = new QDrag(this);

        QMimeData *mimeData = new QMimeData;
        mimeData->setText(Property_->getName());
        drag->setMimeData(mimeData);

        drag->exec(Qt::CopyAction, Qt::CopyAction);
    }
}

PropertyToolBox::PropertyToolBox(PropertyModel *model,
                                 QWidget *parent) :
    QToolBox(parent),
    PropertyModel_(model)
{
    build();

    setMinimumWidth(300);
    setMinimumHeight(300);
}

void PropertyToolBox::build()
{
    QWidget * box;
    PropertyToolBoxItem * item;
    std::map<QString,QWidget*> categoryWidgetMap;
    const std::vector<QString>& categories = PropertyModel_->getCategories();
    for (std::vector<QString>::const_iterator it = categories.begin();
         it!=categories.end();
         ++it) {
        QString s = *it;
        //std::cout << s.toStdString() << std::endl;

        //box = new QWidget(this);
        //QVBoxLayout * layout = new QVBoxLayout(box);
        //box->setLayout(layout);

        //CategoryWidgetMap_[s] = box;
    }

    const std::map<QString,std::vector<Property*> >& m = PropertyModel_->getPropertiesByCategory();
    for (std::map<QString,std::vector<Property*> >::const_iterator itm = m.begin();
         itm!=m.end();
         ++itm) {
        const std::vector<Property*>& v = itm->second;
        QString s = itm->first;
        //std::cout << s.toStdString() << std::endl;

        box = new QWidget(this);
        categoryWidgetMap[s] = box;
        QVBoxLayout * layout = new QVBoxLayout(box);
        box->setLayout(layout);

        for (std::vector<Property*>::const_iterator itv = v.begin();
             itv!=v.end();
             ++itv) {
            //std::cout << (*itv)->getName().toStdString() << std::endl;
            item = new PropertyToolBoxItem((*itv)->getName(), (*itv), box);
            layout->addWidget(item);
        }
        layout->addStretch(1);
    }

    for (std::vector<QString>::const_iterator it = categories.begin();
         it!=categories.end();
         ++it) {
        QString s = *it;

        box = categoryWidgetMap[s];
        if (!box) continue;
        //std::cout << s.toStdString() << std::endl;
        addItem(box, s);
    }
}

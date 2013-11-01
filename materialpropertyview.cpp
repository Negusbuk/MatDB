#include <iostream>

#include <QHeaderView>

#include "materialpropertyview.h"

MaterialPropertyViewParameterItem::MaterialPropertyViewParameterItem(Material * material,
                                                                     Property * property,
                                                                     Parameter * parameter,
                                                                     QTreeWidget *parent) :
    QTreeWidgetItem(parent, 1002),
    Material_(material),
    Property_(property),
    Parameter_(parameter)
{
    setTextAlignment(1, Qt::AlignRight);
    setTextAlignment(2, Qt::AlignLeft);

    setText(0, Property_->getName());

    Parameter_->setViewItem(this);

    update();
}

MaterialPropertyViewParameterItem::MaterialPropertyViewParameterItem(Material * material,
                                                                     Property * property,
                                                                     Parameter * parameter,
                                                                     MaterialPropertyViewItem *parent) :
    QTreeWidgetItem(parent, 1002),
    Material_(material),
    Property_(property),
    Parameter_(parameter)
{
    setTextAlignment(1, Qt::AlignRight);
    setTextAlignment(2, Qt::AlignLeft);

    setText(0, Parameter_->getName());

    Parameter_->setViewItem(this);

    update();
}

int MaterialPropertyViewParameterItem::getNumberOfParameters()
{
    return Property_->getParameters().size();
}

void MaterialPropertyViewParameterItem::update()
{
   if (Parameter_->getNumberOfValues()==0) {
        setText(1, "undefined");
        setText(2, "");
        setBackground(1, QBrush(Qt::red));
        setBackground(2, QBrush(Qt::red));
    } else if (Parameter_->getNumberOfValues()==1) {
        const ParameterValue value = Parameter_->getValues().front();
        if (value.isValueValid()) {
            setText(1, value.prettyValue());
            setText(2, Parameter_->getValueUnit()->currentUnitAsString());
            setBackground(1, background(0));
            setBackground(2, background(0));
        } else {
            setText(1, "undefined");
            setText(2, "");
            setBackground(1, QBrush(Qt::red));
            setBackground(2, QBrush(Qt::red));
        }
    } else {
        setText(1, "table");
        setText(2, "");
    }
}

MaterialPropertyViewItem::MaterialPropertyViewItem(Material * material,
                                                   Property * property,
                                                   QTreeWidget *parent) :
    QTreeWidgetItem(parent, 1001),
    Material_(material),
    Property_(property)
{
    setText(0, Property_->getName());
    setText(1, "");
    setText(2, "");

    std::map<QString,Parameter*>& map = Property_->getParameters();
    for (std::map<QString,Parameter*>::iterator it = map.begin();
         it!=map.end();
         ++it) {
        addChild(new MaterialPropertyViewParameterItem(material, property, it->second, this));
    }
}


void MaterialPropertyViewItem::update()
{

}

MaterialPropertyView::MaterialPropertyView(MaterialListModel *listmodel,
                                           MaterialSelectionModel * selectionmodel,
                                           PropertyModel *propertymodel,
                                           ParameterModel *parametermodel,
                                           PropertySelectionModel* propertyselectionmodel,
                                           ParameterSelectionModel* parameterselectionmodel,
                                           QWidget *parent) :
    QTreeWidget(parent),
    ListModel_(listmodel),
    SelectionModel_(selectionmodel),
    PropertyModel_(propertymodel),
    ParameterModel_(parametermodel),
    PropertySelectionModel_(propertyselectionmodel),
    ParameterSelectionModel_(parameterselectionmodel)
{
    setAttribute(Qt::WA_MacShowFocusRect, 0);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(displayContextMenu(const QPoint&)));

    ContextMenu_ = new QMenu();
    ContextMenu_->addAction("Delete", this, SLOT(deleteProperty()));

    setMinimumWidth(500);
    setMaximumWidth(550);
    setMinimumHeight(200);

    setColumnCount(3);

    QStringList headers;
    headers << "Property";
    headers << "Value";
    headers << "Unit";
    setHeaderLabels(headers);

    QHeaderView* hv = header();
    hv->setStretchLastSection(false);
    hv->setResizeMode(0, QHeaderView::Stretch);
    hv->setResizeMode(1, QHeaderView::Fixed);
    setColumnWidth(1, 100);
    hv->resizeSection(1, 100);
    hv->setResizeMode(2, QHeaderView::Fixed);
    setColumnWidth(2, 100);
    hv->resizeSection(2, 100);

    connect(SelectionModel_, SIGNAL(selectionChanged(Material*)),
            this, SLOT(materialChanged(Material*)));

    connect(ParameterSelectionModel_, SIGNAL(parameterModified(Parameter*)),
            this, SLOT(parameterModified(Parameter*)));

    connect(this, SIGNAL(itemSelectionChanged()),
            this, SLOT(selectionChanged()));

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DropOnly);

    setAlternatingRowColors(true);

    setSortingEnabled(false);
}

void MaterialPropertyView::materialChanged(Material* material)
{
    std::cout << "void MaterialPropertyView::materialChanged()" << std::endl;

    clear();

    const std::vector<Property*>& v = material->getSortedProperties();
    for (std::vector<Property*>::const_iterator it = v.begin();
         it!=v.end();
         ++it) {
        //std::cout << it->first.toStdString() << std::endl;

        std::map<QString,Parameter*>& pmap = (*it)->getParameters();
        if (pmap.size()==1) {
            Parameter* parameter = (pmap.begin())->second;
            std::cout << parameter->getValueUnit()->currentUnitAsString().toStdString() << std::endl;
            addTopLevelItem(new MaterialPropertyViewParameterItem(material, *it, parameter, this));
        } else {
            MaterialPropertyViewItem* item = new MaterialPropertyViewItem(material, *it, this);
            addTopLevelItem(item);
            setFirstItemColumnSpanned(item, true);
        }
    }

    expandAll();

    resetInputContext();

    ParameterSelectionModel_->setSelection(NULL);
}

void MaterialPropertyView::selectionChanged()
{
    std::cout << "MaterialPropertyView::selectionChanged()" << std::endl;

    QList<QTreeWidgetItem*> items = selectedItems();
    if (items.count()==1) {
        QTreeWidgetItem* item = items.first();
        std::cout << item->type() << std::endl;
        if (item->type()==1002) {
            MaterialPropertyViewParameterItem* pitem = dynamic_cast<MaterialPropertyViewParameterItem*>(item);
            PropertySelectionModel_->setSelection(NULL);
            ParameterSelectionModel_->setSelection(pitem->getParameter());
        } else {
            MaterialPropertyViewItem* pitem = dynamic_cast<MaterialPropertyViewItem*>(item);
            PropertySelectionModel_->setSelection(pitem->getProperty());
            pitem->getProperty()->recalculate();
            ParameterSelectionModel_->setSelection(NULL);
        }
    }
}

void MaterialPropertyView::parameterModified(Parameter* parameter)
{
    std::cout << "void MaterialPropertyView::parameterModified(Parameter* parameter)" << std::endl;

    if (parameter->getViewItem()) parameter->getViewItem()->update();
}

void MaterialPropertyView::dragEnterEvent(QDragEnterEvent *event)
{
    //std::cout << "dragEnterEvent" << std::endl;
    event->acceptProposedAction();
}

void MaterialPropertyView::dragMoveEvent(QDragMoveEvent *event)
{
    //std::cout << "dragMoveEvent" << std::endl;

    if (SelectionModel_->getSelection()==NULL) {
        event->ignore();
        return;
    }

    Material * material = SelectionModel_->getSelection();

    Property * property = material->getProperty(event->mimeData()->text());
    if (property) {
        event->ignore();
        return;
    }

    event->acceptProposedAction();
}

void MaterialPropertyView::dropEvent(QDropEvent *event)
{
    //std::cout << "dropEvent" << std::endl;

    if (SelectionModel_->getSelection()==NULL) {
        event->ignore();
        return;
    }

    Material * material = SelectionModel_->getSelection();
    Property * property = material->getProperty(event->mimeData()->text());
    if (property) {
        event->ignore();
        return;
    }

    property = PropertyModel_->getProperty(event->mimeData()->text());
    if (!property) {
        event->ignore();
        return;
    }

    material->addProperty(property);

    materialChanged(material);

    event->acceptProposedAction();
}

void MaterialPropertyView::deleteProperty()
{
    std::cout << "delete" << std::endl;

    Property * property = 0;
    Material * material = 0;

    QList<QTreeWidgetItem*> items = selectedItems();
    if (items.count()==1) {
        QTreeWidgetItem* item = items.first();
        if (item->type()==1001) {
            MaterialPropertyViewItem* pitem = dynamic_cast<MaterialPropertyViewItem*>(item);
            property = pitem->getProperty();
            material = pitem->getMaterial();
        }
        if (item->type()==1002) {
            MaterialPropertyViewParameterItem* pitem = dynamic_cast<MaterialPropertyViewParameterItem*>(item);
            if (pitem->getNumberOfParameters()==1) {
                property = pitem->getProperty();
                material = pitem->getMaterial();
            }
        }

        if (property && material) {
            int i = indexOfTopLevelItem(item);
            takeTopLevelItem(i);
            delete item;

            material->removeProperty(property);
        }
    }
}

void MaterialPropertyView::displayContextMenu(const QPoint& point)
{
    std::cout << "context" << std::endl;

    QList<QTreeWidgetItem*> items = selectedItems();
    if (items.count()==1) {
        QTreeWidgetItem* item = items.first();
        if (item->type()==1001) {
            ContextMenu_->exec(mapToGlobal(point));
        }
        if (item->type()==1002) {
            MaterialPropertyViewParameterItem* pitem = dynamic_cast<MaterialPropertyViewParameterItem*>(item);
            if (pitem->getNumberOfParameters()==1) {
                ContextMenu_->exec(mapToGlobal(point));
            }
        }
    }
}

#include <QBoxLayout>
#include <QHeaderView>
#include <QFileDialog>
#include <QDesktopServices>

#include <nqlogger.h>

#include <materialparameterview.h>

MaterialParameterViewItem::MaterialParameterViewItem(ParameterValue* parameter,
                                                     int column) :
    ParameterValue_(parameter),
    Column_(column)
{
    update();
}

void MaterialParameterViewItem::update()
{
    if (ParameterValue_!=0) {
        QString t;
        if (Column_==0) {
            if (ParameterValue_->isTemperatureValid()) {
                setText(ParameterValue_->prettyTemperature());
            } else {
                setText("");
            }
        } else {
            if (ParameterValue_->isValueValid()) {
                setText(ParameterValue_->prettyValue());
            } else {
                setText("");
            }
        }
    }
}

MaterialParameterView::MaterialParameterView(MaterialListModel *listmodel,
                                             MaterialSelectionModel * selectionmodel,
                                             PropertySelectionModel* propertyselectionmodel,
                                             ParameterSelectionModel* parameterselectionmodel,
                                             QWidget *parent) :
    QWidget(parent),
    ListModel_(listmodel),
    SelectionModel_(selectionmodel),
    PropertySelectionModel_(propertyselectionmodel),
    ParameterSelectionModel_(parameterselectionmodel)
{
    setAttribute(Qt::WA_MacShowFocusRect, 0);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(displayContextMenu(const QPoint&)));

    ContextMenu_ = new QMenu();
    ContextMenu_->addAction("Import", this, SLOT(import()));
    ContextMenu_->addSeparator();
    DeleteAction_ = ContextMenu_->addAction("Delete", this, SLOT(deleteParameterRow()));
    DeleteTempAction_ = ContextMenu_->addAction("Delete Temperature", this, SLOT(deleteTemperature()));

    QVBoxLayout * layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    unitTable_ = new QTableWidget(this);
    layout->addWidget(unitTable_, 0);

    valueTable_ = new QTableWidget(this);
    valueTable_->horizontalHeader()->setVisible(false);
    layout->addWidget(valueTable_);

    setLayout(layout);

    valueTable_->setMinimumWidth(400);
    valueTable_->setMinimumHeight(200);

    unitTable_->setColumnCount(2);
    unitTable_->setHorizontalHeaderItem(0, new QTableWidgetItem("Temperature"));
    unitTable_->setHorizontalHeaderItem(1, new QTableWidgetItem(""));
    unitTable_->setRowCount(1);

    valueTable_->setColumnCount(2);

    valueTable_->setRowCount(1);

    QHeaderView* hv;
    hv = unitTable_->horizontalHeader();
    hv->setStretchLastSection(true);
    hv->setResizeMode(0, QHeaderView::Fixed);
    unitTable_->setColumnWidth(1, 150);
    hv->resizeSection(1, 150);

    hv = valueTable_->horizontalHeader();
    hv->setStretchLastSection(true);
    hv->setResizeMode(0, QHeaderView::Fixed);
    valueTable_->setColumnWidth(1, 150);
    hv->resizeSection(1, 150);

    unitTable_->verticalHeader()->setFixedWidth(40);
    valueTable_->verticalHeader()->setFixedWidth(40);

    TempUnitBox_ = new UnitComboBox(0, unitTable_);
    unitTable_->setCellWidget(0, 0, TempUnitBox_);

    ValueUnitBox_ = new UnitComboBox(0, unitTable_);
    unitTable_->setCellWidget(0, 1, ValueUnitBox_);

    unitTable_->setVerticalHeaderItem(0, new QTableWidgetItem(""));
    valueTable_->setVerticalHeaderItem(0, new QTableWidgetItem("*"));

    valueTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    valueTable_->setEditTriggers(QAbstractItemView::AllEditTriggers);

    connect(SelectionModel_, SIGNAL(selectionChanged(Material*)),
            this, SLOT(materialChanged(Material*)));

    connect(ParameterSelectionModel_, SIGNAL(selectionChanged(Parameter*)),
            this, SLOT(parameterChanged(Parameter*)));

    connect(valueTable_, SIGNAL(itemChanged(QTableWidgetItem*)),
            this, SLOT(parameterValueChanged(QTableWidgetItem*)));

    connect(TempUnitBox_,SIGNAL(activated(const QString&)),
            this, SLOT(temperatureUnitChanged(const QString&)));

    connect(ValueUnitBox_,SIGNAL(activated(const QString&)),
            this, SLOT(valueUnitChanged(const QString&)));

    updateGeometry();
    int height = valueTable_->rowHeight(0);
    height += unitTable_->horizontalHeader()->height();
    unitTable_->setFixedHeight(height+1);
}

void MaterialParameterView::materialChanged(Material*)
{

}

void MaterialParameterView::parameterChanged(Parameter* parameter)
{
    NQLog("MaterialParameterView", NQLog::Spam) << "void parameterChanged(Parameter* parameter)";

    if (parameter==0) {
        if (TempUnitBox_) TempUnitBox_->setUnit(0);
        if (ValueUnitBox_) ValueUnitBox_->setUnit(0);
        valueTable_->setRowCount(1);
        valueTable_->setVerticalHeaderItem(0, new QTableWidgetItem("*"));
        valueTable_->item(0,0)->setText("");
        valueTable_->item(0,1)->setText("");
        return;
    }

    unitTable_->horizontalHeaderItem(1)->setText(parameter->getName());

    if (!parameter->isDependent()) {
        valueTable_->setRowCount(1+parameter->getNumberOfValues());
    } else {
        valueTable_->setRowCount(parameter->getNumberOfValues());
    }
    if (!parameter->isTemperatureDependent()) {
        valueTable_->setRowCount(1);
    }

    if (!parameter->isTemperatureDependent()) {
        TempUnitBox_->setEnabled(false);
    } else {
        TempUnitBox_->setEnabled(true);
    }

    int count = 0;
    for (std::vector<ParameterValue>::iterator it=parameter->getValues().begin();
         it!=parameter->getValues().end();
         ++it) {

        MaterialParameterViewItem * item0 = new MaterialParameterViewItem(&(*it), 0);
        MaterialParameterViewItem * item1 = new MaterialParameterViewItem(&(*it), 1);

        if (parameter->isDependent()) {
            item0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item0->setBackground(QBrush(Qt::yellow));
            item1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item1->setBackground(QBrush(Qt::yellow));
        }

        if (!parameter->isTemperatureDependent()) {
            item0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        }

        valueTable_->setItem(count, 0, item0);
        valueTable_->setItem(count, 1, item1);

        valueTable_->setVerticalHeaderItem(count, new QTableWidgetItem(QString().setNum(count+1)));

        count++;
    }

    if (parameter->isTemperatureDependent() || count==0) {
        if (!parameter->isDependent()) {
            valueTable_->setVerticalHeaderItem(count, new QTableWidgetItem("*"));
            MaterialParameterViewItem * item0 = new MaterialParameterViewItem(0, 0);

            if (!parameter->isTemperatureDependent()) {
                item0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            }

            valueTable_->setItem(count, 0, item0);
            valueTable_->setItem(count, 1, new MaterialParameterViewItem(0, 1));
        }
    }

    TempUnitBox_->setUnit(parameter->getTemperatureUnit());
    ValueUnitBox_->setUnit(parameter->getValueUnit());
}

void MaterialParameterView::parameterValueChanged(QTableWidgetItem* item)
{
    if (!item) return;
    QList<QTableWidgetItem*> items = valueTable_->selectedItems();
    if (items.count()!=1) return;
    if (items.first()!=item) return;

    MaterialParameterViewItem* pitem = dynamic_cast<MaterialParameterViewItem*>(item);
    Parameter* parameter = ParameterSelectionModel_->getSelection();
    if (!parameter) return;
    Property* property = parameter->getProperty();
    ParameterValue* pvalue = 0;
    if (pitem) pvalue = pitem->getParameterValue();

    int row = valueTable_->currentRow();

    int pos;
    bool ok;
    QString text = item->text();
    double value = text.toDouble(&ok);

    if (text.isEmpty()) {
        valueTable_->setCurrentItem(0);
        if (item->column()==1) {
            deleteParameterRow(row);
        } else {
            deleteTemperature(row);
        }
        return;
    }

    QValidator::State state;
    if (item->column()==0) {
        state = parameter->getTemperatureUnit()->validate(text, pos);
    } else {
        state = parameter->getValueUnit()->validate(text, pos);
    }

    if (state!=QValidator::Acceptable) {
        if (pvalue) {
            if (item->column()==0) {
                pvalue->resetTemperature();
            } else {
                pvalue->resetValue();
            }
        }
        item->setText("");
        return;
    }

    if (pvalue) {
        if (item->column()==0) {
            pvalue->setTemperature(value);
            item->setText(pvalue->prettyTemperature());
        } else {
            pvalue->setValue(value);
            item->setText(pvalue->prettyValue());
        }
    } else {
        if (item->column()==0) {
            parameter->addValue(value, 0);
        } else {
            parameter->addValue(value);
        }
    }

    valueTable_->setCurrentItem(0);
    parameter->sort();
    parameterChanged(parameter);

    ParameterSelectionModel_->emitParameterModified();
    property->recalculate();
    PropertySelectionModel_->emitPropertyModified(property);
}

void MaterialParameterView::temperatureUnitChanged(const QString& name)
{
    Parameter* parameter = ParameterSelectionModel_->getSelection();
    if (!parameter) return;

    int unitIndex = parameter->getTemperatureUnit()->getUnitIndex(name);
    if (unitIndex==-1) return;

    for (int r=0;r<parameter->getNumberOfValues();r++) {
        QTableWidgetItem * item = valueTable_->item(r, 0);
        MaterialParameterViewItem* pitem = dynamic_cast<MaterialParameterViewItem*>(item);

        ParameterValue* pvalue = pitem->getParameterValue();
        if (!pvalue->isTemperatureValid()) continue;

        double newValue = parameter->getTemperatureUnit()->convert(pvalue->getTemperature(), unitIndex);
        pvalue->setTemperature(newValue);
        item->setText(QString().setNum(newValue));
    }

    parameter->getTemperatureUnit()->setCurrentUnitIndex(unitIndex);
}

void MaterialParameterView::valueUnitChanged(const QString& name)
{
    Parameter* parameter = ParameterSelectionModel_->getSelection();
    if (!parameter) return;

    int unitIndex = parameter->getValueUnit()->getUnitIndex(name);
    if (unitIndex==-1) return;

    for (int r=0;r<parameter->getNumberOfValues();r++) {
        QTableWidgetItem * item = valueTable_->item(r, 1);
        if (!item) continue;
        MaterialParameterViewItem* pitem = dynamic_cast<MaterialParameterViewItem*>(item);

        ParameterValue* pvalue = pitem->getParameterValue();
        if (!pvalue->isValueValid()) continue;

        double newValue = parameter->getValueUnit()->convert(pvalue->getValue(), unitIndex);
        pvalue->setValue(newValue);
        item->setText(QString().setNum(newValue));
    }

    parameter->getValueUnit()->setCurrentUnitIndex(unitIndex);

    ParameterSelectionModel_->emitParameterModified();
}

void MaterialParameterView::displayContextMenu(const QPoint& point)
{
    NQLog("MaterialParameterView", NQLog::Spam) << "void displayContextMenu(const QPoint& point)";

    int row = valueTable_->currentRow();

    NQLog("MaterialParameterView", NQLog::Spam) << "row " << row << " " << valueTable_->rowCount();

    if (row+1==valueTable_->rowCount()) {
        DeleteAction_->setEnabled(false);
    } else {
        DeleteAction_->setEnabled(true);
    }

    DeleteTempAction_->setEnabled(true);

    MaterialParameterViewItem* pitem = dynamic_cast<MaterialParameterViewItem*>(valueTable_->currentItem());
    if (pitem) {
        ParameterValue* pvalue = pitem->getParameterValue();
        if (!pvalue->isTemperatureValid()) {
            DeleteTempAction_->setEnabled(false);
        }
    }

    ContextMenu_->exec(mapToGlobal(point));
}

void MaterialParameterView::import()
{
    NQLog("MaterialParameterView", NQLog::Spam) << "void import()";

    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Import Data",
                                                    QDesktopServices::storageLocation(QDesktopServices::DesktopLocation),
                                                    "*.csv");
    if (filename.isEmpty()) return;

    Parameter* parameter = ParameterSelectionModel_->getSelection();
    parameter->importValues(filename);

    Property* property = parameter->getProperty();

    ParameterSelectionModel_->emitParameterModified();
    property->recalculate();
    PropertySelectionModel_->emitPropertyModified(property);

    parameterChanged(parameter);
}

void MaterialParameterView::deleteParameterRow()
{
    NQLog("MaterialParameterView", NQLog::Spam) << "void deleteParameterRow()";

    int row = valueTable_->currentRow();
    deleteParameterRow(row);
}

void MaterialParameterView::deleteParameterRow(int row)
{
    NQLog("MaterialParameterView", NQLog::Spam) << "void deleteParameterRow(int row) " << row;

    Parameter* parameter = ParameterSelectionModel_->getSelection();
    if (!parameter) return;
    Property* property = parameter->getProperty();

    parameter->deleteValue(row);

    ParameterSelectionModel_->emitParameterModified();
    property->recalculate();
    PropertySelectionModel_->emitPropertyModified(property);

    this->parameterChanged(parameter);
}

void MaterialParameterView::deleteTemperature()
{
    NQLog("MaterialParameterView", NQLog::Spam) << "void deleteTemperature()";

    int row = valueTable_->currentRow();
    deleteTemperature(row);
}

void MaterialParameterView::deleteTemperature(int row)
{
    NQLog("MaterialParameterView", NQLog::Spam) << "void deleteTemperature(int row) " << row;

    Parameter* parameter = ParameterSelectionModel_->getSelection();
    if (!parameter) return;
    Property* property = parameter->getProperty();

    parameter->deleteTemperature(row);

    ParameterSelectionModel_->emitParameterModified();
    property->recalculate();
    PropertySelectionModel_->emitPropertyModified(property);

    this->parameterChanged(parameter);
}

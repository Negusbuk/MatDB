#include <iostream>

#include <QBoxLayout>
#include <QHeaderView>

#include "materialparameterview.h"

MaterialParameterViewItem::MaterialParameterViewItem(ParameterValue* parameter,
                                                     int column) :
    ParameterValue_(parameter),
    Column_(column)
{
    if (ParameterValue_!=0) {
        QString t;
        if (Column_==0) {
            if (parameter->isTemperatureValid()) {
                setText(ParameterValue_->prettyTemperature());
            } else {
                setText("");
            }
        } else {
            if (parameter->isValueValid()) {
                setText(ParameterValue_->prettyValue());
            } else {
                setText("");
            }
        }
    }
}

MaterialParameterView::MaterialParameterView(MaterialListModel *listmodel,
                                             MaterialSelectionModel * selectionmodel,
                                             ParameterSelectionModel* parameterselectionmodel,
                                             QWidget *parent) :
    QWidget(parent),
    ListModel_(listmodel),
    SelectionModel_(selectionmodel),
    ParameterSelectionModel_(parameterselectionmodel)
{
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
    //parameterChanged(0);
}

void MaterialParameterView::parameterChanged(Parameter* parameter)
{
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

    //std::cout << "void MaterialParameterView::parameterChanged(Parameter* parameter)" << std::endl;
    //std::cout << parameter << std::endl;
    //std::cout << parameter->getName().toStdString() << std::endl;
    //std::cout << parameter->getProperty()->getName().toStdString() << std::endl;

    //std::cout << "blah" << std::endl;
    //std::cout << parameter->getNumberOfValues() << std::endl;
    valueTable_->setRowCount(1+parameter->getNumberOfValues());
    //std::cout << "blah" << std::endl;

    int count = 0;
    for (std::vector<ParameterValue>::iterator it=parameter->getValues().begin();
         it!=parameter->getValues().end();
         ++it) {

        valueTable_->setItem(count, 0, new MaterialParameterViewItem(&(*it), 0));
        valueTable_->setItem(count, 1, new MaterialParameterViewItem(&(*it), 1));
        valueTable_->setVerticalHeaderItem(count, new QTableWidgetItem(QString().setNum(count+1)));

        count++;
    }
    //std::cout << "blah" << std::endl;

    valueTable_->setVerticalHeaderItem(count, new QTableWidgetItem("*"));
    valueTable_->setItem(count, 0, new MaterialParameterViewItem(0, 0));
    valueTable_->setItem(count, 1, new MaterialParameterViewItem(0, 1));
    //std::cout << "blah" << std::endl;

    TempUnitBox_->setUnit(parameter->getTemperatureUnit());
    ValueUnitBox_->setUnit(parameter->getValueUnit());
    //std::cout << "blah" << std::endl;
}

void MaterialParameterView::parameterValueChanged(QTableWidgetItem* item)
{
    //std::cout << "void MaterialParameterView::parameterValueChanged(QTableWidgetItem* item)" << std::endl;

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

    int pos;
    QString text = item->text();
    //std::cout << text.toStdString() << std::endl;
    double value = text.toDouble();

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

    //std::cout << "parameter value changed " << value << " " << parameter << std::endl;

    if (pvalue) {
        item->setText(QString().setNum(value));
        if (item->column()==0) {
            pvalue->setTemperature(value);
        } else {
            pvalue->setValue(value);
            //property->recalculate();
        }
    } else {
        //std::cout << "no pvalue" << std::endl;
        if (item->column()==0) {
            parameter->addValue(value, 0);
        } else {
            parameter->addValue(value);
            //std::cout << "addValue" << std::endl;
        }
        parameterChanged(parameter);
    }

    ParameterSelectionModel_->emitParameterModified();
}

void MaterialParameterView::temperatureUnitChanged(const QString& name)
{
    //std::cout << "MaterialParameterView::temperatureUnitChanged() " << name.toStdString() << std::endl;

    Parameter* parameter = ParameterSelectionModel_->getSelection();
    if (!parameter) return;

    int unitIndex = parameter->getTemperatureUnit()->getUnitIndex(name);
    if (unitIndex==-1) return;

    for (int r=0;r<parameter->getNumberOfValues();r++) {
        QTableWidgetItem * item = valueTable_->item(r, 0);
        MaterialParameterViewItem* pitem = dynamic_cast<MaterialParameterViewItem*>(item);

        ParameterValue* pvalue = pitem->getParameterValue();
        if (!pvalue->isTemperatureValid()) continue;
        //std::cout << "temperatureUnitChanged " << r << " " << pvalue->getTemperature() << std::endl;

        double newValue = parameter->getTemperatureUnit()->convert(pvalue->getTemperature(), unitIndex);
        pvalue->setTemperature(newValue);
        item->setText(QString().setNum(newValue));

        //std::cout << newValue << std::endl;
    }

    parameter->getTemperatureUnit()->setCurrentUnitIndex(unitIndex);
}

void MaterialParameterView::valueUnitChanged(const QString& name)
{
    //std::cout << "MaterialParameterView::valueUnitChanged() " << name.toStdString() << std::endl;

    Parameter* parameter = ParameterSelectionModel_->getSelection();
    if (!parameter) return;

    int unitIndex = parameter->getValueUnit()->getUnitIndex(name);
    if (unitIndex==-1) return;

    for (int r=0;r<parameter->getNumberOfValues();r++) {
        QTableWidgetItem * item = valueTable_->item(r, 1);
        if (!item) continue;
        MaterialParameterViewItem* pitem = dynamic_cast<MaterialParameterViewItem*>(item);

        ParameterValue* pvalue = pitem->getParameterValue();
        //std::cout << "valueUnitChanged " << r << " " << pvalue->getValue() << std::endl;
        if (!pvalue->isValueValid()) continue;

        double newValue = parameter->getValueUnit()->convert(pvalue->getValue(), unitIndex);
        pvalue->setValue(newValue);
        item->setText(QString().setNum(newValue));

        //std::cout << newValue << std::endl;
    }

    parameter->getValueUnit()->setCurrentUnitIndex(unitIndex);

    ParameterSelectionModel_->emitParameterModified();
}

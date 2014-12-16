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

#include <QToolButton>
#include <QMenuBar>
#include <QSettings>
#include <QDir>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif
#include <QFileDialog>
#include <QHeaderView>
#include <QLayout>

#include <nqlogger.h>

#include "materialxmlexportdialog.h"
#include "materialselectiondialog.h"
#include "materialimportdialog.h"

#include "matmlreader.h"
#include "matmlwriter.h"
#include "htmlwriter.h"

#include "matdbaboutdialog.h"
#include "matdbpreferencedialog.h"

#include "materialcategorywidget.h"
#include <materialfilterwidget.h>

#include "matdbmainwindow.h"

MatDBMainWindow::MatDBMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QMenuBar *menubar = new QMenuBar(0);
    setMenuBar(menubar);
    QMenu * menu = menuBar()->addMenu(tr("&File"));
    menu->addAction(tr("about.*"), this, SLOT(aboutDialog()));
    menu->addAction(tr("preferences"), this, SLOT(preferenceDialog()));

    MaterialCategoryModel_ = new MaterialCategoryModel(this);
    ParameterModel_ = new ParameterModel(this);
    PropertyModel_ = new PropertyModel(ParameterModel_, this);

    MaterialListModel_ = new MaterialListModel(MaterialCategoryModel_, this);
    MaterialSelectionModel_ = new MaterialSelectionModel(this);

    connect(MaterialSelectionModel_, SIGNAL(duplicateMaterial(Material*)),
            MaterialListModel_, SLOT(duplicateMaterial(Material*)));
    connect(MaterialSelectionModel_, SIGNAL(deleteMaterial(Material*)),
            MaterialListModel_, SLOT(deleteMaterial(Material*)));

    PropertySelectionModel_ = new PropertySelectionModel(this);
    ParameterSelectionModel_ = new ParameterSelectionModel(this);

    ToolBar_ = addToolBar("ToolBar");
    importXMLAction_ = ToolBar_->addAction(QIcon(":/icons/MatDBImportXML.png"),
                                           tr("Import XML"),
                                           this,
                                           SLOT(importMaterials()));
    ToolBar_->addSeparator();
    addIsotropicMatAction_ = ToolBar_->addAction(QIcon(":/icons/MatDBAddIsotropicMaterial.png"),
                                                 tr("Add Isotropic Material"),
                                                 this,
                                                 SLOT(addDefaultIsotropicMaterial()));
    addOrthotropicMatAction_ = ToolBar_->addAction(QIcon(":/icons/MatDBAddOrthotropicMaterial.png"),
                                                   tr("Add Orthotropic Material"),
                                                   this,
                                                   SLOT(addDefaultOrthotropicMaterial()));
    addLiquidMatAction_ = ToolBar_->addAction(QIcon(":/icons/MatDBAddLiquidMaterial.png"),
                                              tr("Add Liquid Material"),
                                              this,
                                              SLOT(addDefaultLiquidMaterial()));
    addGaseousMatAction_ = ToolBar_->addAction(QIcon(":/icons/MatDBAddGaseousMaterial.png"),
                                               tr("Add Gaseous Material"),
                                               this,
                                               SLOT(addDefaultGaseousMaterial()));
    QWidget* stretch = new QWidget(ToolBar_);
    stretch->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ToolBar_->addWidget(stretch);
    togglePropertyToolBoxDockWidgetAction_ = ToolBar_->addAction(QIcon(":/icons/MatDBPropertyToolBox.png"),
                                                                 tr("Hide Toolbox"),
                                                                 this,
                                                                 SLOT(togglePropertyToolBoxDockWidget()));
    togglePropertyToolBoxDockWidgetAction_->setCheckable(true);
    togglePropertyToolBoxDockWidgetAction_->setChecked(true);
    toggleCategoryDockWidgetAction_ = ToolBar_->addAction(QIcon(":/icons/MatDBCategories.png"),
                                                          tr("Hide Categories"),
                                                          this,
                                                          SLOT(toggleCategoryDockWidget()));
    toggleCategoryDockWidgetAction_->setCheckable(true);
    toggleCategoryDockWidgetAction_->setChecked(true);
    ToolBar_->addSeparator();
    exportXMLAction_ = ToolBar_->addAction(QIcon(":/icons/MatDBExportXML.png"),
                                           tr("Export XML"),
                                           this,
                                           SLOT(exportMaterialsXML()));
    exportHTMLAction_ = ToolBar_->addAction(QIcon(":/icons/MatDBExportHTML.png"),
                                            tr("Export HTML"),
                                            this,
                                            SLOT(exportMaterialsHTML()));
    ToolBar_->setFloatable(false);
    ToolBar_->setMovable(false);
    ToolBar_->setToolButtonStyle(Qt::ToolButtonIconOnly);

    QWidget* central = new QWidget(this);
    QVBoxLayout* clayout = new QVBoxLayout(central);
    clayout->setContentsMargins(0, 0, 0, 0);
    clayout->setSpacing(0);
    central->setLayout(clayout);

    MaterialFilterWidget* filterWidget = new MaterialFilterWidget(MaterialListModel_,
                                                                  central);
    clayout->addWidget(filterWidget);

    MaterialTableView_ = new MaterialTableView(MaterialListModel_,
                                               MaterialSelectionModel_,
                                               PropertyModel_,
                                               ParameterModel_,
                                               central);
    MaterialTableView_->horizontalHeader()->hide();
    clayout->addWidget(MaterialTableView_);
    setCentralWidget(central);

    propertyToolBoxDockWidget_ = new QDockWidget(tr("Property Collection"), this);
    propertyToolBoxDockWidget_->setAllowedAreas(Qt::LeftDockWidgetArea);
    propertyToolBoxDockWidget_->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetVerticalTitleBar);
    PropertyToolBox_ = new PropertyToolBox(PropertyModel_,
                                           propertyToolBoxDockWidget_);
    propertyToolBoxDockWidget_->setWidget(PropertyToolBox_);
    addDockWidget(Qt::LeftDockWidgetArea, propertyToolBoxDockWidget_);

    categoryDockWidget_ = new QDockWidget(tr("Categories"), this);
    categoryDockWidget_->setAllowedAreas(Qt::LeftDockWidgetArea);
    categoryDockWidget_->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetVerticalTitleBar);
    MaterialCategoryWidget * categoryWidget = new MaterialCategoryWidget(MaterialCategoryModel_,
                                                                         categoryDockWidget_);
    categoryDockWidget_->setWidget(categoryWidget);
    addDockWidget(Qt::LeftDockWidgetArea, categoryDockWidget_);

    propertiesDockWidget_ = new QDockWidget(tr("Properties"), this);
    propertiesDockWidget_->setAllowedAreas(Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
    propertiesDockWidget_->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetVerticalTitleBar);
    MaterialPropertyView_ = new MaterialPropertyView(MaterialListModel_,
                                                     MaterialSelectionModel_,
                                                     PropertyModel_,
                                                     ParameterModel_,
                                                     PropertySelectionModel_,
                                                     ParameterSelectionModel_,
                                                     propertiesDockWidget_);
    propertiesDockWidget_->setWidget(MaterialPropertyView_);
    addDockWidget(Qt::RightDockWidgetArea, propertiesDockWidget_);

    parameterDockWidget_ = new QDockWidget(tr("Parameter"), this);
    parameterDockWidget_->setAllowedAreas(Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    parameterDockWidget_->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetVerticalTitleBar);
    ParameterStackView_ = new ParameterStackView(MaterialListModel_,
                                                 MaterialSelectionModel_,
                                                 PropertySelectionModel_,
                                                 ParameterSelectionModel_,
                                                 PropertyModel_,
                                                 MaterialCategoryModel_,
                                                 parameterDockWidget_);
    parameterDockWidget_->setWidget(ParameterStackView_);
    addDockWidget(Qt::RightDockWidgetArea, parameterDockWidget_);

    QSettings settings;
    QString dbPath = settings.value("dbpath").toString();
    QDir dbDir(dbPath);

    QFile fileCat(dbDir.absoluteFilePath("Categories.xml"));
    if (fileCat.open(QIODevice::ReadOnly)) {
        MaterialCategoryModel_->read(&fileCat);
        fileCat.close();
    }

    MaterialListModel_->read(dbDir, PropertyModel_);
    MaterialListModel_->setModified(false);
    if (MaterialListModel_->getMaterialCount()==0) makeDefaultMaterials();

    autoSaveTimer_ = new QTimer(this);
    connect(autoSaveTimer_, SIGNAL(timeout()),
            this, SLOT(saveData()));
    autoSaveTimer_->start(1000*30);

    updateGeometry();
}

MatDBMainWindow::~MatDBMainWindow()
{

}

void MatDBMainWindow::addDefaultIsotropicMaterial()
{
    Material* mat = Material::makeDefaultIsotropicMaterial(PropertyModel_);
    mat->setModified();
    MaterialListModel_->addMaterial(mat);
}

void MatDBMainWindow::addDefaultOrthotropicMaterial()
{
    Material* mat = Material::makeDefaultOrthotropicMaterial(PropertyModel_);
    mat->setModified();
    MaterialListModel_->addMaterial(mat);
}

void MatDBMainWindow::addDefaultLiquidMaterial()
{
    Material* mat = Material::makeDefaultLiquidMaterial(PropertyModel_);
    mat->setModified();
    MaterialListModel_->addMaterial(mat);
}

void MatDBMainWindow::addDefaultGaseousMaterial()
{
    Material* mat = Material::makeDefaultGaseousMaterial(PropertyModel_);
    mat->setModified();
    MaterialListModel_->addMaterial(mat);
}

void MatDBMainWindow::makeDefaultMaterials()
{
    addDefaultIsotropicMaterial();
    addDefaultOrthotropicMaterial();
    addDefaultLiquidMaterial();
    addDefaultGaseousMaterial();
}

void MatDBMainWindow::exportMaterialsXML()
{
    NQLog("MatDBMainWindow", NQLog::Spam) << "void exportMaterialsXML()";

    MaterialXMLExportDialog dialog(MaterialListModel_, this);

    int result = dialog.exec();
    if (result==0) return;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Export Materials"),
                                                    QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation),
                                                    "*.xml");
#else
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Export Materials"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    "*.xml");
#endif
    if (filename.isEmpty() || !filename.endsWith(".xml")) return;

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        const std::vector<Material*>& selection = dialog.getSelectedMaterials();
        MATMLWriter writer(selection,
                           PropertyModel_,
                           ParameterModel_,
                           this);
        writer.write(&file, static_cast<MATMLWriter::ExportMode>(dialog.getExportMode()));
        file.close();
    }
}


void MatDBMainWindow::exportMaterialsHTML()
{
    NQLog("MatDBMainWindow", NQLog::Spam) << "void exportMaterialsHTML()";

    /*
    QDir destination("/Users/mussgill/Desktop/MatDB");
    const std::vector<Material*>& selection = MaterialListModel_->getMaterials();
    HTMLWriter writer(selection,
                      PropertyModel_,
                      ParameterModel_,
                      MaterialCategoryModel_,
                      this);
    writer.write(destination);
    */

    MaterialSelectionDialog dialog("Export", MaterialListModel_, this);

    int result = dialog.exec();
    if (result==0) return;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QString dirname = QFileDialog::getExistingDirectory(this,
                                                    tr("Export Materials"),
                                                    QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation),
                                                    QFileDialog::ShowDirsOnly);
#else
    QString dirname = QFileDialog::getExistingDirectory(this,
                                                    tr("Export Materials"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    QFileDialog::ShowDirsOnly);
#endif
    if (dirname.isEmpty()) return;

    QDir destination(dirname);

    const std::vector<Material*>& selection = dialog.getSelectedMaterials();
    HTMLWriter writer(selection,
                      PropertyModel_,
                      ParameterModel_,
                      MaterialCategoryModel_,
                      this);
    writer.write(destination);
}

void MatDBMainWindow::importMaterials()
{
    NQLog("MatDBMainWindow", NQLog::Spam) << "void importMaterials()";

    QString formats;
    formats += "All supported formats (*.xml);;";
    formats += "MatML files (*.xml);;";
    formats += "ANSYS Engineering Data files (*.xml)";

    QString selectedFormat;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Import Materials"),
                                                    QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation),
                                                    formats,
                                                    &selectedFormat);
#else
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Import Materials"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    formats,
                                                    &selectedFormat);
#endif
    if (filename.isEmpty()) return;

    MaterialListModel listModel(MaterialCategoryModel_);

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {

        if (filename.endsWith(".xml")) {
            MATMLReader reader(&listModel,
                               PropertyModel_,
                               ParameterModel_,
                               MaterialCategoryModel_,
                               this);
            reader.read(&file);
        }

        file.close();
    }

    if (listModel.getMaterialCount()==0) return;

    MaterialImportDialog dialog(&listModel, this);

    int result = dialog.exec();
    if (result==0) return;

    MaterialListModel_->addMaterials(dialog.getSelectedMaterials());
}

void MatDBMainWindow::saveData()
{
    NQLog("MatDBMainWindow", NQLog::Message) << "saving database";

    QSettings settings;
    QString dbPath = settings.value("dbpath").toString();
    QDir dbDir(dbPath);
    if (!dbDir.exists()) {
        dbDir.mkpath(".");
    }

    MaterialListModel_->write(dbDir);

    //if (MaterialCategoryModel_->isModified()) {
        QFile ofileCat(dbDir.absoluteFilePath("Categories.xml"));
        if (ofileCat.open(QIODevice::WriteOnly)) {
            MaterialCategoryModel_->write(&ofileCat);
            ofileCat.close();
        }
    //}
}

void MatDBMainWindow::closeEvent(QCloseEvent * /* event */)
{
    NQLog("MatDBMainWindow", NQLog::Spam) << "void closeEvent(QCloseEvent *event)";

    saveData();
}

void MatDBMainWindow::aboutDialog()
{
    NQLog("MatDBMainWindow", NQLog::Spam) << "void aboutDialog()";

    MatDBAboutDialog dialog;
    dialog.exec();
}

void MatDBMainWindow::preferenceDialog()
{
    NQLog("MatDBMainWindow", NQLog::Spam) << "void preferenceDialog()";

    MatDBPreferenceDialog dialog;
    dialog.exec();
}

void MatDBMainWindow::togglePropertyToolBoxDockWidget()
{
    if (togglePropertyToolBoxDockWidgetAction_->isChecked()) {
        propertyToolBoxDockWidget_->show();
        togglePropertyToolBoxDockWidgetAction_->setText(tr("Hide Toolbox"));
    } else {
        propertyToolBoxDockWidget_->hide();
        togglePropertyToolBoxDockWidgetAction_->setText(tr("Show Toolbox"));
    }
}

void MatDBMainWindow::toggleCategoryDockWidget()
{
    if (toggleCategoryDockWidgetAction_->isChecked()) {
        categoryDockWidget_->show();
        toggleCategoryDockWidgetAction_->setText(tr("Hide Categories"));
    } else {
        categoryDockWidget_->hide();
        toggleCategoryDockWidgetAction_->setText(tr("Show Categories"));
    }
}

void MatDBMainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {

        propertyToolBoxDockWidget_->setWindowTitle(tr("Property Collection"));
        categoryDockWidget_->setWindowTitle(tr("Categories"));
        propertiesDockWidget_->setWindowTitle(tr("Properties"));
        parameterDockWidget_->setWindowTitle(tr("Parameter"));

        importXMLAction_->setText(tr("Import XML"));
        addIsotropicMatAction_->setText(tr("Add Isotropic Material"));
        addOrthotropicMatAction_->setText(tr("Add Orthotropic Material"));
        addLiquidMatAction_->setText(tr("Add Liquid Material"));
        addGaseousMatAction_->setText(tr("Add Gaseous Material"));

        if (togglePropertyToolBoxDockWidgetAction_->isChecked()) {
            togglePropertyToolBoxDockWidgetAction_->setText(tr("Hide Toolbox"));
        } else {
            togglePropertyToolBoxDockWidgetAction_->setText(tr("Show Toolbox"));
        }

        if (toggleCategoryDockWidgetAction_->isChecked()) {
            toggleCategoryDockWidgetAction_->setText(tr("Hide Categories"));
        } else {
            toggleCategoryDockWidgetAction_->setText(tr("Show Categories"));
        }

        exportXMLAction_->setText(tr("Export XML"));
        exportHTMLAction_->setText(tr("Export HTML"));

        MaterialCategoryModel_->changeEvent(event);

    } else {
        QWidget::changeEvent(event);
    }
}

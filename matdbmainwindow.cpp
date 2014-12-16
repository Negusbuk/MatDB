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
#include <QSettings>

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
    QMenu * menu = menuBar()->addMenu("MatDB");
    menu->addAction(tr("about.*"), this, SLOT(aboutDialog()));
    menu->addAction(tr("preferences"), this, SLOT(preferenceDialog()));

    menu = menuBar()->addMenu(tr("File"));
    importXMLMenuAction_ = menu->addAction(tr("Import XML File"), this, SLOT(importMaterials()));
    menu->addSeparator();
    addIsotropicMatMenuAction_ = menu->addAction(tr("Add Isotropic Material"), this, SLOT(addDefaultIsotropicMaterial()));
    addOrthotropicMatAction_ = menu->addAction(tr("Add Orthotropic Material"), this, SLOT(addDefaultOrthotropicMaterial()));
    addLiquidMatMenuAction_ = menu->addAction(tr("Add Liquid Material"), this, SLOT(addDefaultLiquidMaterial()));
    addGaseousMatMenuAction_ = menu->addAction(tr("Add Gaseous Material"), this, SLOT(addDefaultGaseousMaterial()));
    menu->addSeparator();
    exportXMLMenuAction_ = menu->addAction(tr("Export XML"), this, SLOT(exportMaterialsXML()));
    exportHTMLMenuAction_ = menu->addAction(tr("Export HTML"), this, SLOT(exportMaterialsHTML()));

    menu = menuBar()->addMenu(tr("Window"));
    togglePropertyToolBoxDockWidgetMenuAction_ = menu->addAction(tr("Toolbox"),
                                                                 this,
                                                                 SLOT(togglePropertyToolBoxDockWidgetMenu()));
    togglePropertyToolBoxDockWidgetMenuAction_->setCheckable(true);
    togglePropertyToolBoxDockWidgetMenuAction_->setChecked(true);
    toggleCategoryDockWidgetMenuAction_ = menu->addAction(tr("Categories"),
                                                          this,
                                                          SLOT(toggleCategoryDockWidgetMenu()));
    toggleCategoryDockWidgetMenuAction_->setCheckable(true);
    toggleCategoryDockWidgetMenuAction_->setChecked(true);
    menu->addSeparator();
    toggleFullScreenAction_ = menu->addAction(tr("Enter Full Screen"),
                                              this,
                                              SLOT(toggleFullScreen()));

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
                                           tr("Import XML File"),
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
    togglePropertyToolBoxDockWidgetButtonAction_ = ToolBar_->addAction(QIcon(":/icons/MatDBPropertyToolBox.png"),
                                                                       tr("Hide Toolbox"),
                                                                       this,
                                                                       SLOT(togglePropertyToolBoxDockWidgetButton()));
    togglePropertyToolBoxDockWidgetButtonAction_->setCheckable(true);
    togglePropertyToolBoxDockWidgetButtonAction_->setChecked(true);
    toggleCategoryDockWidgetButtonAction_ = ToolBar_->addAction(QIcon(":/icons/MatDBCategories.png"),
                                                                tr("Hide Categories"),
                                                                this,
                                                                SLOT(toggleCategoryDockWidgetButton()));
    toggleCategoryDockWidgetButtonAction_->setCheckable(true);
    toggleCategoryDockWidgetButtonAction_->setChecked(true);
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

    restoreGeometry(settings.value("geometry").toByteArray());

    updateGeometry();

    QSize size = settings.value("toolbox/size", propertyToolBoxDockWidget_->minimumSize()).toSize();
    propertyToolBoxDockWidget_->resize(size);
    bool toolboxHidden = settings.value("toolbox/hidden", false).toBool();
    if (toolboxHidden) {
        propertyToolBoxDockWidget_->hide();
        togglePropertyToolBoxDockWidgetButtonAction_->setText(tr("Show Toolbox"));
        togglePropertyToolBoxDockWidgetButtonAction_->setChecked(false);
        togglePropertyToolBoxDockWidgetMenuAction_->setChecked(false);
    }

    size = settings.value("categories/size", categoryDockWidget_->minimumSize()).toSize();
    categoryDockWidget_->resize(size);
    bool categoriesHidden = settings.value("categories/hidden", false).toBool();
    if (categoriesHidden) {
        categoryDockWidget_->hide();
        toggleCategoryDockWidgetButtonAction_->setText(tr("Show Categories"));
        toggleCategoryDockWidgetButtonAction_->setChecked(false);
        toggleCategoryDockWidgetMenuAction_->setChecked(false);
    }

    if (settings.value("fullscreen", false).toBool()) {
        QTimer::singleShot(500, this, SLOT(showFullScreen()));
    }
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

    QSettings settings;
    settings.setValue("toolbox/hidden", (bool)propertyToolBoxDockWidget_->isHidden());
    settings.setValue("toolbox/size", propertyToolBoxDockWidget_->size());
    settings.setValue("categories/hidden", (bool)categoryDockWidget_->isHidden());
    settings.setValue("categories/size", categoryDockWidget_->size());
    settings.setValue("fullscreen", (bool)isFullScreen());
    if (!isFullScreen()) settings.setValue("geometry", saveGeometry());

    NQLog("MatDBMainWindow", NQLog::Spam) << settings.fileName();
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

void MatDBMainWindow::togglePropertyToolBoxDockWidgetButton()
{
    if (togglePropertyToolBoxDockWidgetButtonAction_->isChecked()) {
        propertyToolBoxDockWidget_->show();
        togglePropertyToolBoxDockWidgetButtonAction_->setText(tr("Hide Toolbox"));
        togglePropertyToolBoxDockWidgetMenuAction_->setChecked(true);
    } else {
        propertyToolBoxDockWidget_->hide();
        togglePropertyToolBoxDockWidgetButtonAction_->setText(tr("Show Toolbox"));
        togglePropertyToolBoxDockWidgetMenuAction_->setChecked(false);
    }
}

void MatDBMainWindow::togglePropertyToolBoxDockWidgetMenu()
{
    if (togglePropertyToolBoxDockWidgetMenuAction_->isChecked()) {
        propertyToolBoxDockWidget_->show();
        togglePropertyToolBoxDockWidgetButtonAction_->setText(tr("Hide Toolbox"));
        togglePropertyToolBoxDockWidgetButtonAction_->setChecked(true);
    } else {
        propertyToolBoxDockWidget_->hide();
        togglePropertyToolBoxDockWidgetButtonAction_->setText(tr("Show Toolbox"));
        togglePropertyToolBoxDockWidgetButtonAction_->setChecked(false);
    }
}

void MatDBMainWindow::toggleCategoryDockWidgetButton()
{
    if (toggleCategoryDockWidgetButtonAction_->isChecked()) {
        categoryDockWidget_->show();
        toggleCategoryDockWidgetButtonAction_->setText(tr("Hide Categories"));
        toggleCategoryDockWidgetMenuAction_->setChecked(true);
    } else {
        categoryDockWidget_->hide();
        toggleCategoryDockWidgetButtonAction_->setText(tr("Show Categories"));
        toggleCategoryDockWidgetMenuAction_->setChecked(false);
    }
}

void MatDBMainWindow::toggleCategoryDockWidgetMenu()
{
    if (toggleCategoryDockWidgetMenuAction_->isChecked()) {
        categoryDockWidget_->show();
        toggleCategoryDockWidgetButtonAction_->setText(tr("Hide Categories"));
        toggleCategoryDockWidgetButtonAction_->setChecked(true);
    } else {
        categoryDockWidget_->hide();
        toggleCategoryDockWidgetButtonAction_->setText(tr("Show Categories"));
        toggleCategoryDockWidgetButtonAction_->setChecked(false);
    }
}

void MatDBMainWindow::toggleFullScreen()
{
    if (isFullScreen()) {
        showNormal();
        toggleFullScreenAction_->setText(tr("Enter Full Screen"));
    } else {
        showFullScreen();
        toggleFullScreenAction_->setText(tr("Exit Full Screen"));
    }
}

void MatDBMainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {

        propertyToolBoxDockWidget_->setWindowTitle(tr("Property Collection"));
        categoryDockWidget_->setWindowTitle(tr("Categories"));
        propertiesDockWidget_->setWindowTitle(tr("Properties"));
        parameterDockWidget_->setWindowTitle(tr("Parameter"));

        importXMLMenuAction_->setText(tr("Import XML"));
        addIsotropicMatMenuAction_->setText(tr("Add Isotropic Material"));
        addOrthotropicMatMenuAction_->setText(tr("Add Orthotropic Material"));
        addLiquidMatMenuAction_->setText(tr("Add Liquid Material"));
        addGaseousMatMenuAction_->setText(tr("Add Gaseous Material"));

        importXMLAction_->setText(tr("Import XML"));
        addIsotropicMatAction_->setText(tr("Add Isotropic Material"));
        addOrthotropicMatAction_->setText(tr("Add Orthotropic Material"));
        addLiquidMatAction_->setText(tr("Add Liquid Material"));
        addGaseousMatAction_->setText(tr("Add Gaseous Material"));

        if (togglePropertyToolBoxDockWidgetButtonAction_->isChecked()) {
            togglePropertyToolBoxDockWidgetButtonAction_->setText(tr("Hide Toolbox"));
        } else {
            togglePropertyToolBoxDockWidgetButtonAction_->setText(tr("Show Toolbox"));
        }
        togglePropertyToolBoxDockWidgetMenuAction_->setText(tr("Toolbox"));

        if (toggleCategoryDockWidgetButtonAction_->isChecked()) {
            toggleCategoryDockWidgetButtonAction_->setText(tr("Hide Categories"));
        } else {
            toggleCategoryDockWidgetButtonAction_->setText(tr("Show Categories"));
        }
        toggleCategoryDockWidgetMenuAction_->setText(tr("Categories"));

        exportXMLMenuAction_->setText(tr("Export XML"));
        exportHTMLMenuAction_->setText(tr("Export HTML"));

        exportXMLAction_->setText(tr("Export XML"));
        exportHTMLAction_->setText(tr("Export HTML"));

        checkWindowState();

        MaterialCategoryModel_->changeEvent(event);

    } else if (event->type() == QEvent::WindowStateChange) {

        QTimer::singleShot(250, this, SLOT(checkWindowState()));

    } else {
        QWidget::changeEvent(event);
    }
}

void MatDBMainWindow::checkWindowState()
{
    if (!isFullScreen()) {
        toggleFullScreenAction_->setText(tr("Enter Full Screen"));
    } else {
        toggleFullScreenAction_->setText(tr("Exit Full Screen"));
    }
}

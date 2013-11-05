#include <iostream>

#include <QToolButton>
#include <QMenuBar>
#include <QSettings>
#include <QDir>
#include <QDesktopServices>
#include <QFileDialog>
#include <QDockWidget>
#include <QHeaderView>

#include "materialxmlexportdialog.h"
#include "materialimportdialog.h"

#include "matmlreader.h"
#include "matmlwriter.h"

#include "matdbaboutdialog.h"
#include "materialcategorydialog.h"
#include "matdbmainwindow.h"

MatDBMainWindow::MatDBMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QMenuBar *menubar = new QMenuBar(0);
    setMenuBar(menubar);
    QMenu * menu = menuBar()->addMenu("&File");
    menu->addAction("about.*", this, SLOT(aboutDialog()));
    menu->addAction("config", this, SLOT(preferenceDialog()));

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
    ToolBar_->addAction(QIcon(":/icons/MatDBImportXML.png"), "Import XML", this, SLOT(importMaterials()));
    QWidget* stretch = new QWidget(ToolBar_);
    stretch->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ToolBar_->addWidget(stretch);
    ToolBar_->addAction(QIcon(":/icons/MatDBEditCategories.png"), "Edit Categories", this, SLOT(editCategories()));
    ToolBar_->addSeparator();
    ToolBar_->addAction(QIcon(":/icons/MatDBExportXML.png"), "Export XML", this, SLOT(exportMaterialsXML()));
    ToolBar_->addAction(QIcon(":/icons/MatDBExportHTML.png"), "Export HTML", this, SLOT(exportMaterialsHTML()));
    ToolBar_->setFloatable(false);
    ToolBar_->setMovable(false);
    ToolBar_->setToolButtonStyle(Qt::ToolButtonIconOnly);

    QDockWidget *dock;
    //QWidget * central = new QWidget(this);
    //setCentralWidget(central);

    dock = new QDockWidget("Materials", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetVerticalTitleBar);
    MaterialTableView_ = new MaterialTableView(MaterialListModel_,
                                               MaterialSelectionModel_,
                                               PropertyModel_,
                                               ParameterModel_,
                                               dock);
    MaterialTableView_->horizontalHeader()->hide();
    dock->setWidget(MaterialTableView_);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock= new QDockWidget(tr("Property Collection"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetVerticalTitleBar);
    PropertyToolBox_ = new PropertyToolBox(PropertyModel_,
                                           dock);
    dock->setWidget(PropertyToolBox_);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock= new QDockWidget(tr("Properties"), this);
    dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetVerticalTitleBar);
    MaterialPropertyView_ = new MaterialPropertyView(MaterialListModel_,
                                                     MaterialSelectionModel_,
                                                     PropertyModel_,
                                                     ParameterModel_,
                                                     PropertySelectionModel_,
                                                     ParameterSelectionModel_,
                                                     dock);
    dock->setWidget(MaterialPropertyView_);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    dock= new QDockWidget(tr("Parameter"), this);
    dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetVerticalTitleBar);
    ParameterStackView_ = new ParameterStackView(MaterialListModel_,
                                                 MaterialSelectionModel_,
                                                 PropertySelectionModel_,
                                                 ParameterSelectionModel_,
                                                 PropertyModel_,
                                                 MaterialCategoryModel_,
                                                 dock);
    dock->setWidget(ParameterStackView_);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    QSettings settings;
    QString dbPath = settings.value("dbpath").toString();
    QDir dbDir(dbPath);

    QFile fileCat(dbDir.absoluteFilePath("Categories.xml"));
    if (fileCat.open(QIODevice::ReadOnly)) {
        MaterialCategoryModel_->read(&fileCat);
        fileCat.close();
    }

    QFile file(dbDir.absoluteFilePath("Materials.xml"));
    //QFile file("/Users/mussgill/Desktop/Transfer/MaterialsX.xml");
    if (file.open(QIODevice::ReadOnly)) {
        MATMLReader reader(MaterialListModel_,
                           PropertyModel_,
                           ParameterModel_,
                           MaterialCategoryModel_,
                           this);
        reader.read(&file);
        file.close();
    } else {
        makeDefaultMaterial();
    }

    categoryDialog_ = 0;

    updateGeometry();
}

MatDBMainWindow::~MatDBMainWindow()
{

}

void MatDBMainWindow::makeDefaultMaterial()
{
    Material * mat;
    Property * prop;

    mat = new Material();
    mat->setName("material 1");
    prop = PropertyModel_->getProperty("Density");
    mat->addProperty(prop);
    prop = PropertyModel_->getProperty("Orthotropic Thermal Conductivity");
    mat->addProperty(prop);
    prop = PropertyModel_->getProperty("Isotropic Coefficient of Thermal Expansion");
    mat->addProperty(prop);
    prop = PropertyModel_->getProperty("Isotropic Elasticity");
    mat->addProperty(prop);
    MaterialListModel_->addMaterial(mat);

    prop = mat->getProperty("Density");
    prop->getParameter("Density")->addValue(ParameterValue(20.85, 23));
    prop->getParameter("Density")->addValue(ParameterValue(28.85, 25));

    prop = mat->getProperty("Isotropic Elasticity");
    prop->getParameter("Young's Modulus")->addValue(ParameterValue(20.85, 900));
    prop->getParameter("Shear Modulus")->addValue(ParameterValue(20.85, 80));
}

void MatDBMainWindow::exportMaterialsXML()
{
    std::cout << "void MatDBMainWindow::exportMaterialsXML()" << std::endl;

    MaterialXMLExportDialog dialog(MaterialListModel_, this);

    int result = dialog.exec();
    if (result==0) return;

    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Export Materials",
                                                    QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation),
                                                    "*.xml");
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
    std::cout << "void MatDBMainWindow::exportMaterialsHTML()" << std::endl;

    MaterialXMLExportDialog dialog(MaterialListModel_, this);

    int result = dialog.exec();
    if (result==0) return;

    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Export Materials",
                                                    QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation),
                                                    "*.xml");
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

void MatDBMainWindow::importMaterials()
{
    std::cout << "void MatDBMainWindow::importMaterials()" << std::endl;

    QString formats;
    formats += "All supported formats (*.xml);;";
    formats += "MatML files (*.xml);;";
    formats += "ANSYS Engineering Data files (*.xml)";

    QString selectedFormat;

    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Import Materials",
                                                    QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation),
                                                    formats,
                                                    &selectedFormat);
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

void MatDBMainWindow::closeEvent(QCloseEvent *event)
{
    std::cout << "void MatDBMainWindow::closeEvent(QCloseEvent *event)" << std::endl;

    if (categoryDialog_) {
        categoryDialog_->hide();
        delete categoryDialog_;
    }

    QSettings settings;
    QString dbPath = settings.value("dbpath").toString();
    QDir dbDir(dbPath);
    if (!dbDir.exists()) {
        dbDir.mkpath(".");
    }

    QFile ofile(dbDir.absoluteFilePath("Materials.xml"));
    if (ofile.open(QIODevice::WriteOnly)) {
        MATMLWriter writer(MaterialListModel_->getMaterials(),
                           PropertyModel_,
                           ParameterModel_,
                           this);
        writer.write(&ofile, MATMLWriter::ANSYS);
        ofile.close();
    }

    /*
    QFile ofile1(dbDir.absoluteFilePath("Materials2_MatML.xml"));
    if (ofile1.open(QIODevice::WriteOnly)) {
        MATMLWriter writer(MaterialListModel_->getMaterials(),
                           PropertyModel_,
                           ParameterModel_,
                           this);
        writer.write(&ofile1, MATMLWriter::MatML);
        ofile1.close();
    }

    QFile ofile2(dbDir.absoluteFilePath("Materials2_ANSYS.xml"));
    if (ofile2.open(QIODevice::WriteOnly)) {
        MATMLWriter writer(MaterialListModel_->getMaterials(),
                           PropertyModel_,
                           ParameterModel_,
                           this);
        writer.write(&ofile2, MATMLWriter::ANSYS);
        ofile2.close();
    }
    */

    QFile ofileCat(dbDir.absoluteFilePath("Categories.xml"));
    if (ofileCat.open(QIODevice::WriteOnly)) {
        MaterialCategoryModel_->write(&ofileCat);
        ofileCat.close();
    }
}

void MatDBMainWindow::aboutDialog()
{
    std::cout << "about" << std::endl;
    MatDBAboutDialog dialog;
         dialog.exec();
}

void MatDBMainWindow::editCategories()
{
    if (categoryDialog_==0) {
        categoryDialog_ = new MaterialCategoryDialog(MaterialCategoryModel_);
    }

    if (categoryDialog_->isHidden()) {
        categoryDialog_->applyGeometry();
        categoryDialog_->show();
    } else {
        categoryDialog_->storeGeometry();
        categoryDialog_->hide();
    }
}

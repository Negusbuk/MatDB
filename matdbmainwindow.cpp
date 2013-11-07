#include <QToolButton>
#include <QMenuBar>
#include <QSettings>
#include <QDir>
#include <QDesktopServices>
#include <QFileDialog>
#include <QHeaderView>
#include <QLayout>

#include <nqlogger.h>

#include "materialxmlexportdialog.h"
#include "materialimportdialog.h"

#include "matmlreader.h"
#include "matmlwriter.h"

#include "matdbaboutdialog.h"
#include "materialcategorywidget.h"
#include <materialfilterwidget.h>

#include "matdbmainwindow.h"

MatDBMainWindow::MatDBMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QMenuBar *menubar = new QMenuBar(0);
    setMenuBar(menubar);
    QMenu * menu = menuBar()->addMenu("&File");
    menu->addAction("about.*", this, SLOT(aboutDialog()));
    //menu->addAction("config", this, SLOT(preferenceDialog()));

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
    togglePropertyToolBoxDockWidgetAction_ = ToolBar_->addAction(QIcon(":/icons/MatDBEditCategories.png"),
                                                                 "Hide Toolbox",
                                                                 this,
                                                                 SLOT(togglePropertyToolBoxDockWidget()));
    togglePropertyToolBoxDockWidgetAction_->setCheckable(true);
    togglePropertyToolBoxDockWidgetAction_->setChecked(true);
    toggleCategoryDockWidgetAction_ = ToolBar_->addAction(QIcon(":/icons/MatDBEditCategories.png"),
                                                          "Hide Categories",
                                                          this,
                                                          SLOT(toggleCategoryDockWidget()));
    toggleCategoryDockWidgetAction_->setCheckable(true);
    toggleCategoryDockWidgetAction_->setChecked(true);
    ToolBar_->addSeparator();
    ToolBar_->addAction(QIcon(":/icons/MatDBExportXML.png"), "Export XML", this, SLOT(exportMaterialsXML()));
    ToolBar_->addAction(QIcon(":/icons/MatDBExportHTML.png"), "Export HTML", this, SLOT(exportMaterialsHTML()));
    ToolBar_->setFloatable(false);
    ToolBar_->setMovable(false);
    ToolBar_->setToolButtonStyle(Qt::ToolButtonIconOnly);

    QWidget* central = new QWidget(this);
    QVBoxLayout* clayout = new QVBoxLayout(central);
    clayout->setContentsMargins(0, 0, 0, 0);
    central->setLayout(clayout);

    MaterialFilterWidget* filterWidget = new MaterialFilterWidget(central);
    clayout->addWidget(filterWidget);

    MaterialTableView_ = new MaterialTableView(MaterialListModel_,
                                               MaterialSelectionModel_,
                                               PropertyModel_,
                                               ParameterModel_,
                                               central);
    MaterialTableView_->horizontalHeader()->hide();
    clayout->addWidget(MaterialTableView_);
    setCentralWidget(central);

    QDockWidget *dock;

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

    dock = new QDockWidget(tr("Properties"), this);
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

    dock = new QDockWidget(tr("Parameter"), this);
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

    updateGeometry();
}

MatDBMainWindow::~MatDBMainWindow()
{

}

void MatDBMainWindow::makeDefaultMaterial()
{
    MaterialListModel_->addMaterial(Material::makeDefaultMaterial(PropertyModel_));
}

void MatDBMainWindow::exportMaterialsXML()
{
    NQLog("MatDBMainWindow", NQLog::Spam) << "void exportMaterialsXML()";

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
    NQLog("MatDBMainWindow", NQLog::Spam) << "void exportMaterialsHTML()";

    MaterialXMLExportDialog dialog(MaterialListModel_, this);

    int result = dialog.exec();
    if (result==0) return;

    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Export Materials",
                                                    QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation),
                                                    "*.html");
    if (filename.isEmpty() || !filename.endsWith(".html")) return;

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
    NQLog("MatDBMainWindow", NQLog::Spam) << "void importMaterials()";

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

void MatDBMainWindow::closeEvent(QCloseEvent * /* event */)
{
    NQLog("MatDBMainWindow", NQLog::Spam) << "void closeEvent(QCloseEvent *event)";

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
    NQLog("MatDBMainWindow", NQLog::Spam) << "void aboutDialog()";

    MatDBAboutDialog dialog;
         dialog.exec();
}

void MatDBMainWindow::togglePropertyToolBoxDockWidget()
{
    if (togglePropertyToolBoxDockWidgetAction_->isChecked()) {
        propertyToolBoxDockWidget_->show();
        togglePropertyToolBoxDockWidgetAction_->setText("Hide Toolbox");
    } else {
        propertyToolBoxDockWidget_->hide();
        togglePropertyToolBoxDockWidgetAction_->setText("Show Toolbox");
    }
}

void MatDBMainWindow::toggleCategoryDockWidget()
{
    if (toggleCategoryDockWidgetAction_->isChecked()) {
        categoryDockWidget_->show();
        toggleCategoryDockWidgetAction_->setText("Hide Categories");
    } else {
        categoryDockWidget_->hide();
        toggleCategoryDockWidgetAction_->setText("Show Categories");
    }
}

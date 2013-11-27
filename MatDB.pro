#-------------------------------------------------
#
# Project created by QtCreator 2013-03-28T09:24:50
#
#-------------------------------------------------

macx {
  QMAKE_CXXFLAGS += -stdlib=libc++ -std=c++11
  QMAKE_LFLAGS += -stdlib=libc++ -std=c++11
  QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
} else {
  QMAKE_CXXFLAGS += -std=c++11
  QMAKE_LFLAGS += -std=c++11
}

win32-g++ {
  DEFINES += WIN32GPP
}

DEFINES += APPVERMAJOR=0
DEFINES += APPVERMINOR=9
DEFINES += APPPATCHLEVEL=2

QT       += core gui xml

INCLUDEPATH += ./properties ./external

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

greaterThan(QT_MAJOR_VERSION, 4){
  cache()
}

TARGET = MatDB
TEMPLATE = app

macx {
  ICON = artwork/MatDB.icns
}
win32-g++ {
  RC_FILE = artwork/MatDB.rc
}

SOURCES += main.cpp \
           matdbmainwindow.cpp \
           material.cpp \
           materiallistmodel.cpp \
           materialselectionmodel.cpp \
           materialpropertyview.cpp \
           property.cpp \
           properties/densityproperty.cpp \
           properties/viscosityproperty.cpp \
           properties/elasticityproperty.cpp \
           properties/thermalconductivityproperty.cpp \
           properties/cteproperty.cpp \
           properties/resistivityproperty.cpp \
           properties/specificheatproperty.cpp \
           parameter.cpp \
           propertymodel.cpp \
           parametermodel.cpp \
           propertytoolbox.cpp \
           materialparameterview.cpp \
           materialtableview.cpp \
           unit.cpp \
           unitcombobox.cpp \
           parameterselectionmodel.cpp \
           propertyselectionmodel.cpp \
           parameterstackview.cpp \
           matmlreader.cpp \
    matmlwriter.cpp \
    materialmetadatawidget.cpp \
    external/flowlayout.cpp \
    external/nqlogger.cpp \
    external/nqtokenedit.cpp \
    external/nqtokenedititem.cpp \
    external/nqcolorwheel.cpp \
    materialtableitem.cpp \
    materialxmlexportdialog.cpp \
    materialimportdialog.cpp \
    matdbaboutdialog.cpp \
    materialcategory.cpp \
    materialcategorymodel.cpp \
    materialtagsedit.cpp \
    materialcategorybox.cpp \
    materialcategoryeditdialog.cpp \
    propertyspecialwidget.cpp \
    materialfilterwidget.cpp \
    materialcategorywidget.cpp \
    materialindexer.cpp \
    htmlwriter.cpp \
    materialselectiondialog.cpp \
    parameterset.cpp

HEADERS += matdbmainwindow.h \
    material.h \
    materiallistmodel.h \
    materialselectionmodel.h \
    materialpropertyview.h \
    property.h \
    properties/densityproperty.h \
           properties/viscosityproperty.h \
    properties/elasticityproperty.h \
    properties/thermalconductivityproperty.h \
    properties/cteproperty.h \
    properties/resistivityproperty.h \
    properties/specificheatproperty.h \
    parameter.h \
    propertymodel.h \
    parametermodel.h \
    propertytoolbox.h \
    materialparameterview.h \
    materialtableview.h \
    unit.h \
    unitcombobox.h \
    parameterselectionmodel.h \
    propertyselectionmodel.h \
    parameterstackview.h \
    matmlreader.h \
    matmlwriter.h \
    materialmetadatawidget.h \
    external/flowlayout.h \
    external/nqlogger.h \
    external/nqtokenedit.h \
    external/nqtokenedititem.h \
    external/nqcolorwheel.h \
    materialtableitem.h \
    materialxmlexportdialog.h \
    materialimportdialog.h \
    matdbaboutdialog.h \
    materialcategory.h \
    materialcategorymodel.h \
    materialtagsedit.h \
    materialcategorybox.h \
    materialcategoryeditdialog.h \
    propertyspecialwidget.h \
    materialfilterwidget.h \
    materialcategorywidget.h \
    materialindexer.h \
    htmlwriter.h \
    materialselectiondialog.h \
    parameterset.h

FORMS    +=

RESOURCES += resources.qrc

OTHER_FILES += stylesheet.qss \
    icons/MatDBImportXML.png \
    icons/MatDBExportXML.png \
    icons/MatDBExportHTML.png \
    icons/MatDBPropertiesToolBox.png \
    icons/MatDBCategories.png \
    icons/MatDBAddIsotropicMaterial.png \
    icons/MatDBAddOrthotropicMaterial.png \
    icons/MatDBAddLiquidMaterial.png \
    icons/MatDBAddGaseousMaterial.png \
    icons/MatDBAddCategory.png \
    icons/MatDBRemoveCategory.png \
    icons/MatDBResetFilter.png \
    artwork/MatDBSplashScreen.png \
    artwork/MatDBBG.png \
    html/header.html \
    html/footer.html \
    LICENSE.html

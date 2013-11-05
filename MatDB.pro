#-------------------------------------------------
#
# Project created by QtCreator 2013-03-28T09:24:50
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -stdlib=libc++ -std=c++11
QMAKE_LFLAGS += -stdlib=libc++ -std=c++11

QT       += core gui xml

INCLUDEPATH += ./properties ./external

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MatDB
TEMPLATE = app

ICON = artwork/MatDB.icns

SOURCES += main.cpp \
           matdbmainwindow.cpp \
           material.cpp \
           materiallistmodel.cpp \
           materialselectionmodel.cpp \
           materialpropertyview.cpp \
           property.cpp \
           properties/densityproperty.cpp \
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
    materialcategorydialog.cpp \
    materialcategoryeditdialog.cpp

HEADERS  += matdbmainwindow.h \
    material.h \
    materiallistmodel.h \
    materialselectionmodel.h \
    materialpropertyview.h \
    property.h \
    properties/densityproperty.h \
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
    materialcategorydialog.h \
    materialcategoryeditdialog.h

FORMS    +=

RESOURCES += resources.qrc

OTHER_FILES += stylesheet.qss \
    icons/MatDBImportXML.png \
    icons/MatDBExportXML.png \
    icons/MatDBExportHTML.png \
    icons/MatDBEditCategories.png \
    artwork/MatDBSplashScreen.png \
    artwork/MatDBBG.png

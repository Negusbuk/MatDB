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
DEFINES += APPPATCHLEVEL=6

win32-g++ {
  DEFINES += APPGITVERSION=unknown
} else {
  DEFINES += APPGITVERSION=$$system(git describe --tags)
}

QT       += core gui xml

INCLUDEPATH += ./src ./src/properties ./src/external

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

OBJECTS_DIR=build
MOC_DIR=moc

SOURCES += src/main.cpp \
           src/matdbmainwindow.cpp \
           src/material.cpp \
           src/materiallistmodel.cpp \
           src/materialselectionmodel.cpp \
           src/materialpropertyview.cpp \
           src/property.cpp \
           src/properties/densityproperty.cpp \
           src/properties/viscosityproperty.cpp \
           src/properties/elasticityproperty.cpp \
           src/properties/thermalconductivityproperty.cpp \
           src/properties/cteproperty.cpp \
           src/properties/resistivityproperty.cpp \
           src/properties/specificheatproperty.cpp \
           src/parameter.cpp \
           src/propertymodel.cpp \
           src/parametermodel.cpp \
           src/propertytoolbox.cpp \
           src/materialparameterview.cpp \
           src/materialtableview.cpp \
           src/unit.cpp \
           src/unitcombobox.cpp \
           src/parameterselectionmodel.cpp \
           src/propertyselectionmodel.cpp \
           src/parameterstackview.cpp \
           src/matmlreader.cpp \
           src/matmlwriter.cpp \
           src/materialmetadatawidget.cpp \
           src/external/flowlayout.cpp \
           src/external/nqlogger.cpp \
           src/external/nqtokenedit.cpp \
           src/external/nqtokenedititem.cpp \
           src/external/nqcolorwheel.cpp \
           src/materialtableitem.cpp \
           src/materialxmlexportdialog.cpp \
           src/materialimportdialog.cpp \
           src/matdbaboutdialog.cpp \
           src/matdbpreferencedialog.cpp \
           src/materialcategory.cpp \
           src/materialcategorymodel.cpp \
           src/materialtagsedit.cpp \
           src/materialcategorybox.cpp \
           src/materialcategoryeditdialog.cpp \
           src/propertyspecialwidget.cpp \
           src/materialfilterwidget.cpp \
           src/materialcategorywidget.cpp \
           src/materialindexer.cpp \
           src/htmlwriter.cpp \
           src/materialselectiondialog.cpp \
           src/properties/criticalpressureproperty.cpp \
           src/properties/criticaltemperatureproperty.cpp \
           src/properties/boilingpointproperty.cpp \
           src/properties/referencetemperatureproperty.cpp \
           src/properties/plytypeproperty.cpp \
           src/properties/stresslimitsproperty.cpp \
           src/properties/strainlimitsproperty.cpp \
           src/matdblanguagehandler.cpp

HEADERS += src/matdbversion.h \
           src/matdbmainwindow.h \
           src/material.h \
           src/materiallistmodel.h \
           src/materialselectionmodel.h \
           src/materialpropertyview.h \
           src/property.h \
           src/properties/densityproperty.h \
           src/properties/viscosityproperty.h \
           src/properties/elasticityproperty.h \
           src/properties/thermalconductivityproperty.h \
           src/properties/cteproperty.h \
           src/properties/resistivityproperty.h \
           src/properties/specificheatproperty.h \
           src/parameter.h \
           src/propertymodel.h \
           src/parametermodel.h \
           src/propertytoolbox.h \
           src/materialparameterview.h \
           src/materialtableview.h \
           src/unit.h \
           src/unitcombobox.h \
           src/parameterselectionmodel.h \
           src/propertyselectionmodel.h \
           src/parameterstackview.h \
           src/matmlreader.h \
           src/matmlwriter.h \
           src/materialmetadatawidget.h \
           src/external/flowlayout.h \
           src/external/nqlogger.h \
           src/external/nqtokenedit.h \
           src/external/nqtokenedititem.h \
           src/external/nqcolorwheel.h \
           src/materialtableitem.h \
           src/materialxmlexportdialog.h \
           src/materialimportdialog.h \
           src/matdbaboutdialog.h \
           src/matdbpreferencedialog.h \
           src/materialcategory.h \
           src/materialcategorymodel.h \
           src/materialtagsedit.h \
           src/materialcategorybox.h \
           src/materialcategoryeditdialog.h \
           src/propertyspecialwidget.h \
           src/materialfilterwidget.h \
           src/materialcategorywidget.h \
           src/materialindexer.h \
           src/htmlwriter.h \
           src/materialselectiondialog.h \
           src/properties/criticalpressureproperty.h \
           src/properties/criticaltemperatureproperty.h \
           src/properties/boilingpointproperty.h \
           src/properties/referencetemperatureproperty.h \
           src/properties/plytypeproperty.h \
           src/properties/stresslimitsproperty.h \
           src/properties/strainlimitsproperty.h \
           src/matdblanguagehandler.h

FORMS    +=

RESOURCES += resources.qrc

OTHER_FILES += style/stylesheet.qss \
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
               artwork/MatDBAbout.png \
               html/header.html \
               html/footer.html \
               html/LICENSE.html \
               html/COPYRIGHT.html

TRANSLATIONS = translations/MatDB_en.ts \
               translations/MatDB_de.ts

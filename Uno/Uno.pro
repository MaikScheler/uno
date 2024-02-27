QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QT += network
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cardmodel.cpp \
    clickablelabel.cpp \
    main.cpp \
    maincontroller.cpp \
    mainwindow.cpp \
    playermodel.cpp \
    playingfieldmodel.cpp

HEADERS += \
    cardmodel.h \
    clickablelabel.h \
    maincontroller.h \
    mainwindow.h \
    playermodel.h \
    playingfieldmodel.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Uno_de_DE.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QT += network

SOURCES += \
        cardmodel.cpp \
        main.cpp \
        playermodel.cpp \
        playingfieldmodel.cpp \
        server.cpp \
        stackmodel.cpp

TRANSLATIONS += \
    UnoServer_de_DE.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cardmodel.h \
    playermodel.h \
    playingfieldmodel.h \
    server.h \
    stackmodel.h

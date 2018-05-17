#-------------------------------------------------
#
# Project created by QtCreator 2018-05-14T19:00:13
#
#-------------------------------------------------

TARGET=flocking
QT += core gui opengl
OBJECTS_DIR=obj
MOC_DIR=moc


isEqual(QT_MAJOR_VERSION, 5) {
        cache()
        DEFINES +=QT5BUILD
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlockingGUI
TEMPLATE = app
DESTDIR=./

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += $$PWD/src/*.cpp

HEADERS += $$PWD/include/*.h
INCLUDEPATH +=./include
UI_HEADERS_DIR = ui

FORMS += \
        $$PWD/ui/*.ui

OTHER_FILES+= $$PWD/shaders/*.glsl  \
            README.md

NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
        message("including $HOME/NGL")
        include($(HOME)/NGL/UseNGL.pri)
}
else{ # note brace must be here
        message("Using custom NGL location")
        include($(NGLDIR)/UseNGL.pri)
}

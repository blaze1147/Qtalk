#-------------------------------------------------
#
# Project created by QtCreator 2018-07-19T14:15:18
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IM
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    registerwindow.cpp \
    mainwindow.cpp \
    qmylistwidget.cpp \
    chatwindow.cpp

HEADERS += \
    registerwindow.h \
    mainwindow.h \
    qmylistwidget.h \
    chatwindow.h

FORMS += \
    registerwindow.ui \
    chatwindow.ui \
    mainwindow.ui

DISTFILES +=

RESOURCES += \
    qtalk.qrc


INCLUDEPATH += $$PWD/../../../../../usr/local/include/qxmpp
DEPENDPATH += $$PWD/../../../../../usr/local/include/qxmpp

#ubuntu qxmpplib path
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/ubuntuqxmpp/release/ -lqxmpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/ubuntuqxmpp/debug/ -lqxmpp
else:unix: LIBS += -L$$PWD/../../../../../usr/local/lib/ubuntuqxmpp/ -lqxmpp

#nanopc pxmpplib path
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/release/ -lqxmpp
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/debug/ -lqxmpp
#else:unix: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lqxmpp

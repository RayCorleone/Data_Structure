QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Sources/bnode.cpp \
    Sources/btree.cpp \
    Sources/btreewindow.cpp \
    Sources/choosewindow.cpp \
    Sources/dialogdelete.cpp \
    Sources/dialoghelp.cpp \
    Sources/dialoginsert.cpp \
    Sources/dialogupdate.cpp \
    Sources/dswindow.cpp \
    Sources/helpwindow.cpp \
    Sources/linklist.cpp \
    Sources/main.cpp \
    Sources/tree.cpp

HEADERS += \
    Headers/bnode.h \
    Headers/btree.h \
    Headers/btreewindow.h \
    Headers/choosewindow.h \
    Headers/dialogdelete.h \
    Headers/dialoghelp.h \
    Headers/dialoginsert.h \
    Headers/dialogupdate.h \
    Headers/dsstruct.h \
    Headers/dswindow.h \
    Headers/helpwindow.h \
    Headers/linklist.h \
    Headers/tree.h

FORMS += \
    Forms/btreewindow.ui \
    Forms/choosewindow.ui \
    Forms/dialogdelete.ui \
    Forms/dialoghelp.ui \
    Forms/dialoginsert.ui \
    Forms/dialogupdate.ui \
    Forms/dswindow.ui \
    Forms/helpwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/res.qrc

RC_ICONS = Resources/DS_Final.ico

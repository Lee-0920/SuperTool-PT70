#-------------------------------------------------
#
# Project created by QtCreator 2018-06-21T15:10:31
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SuperTool
TEMPLATE = app
DESTDIR = ../bin

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ./Lib/zlib/include

win32 {
    DEFINES += _CS_X86_WINDOWS
    Debug:LIBS   += -L$$PWD/./Lib/zlib/lib/windows/ -lzlibD

    Release:LIBS += -L$$PWD/./Lib/zlib/lib/windows/ -lzlib
}

unix {
     CONFIG += arm

    arm {
        DEFINES += _CS_ARM_LINUX
        LIBS += -L$$PWD/../Lib/zlib/lib/linux-arm/ -lzlib
    }
}


SOURCES += main.cpp \
    UI/AuxiliaryWidget.cpp \
    UI/FileOperateWidget.cpp \
    UI/FileTableWidget.cpp \
    UI/LoginDialog.cpp \
    UI/Mainwindow.cpp \
    UI/MessageDialog.cpp \
    UI/NumberKeyboard.cpp \
    UI/UpdaterWidget.cpp \
    Flow/FlowManager.cpp \
    UI/SysTimeSettingDialog.cpp \
    UI/QMyEdit.cpp \
    System/BanklightControl.cpp \
    System/DynamicPassword.cpp \
    System/md5.cpp \
    System/Uncompress.cpp \
    UI/ImportDialog.cpp \
    checkUser.cpp \
    UI/LuipMessageDialog.cpp \
    UI/LinuxCmd.cpp \
    UI/InputKeyBoard.cpp

HEADERS += \
    UI/AuxiliaryWidget.h \
    UI/FileOperateWidget.h \
    UI/FileTableWidget.h \
    UI/LoginDialog.h \
    UI/Mainwindow.h \
    UI/MessageDialog.h \
    UI/NumberKeyboard.h \
    UI/UpdaterWidget.h \
    Flow/FlowManager.h \
    UI/SysTimeSettingDialog.h \
    UI/QMyEdit.h \
    System/AppCheck.h \
    System/BanklightControl.h \
    System/DynamicPassword.h \
    System/md5.h \
    System/Uncompress.h \
    UI/ImportDialog.h \
    checkUser.h \
    UI/LuipMessageDialog.h \
    UI/LinuxCmd.h \
    UI/InputKeyBoard.h

RESOURCES += \
    Resources/style.qrc

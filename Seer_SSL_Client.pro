TEMPLATE = app

CONFIG += c++11 release

QT += qml quick serialport network

TARGET = Seer_SSL_Client

RESOURCES += qml.qrc

win32 {
    PROTOBUF_INCLUDE_DIR = D:\workspace\yys\include
    debug {
        LIBPROTOBUF_DEBUG = D:\workspace\yys\lib\debug\x86\libprotobuf.lib
    }
    release {
        LIBPROTOBUF_RELEASE = D:\workspace\yys\lib\release\x86\libprotobuf.lib
    }
}

macx {
    LIBPROTOBUF = /usr/local/lib/libprotobuf.dylib
}

HEADERS += \
    $$PWD/src/commandparser.h \
    $$PWD/src/dataqueue.hpp \
    $$PWD/src/field.h \
    $$PWD/src/interaction.h \
    $$PWD/src/params.h \
    $$PWD/src/radiopacket.h \
    $$PWD/src/refereebox.h \
    $$PWD/src/serialobject.h \
    $$PWD/src/singleparams.h \
    $$PWD/src/singleton.hpp \
    $$PWD/src/visionmodule.h \
    $$PWD/src/lib/crc/crc.h \
    $$PWD/src/proto/messages_robocup_ssl_detection.pb.h \
    $$PWD/src/proto/messages_robocup_ssl_geometry_legacy.pb.h \
    $$PWD/src/proto/messages_robocup_ssl_geometry.pb.h \
    $$PWD/src/proto/messages_robocup_ssl_refbox_log.pb.h \
    $$PWD/src/proto/messages_robocup_ssl_wrapper_legacy.pb.h \
    $$PWD/src/proto/messages_robocup_ssl_wrapper.pb.h \
    $$PWD/src/proto/rcon.pb.h \
    $$PWD/src/proto/referee.pb.h \
    $$PWD/src/proto/savestate.pb.h \
    $$PWD/src/lib/json.hpp \
    $$PWD/src/follow/BallFollow.h \
    $$PWD/src/follow/Follow.h \
    $$PWD/src/follow/munkres.h \
    $$PWD/src/follow/RobotFollow.h \
    $$PWD/src/follow/matrix.h \
    $$PWD/src/follow/message.h \
    $$PWD/src/follow/TwoCameraMixer.h \
    $$PWD/src/follow/GlobalData.h \
    $$PWD/src/follow/Transmit.h \
    $$PWD/src/translation.hpp \
    src/threadmanager.h \
    src/visionthread.h

SOURCES += \
    $$PWD/src/commandparser.cpp \
    $$PWD/src/field.cpp \
    $$PWD/src/interaction.cpp \
    $$PWD/src/main.cpp \
    $$PWD/src/radiopacket.cpp \
    $$PWD/src/refereebox.cpp \
    $$PWD/src/serialobject.cpp \
    $$PWD/src/singleparams.cpp \
    $$PWD/src/visionmodule.cpp \
    $$PWD/src/lib/crc/crc.cpp \
    $$PWD/src/proto/messages_robocup_ssl_detection.pb.cc \
    $$PWD/src/proto/messages_robocup_ssl_geometry_legacy.pb.cc \
    $$PWD/src/proto/messages_robocup_ssl_geometry.pb.cc \
    $$PWD/src/proto/messages_robocup_ssl_refbox_log.pb.cc \
    $$PWD/src/proto/messages_robocup_ssl_wrapper_legacy.pb.cc \
    $$PWD/src/proto/messages_robocup_ssl_wrapper.pb.cc \
    $$PWD/src/proto/rcon.pb.cc \
    $$PWD/src/proto/referee.pb.cc \
    $$PWD/src/proto/savestate.pb.cc \
    $$PWD/src/follow/BallFollow.cpp \
    $$PWD/src/follow/Follow.cpp \
    $$PWD/src/follow/munkres.cpp \
    $$PWD/src/follow/RobotFollow.cpp \
    $$PWD/src/follow/matrix.cpp \
    $$PWD/src/follow/TwoCameraMixer.cpp \
    $$PWD/src/follow/GlobalData.cpp \
    $$PWD/src/follow/Transmit.cpp \
    src/threadmanager.cpp \
    src/visionthread.cpp

TRANSLATIONS = t1_zh.ts

defineTest(copyToDestdir) {
    files = $$1

    for(FILE, files) {
        macx {
            DDIR = $${DESTDIR}/$${TARGET}.app/Contents/MacOS
        }
        win32 {
            DDIR = $$DESTDIR
        }

        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g

        QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}

win32 {
    RC_ICONS = logo.ico
    CONFIG(debug, debug|release) {
        #CONFIG += console
        DESTDIR = ./debug/bin
        MOC_DIR = ./debug
        OBJECTS_DIR = ./debug
        LIBS += $$LIBPROTOBUF_DEBUG
    }
    CONFIG(release, debug|release) {
        DESTDIR = ./release/bin
        MOC_DIR = ./release
        OBJECTS_DIR = ./release
        LIBS += $$LIBPROTOBUF_RELEASE
    }
    INCLUDEPATH += $$PROTOBUF_INCLUDE_DIR
    INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.10240.0/ucrt"
    LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10240.0/ucrt/x86"
    system(lrelease t1_zh.ts)
    copyToDestdir($$PWD/params.json $$PWD/t1_zh.qm)
}

macx {
    CONFIG(debug, debug|release) {
        #CONFIG += console
        DESTDIR = ./debug/bin
        MOC_DIR = ./debug
        OBJECTS_DIR = ./debug
    }
    CONFIG(release, debug|release) {
        DESTDIR = ./release/bin
        MOC_DIR = ./release
        OBJECTS_DIR = ./release
    }
    INCLUDEPATH += /usr/local/include
    system(lrelease t1_zh.ts)
    copyToDestdir($$PWD/params.json $$PWD/t1_zh.qm)
    LIBS += $$LIBPROTOBUF
}


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

#A fix is to patch the file C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\vcvarsqueryregistry.bat as follows:
#--- vcvarsqueryregistry.bat     Thu Mar 17 22:24:58 2016
#+++ vcvarsqueryregistry.bat     Thu May 05 12:48:58 2016
#@@ -351,7 +351,10 @@
# @setlocal enableDelayedExpansion
# @for /f %%i IN ('dir "%UniversalCRTSdkDir%include\" /b /ad-h /on') DO (
#     @set result=%%i
#-    @if "!result:~0,3!"=="10." set CRT=!result!
#+    @set resultcrt=
#+    @if "!result:~0,3!"=="10." set resultcrt=!result!
#+    @if not EXIST "%UniversalCRTSdkDir%include\!result!\ucrt" set resultcrt=
#+    @if not "!resultcrt!"=="" set CRT=!result!
#     @if "!result!"=="%user_inputversion%" set match=1
# )
# @if not "%match%"=="" set CRT=%user_inputversion%

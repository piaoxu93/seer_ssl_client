unix:!android {
    isEmpty(target.path) {
        qnx {
            target.path = /tmp/$${TARGET}/bin
        } else {
            target.path = /opt/$${TARGET}/bin
        }
        export(target.path)
    }
    INSTALLS += target
}

export(INSTALLS)

HEADERS += \
    $$PWD/src/commandparser.h \
    $$PWD/src/dataqueue.hpp \
    $$PWD/src/field.h \
    $$PWD/src/globaldata.h \
    $$PWD/src/interaction.h \
    $$PWD/src/json.hpp \
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
    $$PWD/src/proto/savestate.pb.h

SOURCES += \
    $$PWD/src/commandparser.cpp \
    $$PWD/src/field.cpp \
    $$PWD/src/globaldata.cpp \
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
    $$PWD/src/proto/savestate.pb.cc

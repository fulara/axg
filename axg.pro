QMAKE_LIBS_QT_OPENGL
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS +=  -std=c++0x

unix:!macx:
SOURCES += main.cpp \
    axgapplication.cpp \
    logger.cpp \
    ggwrapper.cpp \
    Ui/loginwindow.cpp \
    alivechecker.cpp \
    Ui/dialogwindow.cpp \
    Ui/chathistory.cpp \
    Ui/historyentry.cpp \
    Ui/contactlist.cpp \
    Ui/contactentry.cpp \
    Ui/contactwindow.cpp \
    Ui/dialogwindowholder.cpp \
    WtUtilFuns.cpp \
    historymanager.cpp






HEADERS += \
    axgapplication.h \
    WtForwards.h \
    logger.h \
    ggwrapper.h \
    ggevent.h \
    ggloginevent.h \
    synchronizedqueue.h \
    event.h \
    loginresultevent.h \
    Ui/loginwindow.h \
    alivechecker.h \
    Ui/dialogwindow.h \
    Ui/chathistory.h \
    Ui/historyentry.h \
    ggmessageevent.h \
    messageevent.h \
    Ui/contactlist.h \
    Ui/contactentry.h \
    Ui/contactwindow.h \
    contactinfo.h \
    ContactGroup.h \
    ContactImportEvent.h \
    Ui/dialogwindowholder.h \
    WtUtilFuns.h \
    historymanager.h \
    ggTypingEvent.h \
    TypingNotificationEvent.h \
    CharacterConversion.h



unix:!macx: LIBS += -L$$PWD/../../libgadu/deploy/ -llibgadu

INCLUDEPATH += $$PWD/../../libgadu/libgadu/include
DEPENDPATH += $$PWD/../../libgadu/libgadu/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../libgadu/deploy/liblibgadu.a


INCLUDEPATH += /home/fular/includes/



unix:!macx: LIBS += -L$$PWD/../../../libs/wt-3.3.1-rc1/build/src/http/ -lwthttp

INCLUDEPATH += $$PWD/../../../libs/wt-3.3.1-rc1/includes
DEPENDPATH += $$PWD/../../../libs/wt-3.3.1-rc1/build/src/http

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../libs/wt-3.3.1-rc1/build/src/http/libwthttp.a

unix:!macx: LIBS += -L$$PWD/../../../libs/wt-3.3.1-rc1/build/src/ -lwt

INCLUDEPATH += $$PWD/../../../libs/wt-3.3.1-rc1/includes
DEPENDPATH += $$PWD/../../../libs/wt-3.3.1-rc1/build/src

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../libs/wt-3.3.1-rc1/build/src/libwt.a

unix:!macx: LIBS += -L$$PWD/../../../libs/boost_1_53_0/stage/lib/ -lboost_signals

INCLUDEPATH += $$PWD/../../../libs/boost_1_53_0
DEPENDPATH += $$PWD/../../../libs/boost_1_53_0

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../libs/boost_1_53_0/stage/lib/libboost_signals.a



unix:!macx: LIBS += -L$$PWD/../../../libs/boost_1_53_0/stage/lib/ -lboost_system

INCLUDEPATH += $$PWD/../../../libs/boost_1_53_0
DEPENDPATH += $$PWD/../../../libs/boost_1_53_0

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../libs/boost_1_53_0/stage/lib/libboost_system.a



unix:!macx: LIBS += -L$$PWD/../../../libs/boost_1_53_0/stage/lib/ -lboost_thread

INCLUDEPATH += $$PWD/../../../libs/boost_1_53_0
DEPENDPATH += $$PWD/../../../libs/boost_1_53_0

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../libs/boost_1_53_0/stage/lib/libboost_thread.a


unix:!macx: LIBS += -L$$PWD/../../../libs/boost_1_53_0/stage/lib/ -lboost_date_time

INCLUDEPATH += $$PWD/../../../libs/boost_1_53_0
DEPENDPATH += $$PWD/../../../libs/boost_1_53_0

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../libs/boost_1_53_0/stage/lib/libboost_date_time.a



unix:!macx: LIBS += -L$$PWD/../../../libs/boost_1_53_0/stage/lib/ -lboost_regex

INCLUDEPATH += $$PWD/../../../libs/boost_1_53_0
DEPENDPATH += $$PWD/../../../libs/boost_1_53_0

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../libs/boost_1_53_0/stage/lib/libboost_regex.a


unix:!macx: LIBS += -L$$PWD/../../../libs/boost_1_53_0/stage/lib/ -lboost_random

INCLUDEPATH += $$PWD/../../../libs/boost_1_53_0
DEPENDPATH += $$PWD/../../../libs/boost_1_53_0

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../libs/boost_1_53_0/stage/lib/libboost_random.a


unix:!macx: LIBS += -L$$PWD/../../../libs/boost_1_53_0/stage/lib/ -lboost_filesystem

unix:!macx: LIBS += -L$$PWD/../../../libs/boost_1_53_0/stage/lib/ -lboost_locale

INCLUDEPATH += $$PWD/../../../libs/boost_1_53_0
DEPENDPATH += $$PWD/../../../libs/boost_1_53_0

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../libs/boost_1_53_0/stage/lib/libboost_filesystem.a

unix:!macx: LIBS += -L$$PWD/../../../libs/boost_1_53_0/stage/lib/ -lboost_program_options

INCLUDEPATH += $$PWD/../../../libs/boost_1_53_0
DEPENDPATH += $$PWD/../../../libs/boost_1_53_0

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../libs/boost_1_53_0/stage/lib/libboost_program_options.a

LIBS += -lpthread
LIBS += -lrt
LIBS += -lssl
LIBS += -lcrypto
LIBS += -lz
LIBS += -ldl


unix:!macx: LIBS += -L$$PWD/../../../libs/pugixml/ -lpugixml

INCLUDEPATH += $$PWD/../../../libs/pugixml/includes
DEPENDPATH += $$PWD/../../../libs/pugixml/includes

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../libs/pugixml/libpugixml.a

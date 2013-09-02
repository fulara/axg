TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS +=  -std=c++0x

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
    Ui/contactwindow.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../libgadu/build-libgadu-Desktop_Qt_5_1_1_GCC_32bit-Release/release/ -llibgadu
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../libgadu/build-libgadu-Desktop_Qt_5_1_1_GCC_32bit-Release/debug/ -llibgadu
else:unix: LIBS += -L$$PWD/../../libgadu/build-libgadu-Desktop_Qt_5_1_1_GCC_32bit-Release/ -llibgadu

INCLUDEPATH += $$PWD/../../libgadu/libgadu/include
DEPENDPATH += $$PWD/../../libgadu/libgadu/include

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../libgadu/build-libgadu-Desktop_Qt_5_1_1_GCC_32bit-Release/release/libgadu.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../libgadu/build-libgadu-Desktop_Qt_5_1_1_GCC_32bit-Release/debug/libgadu.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../libgadu/build-libgadu-Desktop_Qt_5_1_1_GCC_32bit-Release/liblibgadu.a




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
    Ui/contactwindow.h


unix:!macx: LIBS += -L$$PWD/../../../libs/wt-3.3.0/build/src/ -lwt

INCLUDEPATH += $$PWD/../../../libs/wt-3.3.0/includes
DEPENDPATH += $$PWD/../../../libs/wt-3.3.0/includes


unix:!macx: LIBS += -L$$PWD/../../../libs/wt-3.3.0/build/src/http/ -lwthttp

INCLUDEPATH += $$PWD/../../../libs/wt-3.3.0/includes
DEPENDPATH += $$PWD/../../../libs/wt-3.3.0/includes

unix:!macx: LIBS += -L$$PWD/../../../libs/boost_1_53/stage/lib/ -lboost_signals

INCLUDEPATH += $$PWD/../../../libs/boost_1_53
DEPENDPATH += $$PWD/../../../libs/boost_1_53

unix:!macx: LIBS += -L$$PWD/../../../libs/boost_1_53/stage/lib/ -lboost_thread

INCLUDEPATH += $$PWD/../../../libs/boost_1_53
DEPENDPATH += $$PWD/../../../libs/boost_1_53

unix:!macx: LIBS += -L$$PWD/../../../libs/boost_1_53/stage/lib/ -lboost_system

INCLUDEPATH += $$PWD/../../../libs/boost_1_53
DEPENDPATH += $$PWD/../../../libs/boost_1_53

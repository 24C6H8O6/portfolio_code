QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Seat.cpp \
    book.cpp \
    find.cpp \
    join.cpp \
    login.cpp \
    main.cpp \
    map.cpp \
    pay.cpp \
    screen.cpp \
    trainfeepage.cpp \
    trainlist.cpp \
    traintime.cpp \
    widget.cpp

HEADERS += \
    Seat.h \
    book.h \
    find.h \
    join.h \
    login.h \
    map.h \
    pay.h \
    screen.h \
    trainfeepage.h \
    trainlist.h \
    traintime.h \
    widget.h

FORMS += \
    Seat.ui \
    book.ui \
    find.ui \
    join.ui \
    login.ui \
    map.ui \
    pay.ui \
    screen.ui \
    trainfeepage.ui \
    trainlist.ui \
    traintime.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc \
    resource.qrc \
    resources.qrc

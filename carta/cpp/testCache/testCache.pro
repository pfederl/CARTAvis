! include(../common.pri) {
  error( "Could not find the common.pri file!" )
}

QT      +=  webkitwidgets network widgets xml sql

HEADERS += \
    LevelDbIPCache.h \
    SqLitePCache.h

SOURCES += \
    main.cpp \
    LevelDbIPCache.cpp \
    SqLitePCache.cpp

RESOURCES =

unix: LIBS += -L$$OUT_PWD/../core/ -lcore
unix: LIBS += -L$$OUT_PWD/../CartaLib/ -lCartaLib
DEPENDPATH += $$PROJECT_ROOT/core
DEPENDPATH += $$PROJECT_ROOT/CartaLib

QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN/../CartaLib:\$$ORIGIN/../core\''

QWT_ROOT = $$absolute_path("../../../ThirdParty/qwt")
unix:macx {
    QMAKE_LFLAGS += '-F$$QWT_ROOT/lib'
    LIBS +=-framework qwt
    PRE_TARGETDEPS += $$OUT_PWD/../core/libcore.dylib
}
else{
    QMAKE_LFLAGS += '-Wl,-rpath,\'$$QWT_ROOT/lib\''
    LIBS +=-L$$QWT_ROOT/lib -lqwt
    PRE_TARGETDEPS += $$OUT_PWD/../core/libcore.so
}


LEVELDBDIR=/home/pfederl/Downloads/build/leveldb-1.19
#unix: LIBS += -L$$LEVELDBDIR/out-shared -lleveldb
unix: LIBS += -L$$LEVELDBDIR/out-static -lleveldb
INCLUDEPATH += $$LEVELDBDIR/include

INCLUDEPATH += $${PWD}
DEPENDPATH +=$$PWD/solver
DEPENDPATH +=$$PWD/gui

include($$PWD/solver/solver.pri)
include($$PWD/gui/gui.pri)

SOURCES += $$PWD/main.cpp 

HEADERS += $$PWD/constans.h

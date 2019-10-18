TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
TARGET = $${TARGET}$${TARGET_POSTFIX}

include(deployment.pri)
qtcAddDeployment()


OBJECTS_DIR = $${EXAMPLES_ROOT}/build_$${QT_ARCH}/$${TARGET}
MOC_DIR = $$OBJECTS_DIR/.moc
RCC_DIR = $$OBJECTS_DIR/.qrc
UI_DIR = $$OBJECTS_DIR/.u

PROJECT_DIR = $${PWD}/../EmotivWrapper

HEADERS += $${PROJECT_DIR}/Emotiv.hpp
SOURCES += $${PROJECT_DIR}/Emotiv.cpp
HEADERS += $${PROJECT_DIR}/MyEmotivCallback.hpp
SOURCES += $${PROJECT_DIR}/MyEmotivCallback.cpp
SOURCES += $${PROJECT_DIR}/main.cpp

OTHER_FILES+=$${PROJECT_DIR}/CMakeLists.txt


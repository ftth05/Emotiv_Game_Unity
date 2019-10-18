include(deployment.pri)
qtcAddDeployment()

QT += core xml sql network websockets
QT += svg widgets gui
QT += qml quick quickcontrols2

CONFIG += c++11

TARGET = $${TARGET}$${TARGET_POSTFIX}
OBJECTS_DIR = $${EXAMPLES_ROOT}/build_$${QT_ARCH}/$${TARGET}
MOC_DIR = $$OBJECTS_DIR/.moc
RCC_DIR = $$OBJECTS_DIR/.qrc
UI_DIR = $$OBJECTS_DIR/.u

PROJECT_DIR = $${PWD}/../EmoQuick

INCLUDEPATH += $${PROJECT_DIR}
INCLUDEPATH += $${PROJECT_DIR}/../EmotivWrapper

SOURCES += $${PROJECT_DIR}/../EmotivWrapper/Emotiv.cpp
HEADERS += $${PROJECT_DIR}/../EmotivWrapper/Emotiv.hpp

HEADERS += $${PROJECT_DIR}/../EmotivWrapper/QEmotivEngine.hpp
SOURCES += $${PROJECT_DIR}/../EmotivWrapper/QEmotivEngine.cpp

#SOURCES += $${PROJECT_DIR}/srvthread.cpp
#HEADERS += $${PROJECT_DIR}/srvthread.h

SOURCES += $${PROJECT_DIR}/main.cpp

OTHER_FILES += $${PROJECT_DIR}/../EmotivWrapper/main.cpp

RESOURCES += $${PROJECT_DIR}/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


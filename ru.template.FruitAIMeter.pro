TARGET = ru.template.FruitAIMeter

CONFIG += \
    auroraapp \
    c++11

QT += quick qml multimedia

# Включаем конфигурацию от Conan
#include(conanbuildinfo.pri)
#CONANLIBS += $$replace(CONAN_LIBS, -l, -l:lib)
#CONANLIBS += $$replace(CONAN_SYSTEM_LIBS, -l, -l:lib)
#CONFIG += $$CONAN_CONFIG
#QMAKE_CXXFLAGS += $$CONAN_CXXFLAGS
#QMAKE_LFLAGS += $$CONAN_LDFLAGS

SOURCES += \
    src/main.cpp \
    src/ImageAnalyzer.cpp \
    src/TFLiteModel.cpp \
    src/DemoClassifier.cpp \
    src/ImageProcessor.cpp

HEADERS += \
    src/ImageAnalyzer.h \
    src/TFLiteModel.h \
    src/DemoClassifier.h \
    src/ImageProcessor.h

DISTFILES += \
    rpm/ru.template.FruitAIMeter.spec

RESOURCES += \
    resources.qrc

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += auroraapp_i18n

TRANSLATIONS += \
    translations/ru.template.FruitAIMeter.ts \
    translations/ru.template.FruitAIMeter-ru.ts

# РАЗРЕШЕНИЯ для камеры и файловой системы
AURORAAPP_PERMISSIONS = \
    camera \
    file

# Принудительная переустановка при конфликтах
CONFIG += force_deploy

# Для работы с математическими функциями
LIBS += -lm

# Отладочная информация
message("CONAN LIBS: $$CONAN_LIBS")
message("CONAN INCLUDES: $$CONAN_INCLUDES")

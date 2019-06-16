TEMPLATE = aux

INSTALLER = QuickCut-x64-1.0.2-installer

INPUT = \
    $$PWD/config/config.xml \
    $$PWD/packages

RESOURCES += \
    resources/installer.qrc


installer.input = INPUT
installer.output = $$INSTALLER
installer.commands = C:\Qt\QtIFW-3.1.1\bin\binarycreator.exe -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT}
installer.CONFIG += target_predeps no_link combine

QMAKE_EXTRA_COMPILERS += installer

FORMS += $$PWD/com.greich.quickcut.msvc141.x64/meta/licensewidget.ui

OTHER_FILES += \
    $$PWD/resources/* \
    $$PWD/README \
    $$PWD/com.greich.quickcut.msvc141.x64/data/* \
    $$PWD/com.greich.quickcut.msvc141.x64/meta/*

# after making change here, you need to rebuild project


CONFIG(release, debug|release): BINDIR = $$PWD/../bin/$$QT_VERSION/release
CONFIG(debug, debug|release): BINDIR = $$PWD/../bin/$$QT_VERSION/debug

DESTDIR = $${BINDIR}

android {
    DEFINES += EMBEDDED_TARGET
    message("Android target. Define EMBEDDED_TARGET.")
}

CONFIG += c++11

DEFINES+= MULTITHREAD_RENDERING

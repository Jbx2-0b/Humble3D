# after making change here, you need to rebuild project
android {
    DEFINES += EMBEDDED_TARGET
    message("Android target. Define EMBEDDED_TARGET.")
}

win32-g++ {
    DEFINES += EMBEDDED_TARGET
}

CONFIG += c++11

#DEFINES+= MULTITHREAD_RENDERING

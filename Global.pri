# after making change here, you need to rebuild project

android {
    DEFINES += EMBEDDED_TARGET
    message("Android target. Define EMBEDDED_TARGET.")
}

DEFINES+= MULTITHREAD_RENDERING

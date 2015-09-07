#include <QApplication>

#include "ScriptLoaderApp.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ScriptLoaderApp scriptLoaderApp;

    return app.exec();
}

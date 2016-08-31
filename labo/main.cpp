#include <QApplication>
#include "LaboApp.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LaboApp window;
    return app.exec();
}

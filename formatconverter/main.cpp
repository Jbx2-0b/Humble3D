#include <QApplication>
#include "FormatConverterApp.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    FormatConverterApp window;
    window.show();
    return app.exec();
}

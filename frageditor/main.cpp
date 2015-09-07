#include <QApplication>
#include "FragEditor.h"
// #include <vld.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    FragEditor window;
    return app.exec();
}

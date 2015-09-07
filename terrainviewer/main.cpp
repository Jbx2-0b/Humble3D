#include <QApplication>
#include "TerrainViewerApp.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	TerrainViewerApp window;
	window.show();
    return app.exec();
}

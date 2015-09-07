#include "TestMultiPass.h"
#include "TestShadowVolume.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    TestMultiPass w;

    //TestShadowVolume sv;

	return a.exec();
}

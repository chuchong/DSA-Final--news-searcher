#include "gui.h"
#include <QtWidgets/QApplication>
#include"../HashTable.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	gui w;
	w.show();
	return a.exec();
	_CrtDumpMemoryLeaks();
}

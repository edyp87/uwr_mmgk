/**
	@name:     mmgk_painter_gui
	@author:   Marek Jenda
	@revision: 1.0
	@date:     29.03.2014
	@file:     main.h
	@depends:  window.h
**/

#include "window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window window;
    window.show();

    return a.exec();
}

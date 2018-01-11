#define USE_FAKE_DEVICE

#include <QApplication>
#include <plog/Init.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include "Gui/mainwindow.h"
#include "Core/init.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug,&consoleAppender);
    Core core;
    MainWindow w(&core);
    w.show();
    return a.exec();
}

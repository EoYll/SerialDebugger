#include "SerialDebugger.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SerialDebugger window;
    window.show();
    return app.exec();
}

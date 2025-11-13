#include "MainWindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("Qt Skia Integration");
    app.setApplicationVersion("1.0.0");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}

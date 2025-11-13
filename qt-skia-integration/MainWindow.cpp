#include "MainWindow.h"
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Qt Skia Integration Example");
    resize(800, 600);
    
    // 创建中央widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // 创建布局
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    
    // 创建Skia widget
    m_skiaWidget = new SkiaWidget(this);
    layout->addWidget(m_skiaWidget);
}

MainWindow::~MainWindow()
{
}

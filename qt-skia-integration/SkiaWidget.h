#ifndef SKIAWIDGET_H
#define SKIAWIDGET_H

#include <QWidget>
#include <QImage>
#include <QTimer>

// Skia头文件
#include "SkCanvas.h"
#include "SkSurface.h"
#include "SkPaint.h"
#include "SkPath.h"
#include "SkColor.h"
#include "SkRect.h"

class SkiaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SkiaWidget(QWidget *parent = nullptr);
    ~SkiaWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void initializeSkia();
    void drawSkiaContent();
    
    // Skia相关对象
    sk_sp<SkSurface> m_surface;
    SkCanvas* m_canvas;
    QImage m_image;
    
    // 动画相关
    QTimer* m_timer;
    float m_animationValue;
    
private slots:
    void onAnimationTimer();
};

#endif // SKIAWIDGET_H

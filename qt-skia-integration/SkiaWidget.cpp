#include "SkiaWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QDebug>

SkiaWidget::SkiaWidget(QWidget *parent)
    : QWidget(parent)
    , m_canvas(nullptr)
    , m_animationValue(0.0f)
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    
    // 初始化动画定时器
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SkiaWidget::onAnimationTimer);
    m_timer->start(16); // 约60fps
    
    initializeSkia();
}

SkiaWidget::~SkiaWidget()
{
    // Skia对象会自动释放（使用智能指针）
}

void SkiaWidget::initializeSkia()
{
    // 创建Skia图像信息
    SkImageInfo imageInfo = SkImageInfo::MakeN32Premul(
        width(), height(),
        SkColorSpace::MakeSRGB()
    );
    
    // 创建Skia表面
    m_surface = SkSurface::MakeRaster(imageInfo);
    if (!m_surface) {
        qWarning() << "Failed to create Skia surface";
        return;
    }
    
    m_canvas = m_surface->getCanvas();
    m_canvas->clear(SK_ColorWHITE);
}

void SkiaWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    
    // 重新初始化Skia以适应新尺寸
    initializeSkia();
    update();
}

void SkiaWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    if (!m_surface) {
        return;
    }
    
    // 绘制Skia内容
    drawSkiaContent();
    
    // 将Skia表面转换为QImage
    sk_sp<SkImage> image = m_surface->makeImageSnapshot();
    if (!image) {
        return;
    }
    
    SkImageInfo info = image->imageInfo();
    size_t rowBytes = image->rowBytes();
    size_t size = rowBytes * info.height();
    
    // 创建QImage
    QImage::Format format = QImage::Format_RGBA8888_Premultiplied;
    if (info.colorType() == kRGBA_8888_SkColorType) {
        format = QImage::Format_RGBA8888_Premultiplied;
    } else if (info.colorType() == kBGRA_8888_SkColorType) {
        format = QImage::Format_ARGB32_Premultiplied;
    }
    
    // 复制像素数据
    QImage qImage(static_cast<const uchar*>(image->peekPixels(nullptr, nullptr)),
                  info.width(), info.height(), rowBytes, format);
    
    // 绘制到Qt窗口
    QPainter painter(this);
    painter.drawImage(0, 0, qImage);
}

void SkiaWidget::drawSkiaContent()
{
    if (!m_canvas) {
        return;
    }
    
    // 清除画布
    m_canvas->clear(SK_ColorWHITE);
    
    // 示例1: 绘制矩形
    SkPaint paint;
    paint.setColor(SK_ColorBLUE);
    paint.setStyle(SkPaint::kFill_Style);
    paint.setAntiAlias(true);
    
    SkRect rect = SkRect::MakeXYWH(50, 50, 200, 150);
    m_canvas->drawRect(rect, paint);
    
    // 示例2: 绘制圆形
    paint.setColor(SK_ColorRED);
    SkRect circleRect = SkRect::MakeXYWH(300, 100, 150, 150);
    m_canvas->drawOval(circleRect, paint);
    
    // 示例3: 绘制路径
    SkPath path;
    path.moveTo(100, 300);
    path.lineTo(200, 250);
    path.lineTo(300, 300);
    path.close();
    
    paint.setColor(SK_ColorGREEN);
    m_canvas->drawPath(path, paint);
    
    // 示例4: 绘制文本（需要SkFont）
    // 注意：Skia文本渲染需要SkFont和SkTypeface
    // 这里仅作示例，实际使用时需要正确配置字体
    
    // 示例5: 动画效果 - 旋转的矩形
    m_canvas->save();
    m_canvas->translate(width() / 2.0f, height() / 2.0f);
    m_canvas->rotate(m_animationValue * 360.0f);
    
    paint.setColor(0xFF9C27B0); // 紫色
    SkRect animRect = SkRect::MakeXYWH(-50, -50, 100, 100);
    m_canvas->drawRect(animRect, paint);
    
    m_canvas->restore();
    
    // 示例6: 渐变效果（使用SkShader）
    // 这里可以添加更复杂的绘制效果
}

void SkiaWidget::onAnimationTimer()
{
    m_animationValue += 0.01f;
    if (m_animationValue >= 1.0f) {
        m_animationValue = 0.0f;
    }
    update(); // 触发重绘
}

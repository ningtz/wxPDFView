# Qt集成Skia快速开始

## 快速步骤

### 1. 获取Skia库

**选项A: 下载预编译版本**
```bash
# 从GitHub下载预编译的Skia库
# https://github.com/google/skia/releases
# 解压到 third_party/skia 目录
```

**选项B: 使用包管理器（Linux）**
```bash
# 某些发行版可能提供Skia包
sudo apt-get install libskia-dev  # Debian/Ubuntu
```

### 2. 配置CMake

修改 `CMakeLists.txt` 中的Skia路径：
```cmake
set(SKIA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/skia")
set(SKIA_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/skia")
```

### 3. 编译项目

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt
cmake --build .
```

### 4. 运行

```bash
./QtSkiaIntegration
```

## 最小示例

最简单的Qt + Skia集成代码：

```cpp
#include <QWidget>
#include <QPainter>
#include "SkCanvas.h"
#include "SkSurface.h"

class SimpleSkiaWidget : public QWidget {
    sk_sp<SkSurface> surface;
    
public:
    void paintEvent(QPaintEvent*) override {
        // 创建Skia表面
        SkImageInfo info = SkImageInfo::MakeN32Premul(width(), height());
        surface = SkSurface::MakeRaster(info);
        SkCanvas* canvas = surface->getCanvas();
        
        // 使用Skia绘制
        SkPaint paint;
        paint.setColor(SK_ColorBLUE);
        canvas->drawRect(SkRect::MakeXYWH(10, 10, 100, 100), paint);
        
        // 转换为QImage并绘制
        sk_sp<SkImage> img = surface->makeImageSnapshot();
        QImage qimg((uchar*)img->peekPixels(nullptr, nullptr), 
                    width(), height(), QImage::Format_RGBA8888_Premultiplied);
        QPainter(this).drawImage(0, 0, qimg);
    }
};
```

## 目录结构

```
qt-skia-integration/
├── CMakeLists.txt          # CMake配置文件
├── main.cpp                # 程序入口
├── MainWindow.h/cpp        # 主窗口
├── SkiaWidget.h/cpp        # Skia绘制组件
├── README.md               # 详细文档
└── QUICKSTART.md          # 本文件
```

## 常见问题速查

| 问题 | 解决方案 |
|------|---------|
| 找不到Skia头文件 | 检查 `SKIA_INCLUDE_DIR` 路径 |
| 链接错误 | 确保Skia库文件在正确位置 |
| 运行时崩溃 | 检查Skia版本兼容性 |

更多详细信息请参考 `README.md`。

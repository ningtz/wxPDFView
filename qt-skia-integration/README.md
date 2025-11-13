# Qt项目集成Skia指南

本指南介绍如何在Qt项目中集成Google Skia图形库。

## 目录
1. [准备工作](#准备工作)
2. [获取和编译Skia](#获取和编译skia)
3. [CMake配置](#cmake配置)
4. [代码集成](#代码集成)
5. [编译和运行](#编译和运行)
6. [常见问题](#常见问题)

## 准备工作

### 系统要求
- CMake 3.16或更高版本
- Qt 5.12+ 或 Qt 6.0+
- C++17编译器
- Python 3（用于Skia构建）

### 依赖项
Skia需要以下依赖：
- **Linux**: fontconfig, freetype, libpng, libjpeg
- **macOS**: Xcode Command Line Tools
- **Windows**: Visual Studio 2019或更高版本

## 获取和编译Skia

### 方法1: 使用预编译版本（推荐）

从以下位置获取预编译的Skia库：
- [Skia Releases](https://github.com/google/skia/releases)
- [Skia Binaries](https://github.com/google/skia-binaries)

### 方法2: 从源码编译

```bash
# 1. 安装depot_tools
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
export PATH="$PATH:/path/to/depot_tools"

# 2. 获取Skia源码
git clone https://skia.googlesource.com/skia.git
cd skia

# 3. 同步依赖
python tools/git-sync-deps

# 4. 生成构建文件（使用GN）
bin/gn gen out/Release --args='is_debug=false'

# 5. 编译
ninja -C out/Release
```

编译完成后，库文件位于 `out/Release` 目录。

## CMake配置

### 基本配置

在 `CMakeLists.txt` 中：

1. **设置Skia路径**：
```cmake
set(SKIA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/skia" CACHE PATH "Skia source directory")
set(SKIA_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/skia" CACHE PATH "Skia build directory")
```

2. **查找Skia库**：
```cmake
find_library(SKIA_LIBRARY
    NAMES skia libskia
    PATHS ${SKIA_BUILD_DIR} /usr/local/lib
)
```

3. **设置包含目录**：
```cmake
include_directories(${SKIA_INCLUDE_DIR})
include_directories(${SKIA_DIR}/include/core)
include_directories(${SKIA_DIR}/include/gpu)
```

4. **链接库**：
```cmake
target_link_libraries(your_target ${SKIA_LIBRARY})
```

## 代码集成

### 基本使用模式

Skia与Qt集成的基本模式是：
1. 在Qt Widget中创建Skia Surface
2. 使用Skia Canvas进行绘制
3. 将Skia Surface转换为QImage
4. 在Qt的paintEvent中绘制QImage

### 示例代码结构

```cpp
// SkiaWidget.h
class SkiaWidget : public QWidget {
    sk_sp<SkSurface> m_surface;
    SkCanvas* m_canvas;
    QImage m_image;
};

// SkiaWidget.cpp
void SkiaWidget::paintEvent(QPaintEvent* event) {
    // 1. 使用Skia绘制
    drawSkiaContent();
    
    // 2. 转换为QImage
    sk_sp<SkImage> image = m_surface->makeImageSnapshot();
    QImage qImage = convertToQImage(image);
    
    // 3. Qt绘制
    QPainter painter(this);
    painter.drawImage(0, 0, qImage);
}
```

## 编译和运行

### 使用CMake

```bash
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt
cmake --build .
./QtSkiaIntegration
```

### 使用qmake（可选）

创建 `QtSkiaIntegration.pro`：

```pro
QT += core widgets
CONFIG += c++17

TARGET = QtSkiaIntegration
SOURCES += main.cpp MainWindow.cpp SkiaWidget.cpp
HEADERS += MainWindow.h SkiaWidget.h

# Skia配置
SKIA_DIR = $$PWD/third_party/skia
SKIA_BUILD_DIR = $$PWD/build/skia

INCLUDEPATH += $$SKIA_DIR/include \
               $$SKIA_DIR/include/core \
               $$SKIA_DIR/include/gpu

LIBS += -L$$SKIA_BUILD_DIR -lskia
```

## 常见问题

### 1. 找不到Skia头文件

**解决方案**：
- 检查 `SKIA_INCLUDE_DIR` 是否正确设置
- 确保Skia源码或头文件在正确位置

### 2. 链接错误

**解决方案**：
- 确保Skia库已正确编译
- 检查库文件路径是否正确
- 在Linux上可能需要链接额外的系统库（pthread, dl, fontconfig等）

### 3. 运行时崩溃

**可能原因**：
- Skia版本与Qt版本不兼容
- 内存管理问题（确保使用智能指针）
- 线程安全问题

### 4. 性能问题

**优化建议**：
- 使用GPU加速（Skia支持OpenGL/Vulkan）
- 避免频繁创建Surface
- 使用双缓冲技术
- 只在需要时重绘

## 高级用法

### GPU加速

要启用GPU加速，需要：
1. 编译Skia时启用GPU支持
2. 创建GPU Surface：
```cpp
GrContextOptions options;
auto context = GrDirectContext::MakeGL(...);
auto surface = SkSurface::MakeRenderTarget(context, ...);
```

### 文本渲染

Skia文本渲染需要SkFont和SkTypeface：
```cpp
SkFont font;
font.setSize(24);
SkPaint paint;
paint.setColor(SK_ColorBLACK);
canvas->drawString("Hello Skia", x, y, font, paint);
```

## 参考资源

- [Skia官方文档](https://skia.org/docs/)
- [Skia GitHub](https://github.com/google/skia)
- [Qt文档](https://doc.qt.io/)

## 许可证

Skia使用BSD许可证，Qt使用LGPL或商业许可证。

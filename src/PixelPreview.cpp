#include "PixelPreview.h"
#include <QApplication>
#include <QCursor>
#include <QPainter>
#include <QPixmap>
#include <QScreen>
#include <QTimer>

// 全局常量
static const QSize  G_WindowSize(110, 100);     // 窗口尺寸
static const int    G_GrabInterval       = 25;  // 刷新频率
static const int    G_MagnificationTimes = 10;  // 放大倍数
static const double G_Split              = 0.7; // 分割
static const int    G_SizeOfMouseIcon    = 20;  // 鼠标图标大小


PixelPreview::PixelPreview(QWidget* parent) : QWidget(parent) {
    // 创建定时器用于定期更新取色器位置
    QTimer* timer = new QTimer();

    // 设置窗口
    setFixedSize(G_WindowSize); // 设置固定窗口大小
    setMouseTracking(true);     // 启用鼠标追踪

    // 开启定时任务
    timer->setInterval(G_GrabInterval); // 设置定时器间隔

    // 连接定时器超时信号,更新取色器位置
    connect(timer, &QTimer::timeout, [this, parent]() {
        // 计算取色器显示位置,默认显示在鼠标右上方
        QPoint show = QCursor::pos() + QPoint(G_SizeOfMouseIcon, -(G_WindowSize.height() + G_SizeOfMouseIcon));

        // 如果超出屏幕上边界,则显示在鼠标下方
        if (show.y() < 0) {
            show.setY(show.y() + G_WindowSize.height() + G_SizeOfMouseIcon);
        }

        // 如果超出屏幕右边界,则显示在鼠标左方
        if (show.x() + G_WindowSize.width() > parent->width()) {
            show.setX(show.x() - (G_WindowSize.height() + G_SizeOfMouseIcon));
        }

        move(show); // 移动取色器窗口到计算出的位置
        update();   // 触发重绘
    });
    timer->start(); // 启动定时器
}

QColor PixelPreview::getColor() const { return this->mColor; }

void PixelPreview::paintEvent(QPaintEvent* ev) {
    QPainter painter(this);

    // 截取鼠标周围区域的屏幕图像
    QScreen* screen    = QApplication::primaryScreen(); // 获取主屏幕
    QPoint   cursorPos = QCursor::pos();                // 获取当前鼠标位置
    // 计算需要截取的区域,以鼠标为中心
    QRect grabRect(
        cursorPos.x() - G_WindowSize.width() / G_MagnificationTimes / 2,            // 左上角x坐标
        cursorPos.y() - G_WindowSize.height() * G_Split / G_MagnificationTimes / 2, // 左上角y坐标
        G_WindowSize.width() / G_MagnificationTimes,                                // 截取宽度
        G_WindowSize.height() * G_Split / G_MagnificationTimes                      // 截取高度
    );
    // 截取指定区域的屏幕图像
    QPixmap grab = screen->grabWindow(0, grabRect.x(), grabRect.y(), grabRect.width(), grabRect.height());

    // 将截取的图像绘制到窗口上部区域,并放大显示
    painter.drawPixmap(0, 0, G_WindowSize.width(), G_WindowSize.height() * G_Split, grab);

    // 获取鼠标位置的像素颜色
    QPixmap pixmap = grab.copy(
        G_WindowSize.width() / G_MagnificationTimes / 2,
        G_WindowSize.height() * G_Split / G_MagnificationTimes / 2,
        1,
        1
    );                                     // 截图1个像素
    mColor = pixmap.toImage().pixel(0, 0); // 保存当前像素颜色

    // 根据背景颜色的明暗程度选择合适的画笔颜色
    if (mColor.red() > 100 && mColor.blue() > 100 && mColor.green() > 100) {
        painter.setPen(QColor(0, 0, 0)); // 背景较亮时使用黑色
    } else {
        painter.setPen(QColor(255, 255, 255)); // 背景较暗时使用白色
    }

    // 绘制窗口下部的颜色信息显示区域
    painter.fillRect(
        0,
        G_WindowSize.height() * G_Split,
        G_WindowSize.width() - 1,
        G_WindowSize.height() * (1 - G_Split),
        QColor(100, 100, 100)
    );
    painter.drawRect(0, 0, rect().width() - 1, rect().height() - 1); // 绘制窗口边框
    painter.fillRect(4, 74, 22, 22, mColor);                         // 绘制当前颜色预览框

    // 在放大区域中绘制像素选择框
    int   pixelSize = G_WindowSize.width() / G_MagnificationTimes;
    int   centerX   = G_WindowSize.width() / 2;
    int   centerY   = int(G_WindowSize.height() * G_Split / 2);
    QRect pixelRect(
        centerX - pixelSize / 2, // 左上角x
        centerY - pixelSize / 2, // 左上角y
        pixelSize,               // 宽度
        pixelSize                // 高度
    );
    painter.drawRect(pixelRect); // 绘制选择框

    // 显示RGB颜色值
    painter.setPen(QColor(255, 255, 255));
    painter.drawText(32, 82, "RGB"); // 绘制RGB标签
    // 显示当前像素的RGB值(考虑透明度进行修正)
    painter.drawText(
        32,
        95,
        QString().asprintf(
            "%d,%d,%d",
            (mColor.red() - 1) * 255 / 254,
            (mColor.green() - 1) * 255 / 254,
            (mColor.blue() - 1) * 255 / 254
        )
    );
}

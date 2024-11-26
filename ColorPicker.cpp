#include "ColorPicker.h"

#include <QPainter>
#include <QApplication>
#include <QScreen>

ColorPicker::ColorPicker(QWidget *parent)
    : QWidget{parent}
{
    this->mMouseDropper = new MouseDropper(this);

    // 获取主屏幕并设置窗口大小
    QScreen* screen = QApplication::primaryScreen();
    resize(screen->size());

    // 设置窗口无边框且置顶显示
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    // 设置窗口背景透明
    this->setAttribute(Qt::WA_TranslucentBackground);
    // 设置自定义鼠标光标, 偏移(0,19)
    this->setCursor(QCursor(QPixmap("./cursor.png"), 0, 19));
}

void ColorPicker::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(255, 255, 255, 1));
}

void ColorPicker::mousePressEvent(QMouseEvent *event) {
    colorSelect(mMouseDropper->getColor());
    this->close();
}

#pragma once

#include <QObject>
#include <QWidget>

#include "MouseDropper.h"

class ColorPicker : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPicker(QWidget *parent = nullptr);

private:
    MouseDropper* mMouseDropper;

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);

signals:
    void colorSelect(QColor color);
};

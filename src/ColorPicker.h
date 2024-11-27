#pragma once

#include <QObject>
#include <QWidget>

#include "PixelPreview.h"

class ColorPicker : public QWidget {
    Q_OBJECT
public:
    explicit ColorPicker(QWidget* parent = nullptr);

    PixelPreview* getPixelPreview();

private:
    PixelPreview* mPixelPreview;

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);

signals:
    void colorSelect(QColor color);
};

#pragma once
#include "qtmetamacros.h"
#include <QObject>
#include <QWidget>

class PixelPreview : public QWidget {
    Q_OBJECT
public:
    explicit PixelPreview(QWidget* parent);

    QColor getColor() const;

private:
    QColor mColor;

protected:
    void paintEvent(QPaintEvent* ev);

signals:
    void colorChanged(QColor const& color);
};

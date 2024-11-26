#pragma once
#include <QObject>
#include <QWidget>

class MouseDropper : public QWidget
{
    Q_OBJECT
public:
    explicit MouseDropper(QWidget* parent);

    QColor getColor() const;

private:
    QColor mColor;

protected:
    void paintEvent(QPaintEvent* ev);
};


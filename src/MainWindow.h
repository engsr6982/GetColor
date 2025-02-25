#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ColorPicker.h"
#include "qcolordialog.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    QColor        mColor;
    ColorPicker   mPicker;
    QColorDialog* mDialog;

protected:
    void paintEvent(QPaintEvent* ev);

private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H

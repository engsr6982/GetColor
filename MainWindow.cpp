#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <QPainter>
#include <QRgb>
#include <QClipboard>

// 封装
void CopyColor(QString const& color) {
    QClipboard* board = QApplication::clipboard();
    board->setText(color);
}
#define COPYDEF(bt, dt) connect(bt, &QPushButton::clicked, [this](){ CopyColor(dt); });


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 限制窗口大小
    int const& _height = this->height();
    int const& _width = this->width();
    this->setMinimumHeight(_height);
    this->setMaximumHeight(_height);
    this->setMinimumWidth(_width);
    this->setMaximumWidth(_width);

    this->mColor = QColor(255,255,255); // 默认白色

    // 链接事件
    connect(this->ui->mPickUpColor, &QPushButton::pressed, [this](){
        mPicker.show();
    });
    connect(&this->mPicker, &ColorPicker::colorSelect, [this](QColor ev) {
        this->mColor = ev;
        this->update();
    });

    // 拷贝功能
    COPYDEF(this->ui->mHEX_Copy, this->ui->mHEX->text());
    COPYDEF(this->ui->mHEXA_Copy, this->ui->mHEXA->text());
    COPYDEF(this->ui->mRGB_Copy, this->ui->mRGB->text());
    COPYDEF(this->ui->mRGBA_Copy, this->ui->mRGBA->text());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *ev) {
    // 绘制预览颜色（转换坐标系）
    QPainter p(this);
    p.translate(ui->mColorView->pos());
    p.fillRect(QRect(0, 0, ui->mColorView->width(), ui->mColorView->height()), mColor);

    // 输出
    this->ui->mRGB->setText(
        QString("%1,%2,%3")
            .arg(this->mColor.red())
            .arg(this->mColor.green())
            .arg(this->mColor.blue())
    );
    this->ui->mRGBA->setText(
        QString("%1,%2,%3,%4")
            .arg(this->mColor.red())
            .arg(this->mColor.green())
            .arg(this->mColor.blue())
            .arg(this->mColor.alphaF())
    );
    this->ui->mHEX->setText(this->mColor.name());
    this->ui->mHEXA->setText(this->mColor.name(QColor::HexArgb));
}

#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "ColorPicker.h"
#include "qcolordialog.h"
#include "qlabel.h"
#include "qlist.h"
#include "qobject.h"
#include "qpushbutton.h"

#include <QClipboard>
#include <QColorDialog>
#include <QPainter>
#include <QRgb>
#include <sstream>
#include <string>
#include <strstream>
#include <unordered_set>


// 封装
void CopyColor(QString const& color) {
    QClipboard* board = QApplication::clipboard();
    board->setText(color);
}
#define COPYDEF(bt, dt) connect(bt, &QPushButton::clicked, [this]() { CopyColor(dt); });


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 限制窗口大小
    int const& _height = this->height();
    int const& _width  = this->width();
    this->setMinimumHeight(_height);
    this->setMaximumHeight(_height);
    this->setMinimumWidth(_width);
    this->setMaximumWidth(_width);

    // 初始化成员变量
    {
        this->mColor = QColor(255, 255, 255); // 默认白色

        this->mDialog = new QColorDialog(this);
        this->mDialog->setWindowTitle("拾色器");
        this->mDialog->setMinimumWidth(this->width() + 40);
        this->mDialog->setMaximumWidth(this->width() + 40);

        QList<QPushButton*> btns = this->findChildren<QPushButton*>();
        for (auto& btn : btns) {
            if (btn->text() == "OK") {
                btn->setText("确定");
            } else if (btn->text() == "Cancel") {
                btn->setText("取消");
            } else if (btn->text() == "&Pick Screen Color") {
                btn->setText("拾取屏幕颜色");
            } else if (btn->text() == "&Add to Custom Colors") {
                btn->setText("添加到自定义颜色");
            }
        }

        QList<QLabel*> lbs = this->findChildren<QLabel*>();
        for (auto& lb : lbs) {
            QString str = lb->text();
            if (str == "&Basic colors") {
                lb->setText("基础颜色");
            } else if (str == "&Sat:") {
                lb->setText("饱和度:");
            } else if (str == "&Green:") {
                lb->setText("绿色:");
            } else if (str == "&Red:") {
                lb->setText("红色:");
            } else if (str == "&Val:") {
                lb->setText("亮度:");
            } else if (str == "&Custom colors") {
                lb->setText("自定义颜色");
            } else if (str == "Hu&e:") {
                lb->setText("色相:");
            } else if (str == "Bl&ue:") {
                lb->setText("蓝色:");
            } else if (str == "A&lpha channel:") {
                lb->setText("透明度:");
            } else if (str == "&HTML:") {
                lb->setText("HEX:");
            }
        }

        // 链接 mDialog 颜色更新事件
        connect(this->mDialog, &QColorDialog::currentColorChanged, [this](QColor ev) {
            this->mColor = ev;
            this->ui->mColorView->update();
        });
    }

    // 链接颜色更新事件
    connect(&this->mPicker, &ColorPicker::colorSelect, [this](QColor ev) {
        this->mColor = ev;
        this->ui->mColorView->update();
    });

    // 拷贝功能
    COPYDEF(this->ui->mHEX_Copy, this->ui->mHEX->text());
    COPYDEF(this->ui->mHEXA_Copy, this->ui->mHEXA->text());
    COPYDEF(this->ui->mRGB_Copy, this->ui->mRGB->text());
    COPYDEF(this->ui->mRGBA_Copy, this->ui->mRGBA->text());

    // 按钮
    connect(this->ui->mPickUpColor, &QPushButton::pressed, [this]() { mPicker.show(); });
    connect(this->ui->mOpenColorSelect, &QPushButton::pressed, [this]() { this->mDialog->show(); });
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::paintEvent(QPaintEvent* ev) {
    // 绘制预览颜色（转换坐标系）
    QPainter p(this);
    p.translate(ui->mColorView->pos());
    p.fillRect(QRect(0, 0, ui->mColorView->width(), ui->mColorView->height()), mColor);

    // 输出
    this->ui->mRGB->setText(
        QString("%1,%2,%3").arg(this->mColor.red()).arg(this->mColor.green()).arg(this->mColor.blue())
    );
    this->ui->mRGBA->setText(QString("%1,%2,%3,%4")
                                 .arg(this->mColor.red())
                                 .arg(this->mColor.green())
                                 .arg(this->mColor.blue())
                                 .arg(this->mColor.alphaF()));
    this->ui->mHEX->setText(this->mColor.name());
    this->ui->mHEXA->setText(this->mColor.name(QColor::HexArgb));
}

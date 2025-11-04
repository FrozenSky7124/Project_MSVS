#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
  * @brief GernerateQRCode
  * 生成二维码函数
  * @param text  二维码内容
  * @param qrPixmap  二维码像素图
  * @param scale 二维码缩放比例
  */
void MainWindow::GernerateQRCode(const QString &text, QPixmap &qrPixmap, int scale)
{
    if(text.isEmpty())
    {
        return;
    }

    //二维码数据
    QRcode *qrCode = nullptr;

    //这里二维码版本传入参数是2,实际上二维码生成后，它的版本是根据二维码内容来决定的
    qrCode = QRcode_encodeString(text.toStdString().c_str(), 2,
                                 QR_ECLEVEL_Q, QR_MODE_8, 1);
    if(nullptr == qrCode)
    {
        return;
    }

    int qrCode_Width = qrCode->width > 0 ? qrCode->width : 1;
    int width = scale * qrCode_Width;
    int height = scale * qrCode_Width;

    QImage image(width, height, QImage::Format_ARGB32_Premultiplied);

    QPainter mPainter(&image);
    QColor background(Qt::white);
    mPainter.setBrush(background);
    mPainter.setPen(Qt::NoPen);
    mPainter.drawRect(0, 0, width, height);
    QColor foreground(Qt::black);
    mPainter.setBrush(foreground);
    for(int y = 0; y < qrCode_Width; ++y)
    {
        for(int x = 0; x < qrCode_Width; ++x)
        {
            unsigned char character = qrCode->data[y * qrCode_Width + x];
            if(character & 0x01)
            {
                QRect rect(x * scale, y * scale, scale, scale);
                mPainter.drawRects(&rect, 1);
            }
        }
    }

    qrPixmap = QPixmap::fromImage(image);
    QRcode_free(qrCode);
}


void MainWindow::on_textEdit_textChanged()
{
    QString strText = ui->textEdit->toPlainText();
    QPixmap qrPixmap;
    int width = ui->imageLabel->width();
    int height = ui->imageLabel->height();
    GernerateQRCode(strText, qrPixmap, 2);
    qrPixmap = qrPixmap.scaled(QSize(width, height), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->imageLabel->setPixmap(qrPixmap);
    //qrPixmap.save("qrencode.jpg",nullptr,-1);//这里直接就保存为图片
}


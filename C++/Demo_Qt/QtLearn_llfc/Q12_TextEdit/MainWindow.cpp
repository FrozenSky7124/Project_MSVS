#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QTextFrame>
#include <QDebug>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTextDocument *doc = ui->textEdit->document();
    QTextFrame *root_frame = doc->rootFrame();
    QTextFrameFormat format;
    format.setBorder(3);
    format.setBorderBrush(Qt::blue);
    root_frame->setFrameFormat(format);

    QTextFrameFormat frameFormat;
    frameFormat.setBackground(Qt::lightGray);
    frameFormat.setMargin(10);
    frameFormat.setPadding(5);
    frameFormat.setBorder(2);
    frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Dashed);
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.insertFrame(frameFormat);
    ui->textEdit->insertPlainText("Inner Text!\n");
    ui->textEdit->insertPlainText("Hello! Inner Text!\n");

    QAction *action_frame = new QAction("Frame", this);
    QObject::connect(action_frame, &QAction::triggered, this, &MainWindow::showTextFrame);
    ui->toolBar->addAction(action_frame);

    QAction *action_block = new QAction("Block", this);
    QObject::connect(action_block, &QAction::triggered, this, &MainWindow::showTextBlock);
    ui->toolBar->addAction(action_block);

    QAction *action_font = new QAction("Font", this);
    action_font->setCheckable(true);
    QObject::connect(action_font, &QAction::toggled, this, &MainWindow::setTextFont);
    ui->toolBar->addAction(action_font);

    QAction *action_textTable = new QAction(tr("Table"), this);
    QAction *action_textList  = new QAction(tr("List"),  this);
    QAction *action_textImage = new QAction(tr("Image"), this);
    QObject::connect(action_textTable, &QAction::triggered, this, &MainWindow::insertTable);
    QObject::connect(action_textList , &QAction::triggered, this, &MainWindow::insertList );
    QObject::connect(action_textImage, &QAction::triggered, this, &MainWindow::insertImage);
    ui->toolBar->addAction(action_textTable);
    ui->toolBar->addAction(action_textList );
    ui->toolBar->addAction(action_textImage);

    QAction *action_textFind = new QAction(tr("Find"), this);
    QObject::connect(action_textFind, &QAction::triggered, this, &MainWindow::textFind);
    ui->toolBar->addAction(action_textFind);

    pFindDlg = new QDialog(this);
    pFindDlg->setWindowTitle(tr("Find"));
    pLineEdit = new QLineEdit(pFindDlg);
    QPushButton *pBtn = new QPushButton(pFindDlg);
    pBtn->setText(tr("FindNext"));
    QObject::connect(pBtn, &QPushButton::clicked, this, &MainWindow::textFindNext);
    QVBoxLayout *pVLayout = new QVBoxLayout();
    pVLayout->addWidget(pLineEdit);
    pVLayout->addWidget(pBtn);
    pFindDlg->setLayout(pVLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showTextFrame()
{
    QTextDocument *doc = ui->textEdit->document();
    QTextFrame *rootFrame = doc->rootFrame();
    for (auto iter = rootFrame->begin(); iter != rootFrame->end(); iter++)
    {
        auto curFrame = iter.currentFrame();
        auto curBlock = iter.currentBlock();
        if (curFrame)
        {
            qDebug() << "CurNode is Frame.";
        }
        else if (curBlock.isValid())
        {
            qDebug() << "CurNode is Text Block : " << curBlock.text();
        }
    }
}

void MainWindow::showTextBlock()
{
    QTextDocument *doc = ui->textEdit->document();
    QTextBlock block = doc->firstBlock();
    for (int i = 0; i < doc->blockCount(); i++)
    {
        qDebug() << tr("BLOCK %1, No.%2, Length:%3, Content:%4")
                    .arg(i)
                    .arg(block.firstLineNumber())
                    .arg(block.length())
                    .arg(block.text());
        block = block.next();
    }
}

void MainWindow::setTextFont(bool checked)
{
    if (checked)
    {
        QTextCursor cursor = ui->textEdit->textCursor();
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(Qt::AlignCenter);
        cursor.insertBlock(blockFormat);
        QTextCharFormat charFormat;
        charFormat.setBackground(Qt::lightGray);
        charFormat.setForeground(Qt::blue);
        charFormat.setFont(QFont(tr("宋体"), 12, QFont::Bold, true));
        charFormat.setFontUnderline(true);
        cursor.setCharFormat(charFormat);
        //ui->textEdit->insertPlainText("AlignCenter!");
        cursor.insertText(tr("测试一下！"));
    }
    else
    {
        QTextCursor cursor = ui->textEdit->textCursor();
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(Qt::AlignLeft);
        cursor.insertBlock(blockFormat);
        QTextCharFormat charFormat;
        cursor.setCharFormat(charFormat);
        cursor.insertText(tr("测试一下！"));
    }
}

void MainWindow::insertTable()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextTableFormat tableFormat;
    tableFormat.setCellSpacing(2);
    tableFormat.setCellPadding(10);
    cursor.insertTable(3, 2, tableFormat);
}

void MainWindow::insertList()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextListFormat listFormat;
    listFormat.setStyle(QTextListFormat::ListDecimal);
    cursor.insertList(listFormat);
}

void MainWindow::insertImage()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setName(":/Amiya_carrot.gif");
    cursor.insertImage(imageFormat);
}

void MainWindow::textFind()
{
    pFindDlg->show();
}

void MainWindow::textFindNext()
{
    QString str = pLineEdit->text();
    bool isFind = ui->textEdit->find(str, QTextDocument::FindBackward);
    if (isFind)
    {
        qDebug() << tr("Find! Row: %1 Col: %2").arg(ui->textEdit->textCursor().blockNumber())
                                                .arg(ui->textEdit->textCursor().columnNumber());
    }
}
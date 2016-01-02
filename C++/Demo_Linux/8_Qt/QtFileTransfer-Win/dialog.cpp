#include "dialog.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QByteArray>
#include <QHostAddress>
#include <QFileDialog>

#define BUFSIZE 262144  //256k

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    tcpsocket = new QTcpSocket(this);

    setWindowTitle(tr("tcp文件发送客户端"));//设置窗口标题
    label1 = new QLabel;
    label1->setText(tr("请输入目的IP地址"));
    label2 = new QLabel;
    label2->setText(tr("请输入目的端口号"));

    edit1 = new QLineEdit;
    edit2 = new QLineEdit;

    btn1 = new QPushButton;
    btn1->setText(tr("发送文件"));//设置btn1标题

    QGridLayout *layout1 = new QGridLayout(this);

    layout1->addWidget(label1, 0, 0);//加入label1控件
    layout1->addWidget(edit1, 0, 1);//加入edit1控件
    layout1->addWidget(label2, 1, 0);//加入label2控件
    layout1->addWidget(edit2, 1, 1);//加入edit2控件
    layout1->addWidget(btn1, 2, 0);//加入btn1控件

    connect(btn1, SIGNAL(clicked()), this, SLOT(on_clicked1()));
    connect(tcpsocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
}

Dialog::~Dialog()
{

}


void getfilename(const char *filename, char *name)//从完整路径名中解析出文件名称，例如：/home/test/abc.txt,解析完成后为abc.txt
{
    int len = strlen(filename);
    int i;
    for (i = (len - 1); i >= 0; i--)
    {
        if ((filename[i] == '\\') || (filename[i] == '/'))//为了兼容linux与windows的路径，同时解析/与\分隔的路径名称
        {
            break;
        }
    }
    strcpy(name, &filename[i + 1]);
    return;
}

void Dialog::on_clicked1()
{
    filename = QFileDialog::getOpenFileName(this, tr("打开文件"));
    if (!filename.isEmpty())
    {

        tcpsocket->close();
        QHostAddress serverIP;
        serverIP.setAddress(edit1->text());
        tcpsocket->connectToHost(serverIP , edit2->text().toInt());//连接到server端

        char buf[1024];
        memset(buf, 0, sizeof(buf));

        //从完整路径名中解析出文件名称，例如：/home/test/abc.txt,解析完成后为abc.txt
        getfilename(filename.toStdString().data(), buf);
        tcpsocket->write(buf, strlen(buf));//将文件名称发送到server端
    }
}

void Dialog::dataReceived()//接收socket消息的槽函数
{
    char *buf = (char *)malloc(BUFSIZE);//申请一个缓冲区，存放接收到的文件内容

    while(tcpsocket->bytesAvailable() > 0)
    {
        memset(buf, 0, BUFSIZE);
        tcpsocket->read(buf, BUFSIZE);
    }

    if (strncmp(buf, "OK", 2) == 0)//server端回复同意接收文件
    {
        FILE *fd = fopen(filename.toStdString().data(), "rb");//以只读方式打开filename指定的文件
        if (fd)//如果文件成功打开
        {
            while (1)
            {
                memset(buf, 0, BUFSIZE);
                size_t rc = fread(buf, 1, BUFSIZE, fd);//循环读取文件，直到读到文件尾，循环break
                if (rc <= 0)
                {
                    break;//文件尾，循环break
                } else
                {
                    tcpsocket->write(buf, rc);//将从文件中读到的数据，通过socket发送到server端，其中rc为从文件中读到的数据大小
                    if (rc <= 0)//如果发送失败，代表TCP连接出错，循环break
                    {
                        break;
                    }
                }
            }
        }
        fclose(fd);
    }

    tcpsocket->close();//文件发送完成后关闭socket
    free(buf);
}




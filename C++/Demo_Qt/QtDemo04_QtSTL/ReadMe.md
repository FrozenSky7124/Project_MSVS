**QDateTime**

```cpp
QLabel *datalabel = new QLabel();
QDateTime *datatime = new QDateTime(QDateTime::currentDateTime());
datalabel->setText(datatime->date().toString());
datalabel->show();
```

**QTimer**

```cpp
QTimer *time_clock = new QTimer(parent);
connect(time_clock, SIGNAL(timeout()), this, SLOT(slottimedone()));
//开启定时器
time_clock->start(1000); //每隔1000毫秒触发
time_clock->stop();
time_clock->setSingleShot(true);
```

**QWidget**

```cpp
QWidget::setWindowFlags(); //设置窗口标识
QWidget::setParent(); //设置父窗口
QWidget(QWidget *parent = 0; Qt::WindowFlags f = 0); //QWidget Construct Function
```

**Qt正则表达式 QRegExp类**

```cpp
"[A-Za-z_]+[A-Za-z_0-9]*"
//表示要求以字母或下划线开头，后面可以是字母数字和下划线的标识符
```


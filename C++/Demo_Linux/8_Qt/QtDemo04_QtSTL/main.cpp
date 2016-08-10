#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //QList
    QList<int> list;
    for (int j = 0; j < 10; j++)
    {
        //list.push_back(j);
        list.insert(list.end(), j);
    }
    QList<int>::iterator i;
    for (i = list.begin(); i != list.end(); i++)
    {
        qDebug() << (*i);
        *i = (*i) * 10;
    }
    QList<int>::const_iterator ci;
    for (ci = list.constBegin(); ci != list.constEnd(); ci++)
        qDebug() << *ci;

    //QMap
    QMap<QString, QString> map;
    map.insert("Beijing", "111");
    map.insert("Shanghai", "021");
    map.insert("Jinan", "0531");
    QMap<QString, QString>::const_iterator mapi;
    for (mapi = map.constBegin(); mapi != map.constEnd(); mapi++)
        qDebug() << "   " << mapi.key() << "   " << mapi.value();
    QMap<QString, QString>::iterator mi;
    mi = map.find("Beijing");
    if (mi != map.end())
        mi.value() = "010";
    qDebug() << "    ";
    QMap<QString, QString>::const_iterator modi;
    for (modi = map.constBegin(); modi != map.constEnd(); modi++)
        qDebug() << "   " << modi.key() << "   " << modi.value();
    return 0;
    return a.exec();
}


#include <QCoreApplication>
#include <QDebug>

int workQList()
{
    QList<QString> list;
    list << "One" << "Two" << "Three" << "Four";
    QString str0 = list.at(0);
    QString str1 = list[1];
    qDebug() << "0: " << str0 << endl << "1: " << str1;

    QListIterator<QString> i(list);
    while (i.hasNext())
        qDebug() << i.next();

    QListIterator<QString> m(list);
    m.toBack();
    while (m.hasPrevious())
        qDebug() << m.previous();

    QList<QString>::const_iterator sItor;
    for (sItor = list.constBegin(); sItor != list.constEnd(); sItor++)
        qDebug() << *sItor;

    QList<int> ilist;
    ilist << 1 << 2 << 3 << 4 << 5;
    QMutableListIterator<int> itor(ilist);
    while (itor.hasNext())
    {
        if (itor.next() % 2 == 0) itor.remove();
    }
    itor.toFront();
    while (itor.hasNext())
        qDebug() << itor.next();

    return 0;
}

int workQMap()
{
    QMap<QString, QString> mapRoleSkill;
    mapRoleSkill.insert("刻晴", "天街巡游");
    mapRoleSkill.insert("申鹤", "仰灵威召将役咒");
    mapRoleSkill.insert("甘雨", "降众天华");
    QMapIterator<QString, QString> mapIt(mapRoleSkill);
    while (mapIt.hasNext())
    {
        qDebug() << mapIt.peekNext().key() << mapIt.peekNext().value();
        mapIt.next();
    }

    QMap<QString, QString>::const_iterator sitt;
    for (sitt = mapRoleSkill.cbegin(); sitt != mapRoleSkill.cend(); sitt++)
        qDebug() << sitt.key() << ':' << sitt.value();

    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Work
    //workQList();
    workQMap();

    return a.exec();
}

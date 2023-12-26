#ifndef QPERSON_H
#define QPERSON_H

#include <QObject>

class QPerson : public QObject
{
    Q_OBJECT

    Q_CLASSINFO("author", "FrozenSky");
    Q_CLASSINFO("company", "Avalon");
    Q_CLASSINFO("version", "1.0.0");
    Q_PROPERTY(int age READ age WRITE setAge NOTIFY ageChanged)
    Q_PROPERTY(QString name MEMBER m_name)
    Q_PROPERTY(int score MEMBER m_score)

private:
    int m_age;
    QString m_name;
    int m_score = 79;

public:
    explicit QPerson(QString fName, QObject *parent = nullptr);
    int age();
    void setAge(int value);
    void incAge();

signals:
    void ageChanged(int value);
};

#endif // QPERSON_H

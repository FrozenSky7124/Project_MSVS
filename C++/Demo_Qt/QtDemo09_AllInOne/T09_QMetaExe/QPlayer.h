#ifndef QPLAYER_H
#define QPLAYER_H

#include <QObject>

class QPlayer : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("Author",  "FrozenSky")
    Q_CLASSINFO("Updated", "2021-12-17")

    Q_PROPERTY(unsigned int level READ level WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY(unsigned int hp MEMBER m_hp)
    Q_PROPERTY(QString name MEMBER m_name)

private:
    QString m_name;
    unsigned int m_level = 0;
    unsigned int m_hp;

public:
    explicit QPlayer(QString name, QObject *parent = nullptr);
    unsigned int level();
    void setLevel(unsigned int value);
    void PlayerLevelUp();

signals:
    void levelChanged(unsigned int value);
};

#endif // QPLAYER_H

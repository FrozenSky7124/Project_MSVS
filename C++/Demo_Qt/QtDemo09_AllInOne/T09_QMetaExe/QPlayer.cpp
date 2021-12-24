#include "QPlayer.h"

QPlayer::QPlayer(QString name, QObject *parent) : QObject(parent)
{
    m_name = name;
}

unsigned int QPlayer::level()
{
    return m_level;
}

void QPlayer::setLevel(unsigned int value)
{
    m_level = value;
    emit levelChanged(m_level);
}

void QPlayer::PlayerLevelUp()
{
    m_level++;
    emit levelChanged(m_level);
}

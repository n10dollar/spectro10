#ifndef STREAMMANAGER_H
#define STREAMMANAGER_H

#include <QObject>

class StreamManager : public QObject
{
    Q_OBJECT
public:
    explicit StreamManager(QObject *parent = nullptr);

signals:
};

#endif // STREAMMANAGER_H

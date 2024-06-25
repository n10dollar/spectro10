#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include <QWidget>

class Oscilloscope : public QWidget
{
    Q_OBJECT
public:
    explicit Oscilloscope(QWidget *parent = nullptr);

signals:
};

#endif // OSCILLOSCOPE_H

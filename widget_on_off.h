#ifndef MYONOFFWIDGET_H
#define MYONOFFWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

class onOffWidget : public QWidget
{
    Q_OBJECT

public:
    onOffWidget(QWidget *parent, bool* isOn);

private slots:
    void toggleState();

private:
    QPushButton *button;
    QLabel *indicator;
    bool* isOn_;

    QPixmap createRectPixmap(QColor color, int size);
};

#endif // MYONOFFWIDGET_H

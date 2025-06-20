#pragma once

#include <QWidget>

class QLabel;
class QPushButton;

class incrDecrWidget : public QWidget
{
    Q_OBJECT

public:
    explicit incrDecrWidget(QWidget *parent,double* valueToChange,float coefMult);


private slots:
    void increaseSize();
    void decreaseSize();

private:
    void updateLabel();

    double* valueToChange_;
    float coefMult_;
    QLabel *m_label;
    QPushButton *m_buttonPlus;
    QPushButton *m_buttonMinus;
};



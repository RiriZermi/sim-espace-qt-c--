#include "widget_incr_decr.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

incrDecrWidget::incrDecrWidget(QWidget *parent, double* valueToChange,float coefMult)
    : QWidget(parent),valueToChange_(valueToChange),coefMult_(coefMult)
{
    m_label = new QLabel(QString::number(*valueToChange), this);
    m_buttonPlus = new QPushButton("+", this);
    m_buttonMinus = new QPushButton("−", this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_buttonMinus);
    layout->addWidget(m_label);
    layout->addWidget(m_buttonPlus);

    connect(m_buttonPlus, &QPushButton::clicked, this, &incrDecrWidget::increaseSize);
    connect(m_buttonMinus, &QPushButton::clicked, this, &incrDecrWidget::decreaseSize);
}

void incrDecrWidget::increaseSize()
{
    *valueToChange_ *= coefMult_;
    updateLabel();
}

void incrDecrWidget::decreaseSize()
{
    *valueToChange_ /= coefMult_;
    updateLabel();
}

void incrDecrWidget::updateLabel()
{
    m_label->setText(QString::number(*valueToChange_));
}

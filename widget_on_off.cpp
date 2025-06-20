#include "widget_on_off.h"
#include <QPainter>

onOffWidget::onOffWidget(QWidget *parent,bool* isOn)
    : QWidget(parent), isOn_(isOn)
{
    button = new QPushButton("OFF", this);
    indicator = new QLabel(this);
    indicator->setPixmap(createRectPixmap(Qt::green, 20));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(button);
    layout->addWidget(indicator);

    connect(button, &QPushButton::clicked, this, &onOffWidget::toggleState);
}

void onOffWidget::toggleState()
{
    *isOn_ = !(*isOn_);

    if (*isOn_) {
        button->setText("OFF");  
    } else {
        button->setText("ON");   
    }

    QColor color = *isOn_ ? Qt::green : Qt::red;
    indicator->setPixmap(createRectPixmap(color, 20));
    
}

QPixmap onOffWidget::createRectPixmap(QColor color, int size)
{
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(color));
    painter.setPen(Qt::NoPen);
    painter.drawRect(0,0,size,size);
    return pixmap;
}

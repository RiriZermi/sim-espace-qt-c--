#include "AstreDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

AstreDialog::AstreDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Configurer l'objet");

    xEdit = new QLineEdit(this);
    yEdit = new QLineEdit(this);
    vxEdit = new QLineEdit(this);
    vyEdit = new QLineEdit(this);
    masseEdit = new QLineEdit(this);
    radiusEdit = new QLineEdit(this);
    nameEdit = new QLineEdit(this);
    redEdit = new QLineEdit(this);
    greenEdit = new QLineEdit(this);
    blueEdit = new QLineEdit(this);


    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Nom :"));
    layout->addWidget(nameEdit);

    layout->addWidget(new QLabel("Position X (m) :"));
    layout->addWidget(xEdit);
    layout->addWidget(new QLabel("Position Y (m) :"));
    layout->addWidget(yEdit);

    layout->addWidget(new QLabel("Vitesse X (m/s):"));
    layout->addWidget(vxEdit);
    layout->addWidget(new QLabel("Vitesse Y(m/s) :"));
    layout->addWidget(vyEdit);

    layout->addWidget(new QLabel("Radius (m)"));
    layout->addWidget(radiusEdit);

    layout->addWidget(new QLabel("Masse (kg) :"));
    layout->addWidget(masseEdit);

    QHBoxLayout* colorNameLayout = new QHBoxLayout();
    colorNameLayout->addWidget(new QLabel("red (normalized)"));
    colorNameLayout->addWidget(new QLabel("green (normalized)"));
    colorNameLayout->addWidget(new QLabel("blue (normalized)"));
    layout -> addLayout(colorNameLayout);

    QHBoxLayout* colorValuesLayout = new QHBoxLayout();
    colorValuesLayout->addWidget(redEdit);
    colorValuesLayout->addWidget(greenEdit);
    colorValuesLayout->addWidget(blueEdit);
    layout->addLayout(colorValuesLayout);
    
    

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("OK");
    QPushButton* cancelButton = new QPushButton("Annuler");

    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &AstreDialog::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &AstreDialog::reject);
}

void AstreDialog::onAccept() {
    name_ = nameEdit->text().toStdString();
    x_ = xEdit->text().toDouble();
    y_ = yEdit->text().toDouble();
    vx_ = vxEdit->text().toDouble();
    vy_ = vyEdit->text().toDouble();
    radius_ = radiusEdit->text().toDouble();
    m_ = masseEdit->text().toDouble();
    r_ = redEdit->text().toDouble();
    g_ = greenEdit->text().toDouble();
    b_ = blueEdit->text().toDouble();

    accept();
}

double AstreDialog::getX() const { return x_; }
double AstreDialog::getY() const { return y_; }
double AstreDialog::getVx() const { return vx_; }
double AstreDialog::getVy() const { return vy_; }
double AstreDialog::getRadius() const { return radius_; }
double AstreDialog::getM() const { return m_;}
string AstreDialog::getName() const { return name_; }
double AstreDialog::getR() const {return r_;}
double AstreDialog::getB() const {return b_;}
double AstreDialog::getG() const {return g_;}

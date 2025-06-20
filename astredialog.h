#ifndef AstreDialog_H
#define AstreDialog_H

#include <QDialog>
#include <string>

class QLineEdit;
class QLabel;
class QPushButton;

using namespace std;

class AstreDialog : public QDialog {
    Q_OBJECT

public:
    explicit AstreDialog(QWidget* parent = nullptr);

    // Accesseurs
    double getX() const;
    double getY() const;
    double getVx() const;
    double getVy() const;
    double getM() const;
    double getRadius() const;
    double getR() const;
    double getG() const;
    double getB() const;
    string getName() const;

private slots:
    void onAccept();

private:
    QLineEdit* xEdit;
    QLineEdit* yEdit;
    QLineEdit* vxEdit;
    QLineEdit* vyEdit;
    QLineEdit* nameEdit;
    QLineEdit* masseEdit;
    QLineEdit* radiusEdit;
    QLineEdit* redEdit;
    QLineEdit* greenEdit;
    QLineEdit* blueEdit;

    double x_,y_,vx_,vy_,radius_,m_;
    float r_,g_,b_;
    string name_;
};

#endif // AstreDialog_H

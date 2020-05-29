#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void onbutton();
    void onbutton1();
    void onbutton2();
    void onbutton3();
    void onbutton4();
private:
    QTextEdit *m_edit;
    QPushButton *m_button;
    QLabel *m_la;
    QComboBox *m_box;
    int isHex;
    QTextEdit *v,*v1,*v2;
    QComboBox *m_box1;
    QPushButton *m_button1;
    QPushButton *m_button2,*m_button3;
    QPushButton *m_button4;
};
#endif // WIDGET_H

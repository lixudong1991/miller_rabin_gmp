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
private:
    QTextEdit *m_edit;
    QPushButton *m_button;
    QLabel *m_la;
    QComboBox *m_box;
    int isHex;
};
#endif // WIDGET_H

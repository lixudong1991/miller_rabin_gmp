#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "gmp.h"
typedef unsigned long long ui64;
#define MAX_BIT_INDEX 1023
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    setFixedSize(1000,600);
    QVBoxLayout *la=new QVBoxLayout(this);
    m_edit=new QTextEdit();
    m_la=new QLabel();
    m_button=new QPushButton(("测试"));
    m_box=new QComboBox();
    QHBoxLayout *lb=new QHBoxLayout();
    lb->addWidget(m_la);
    lb->addStretch(8);
    lb->addWidget(m_box);
    lb->addStretch(2);
    lb->addWidget(m_button);
    lb->setMargin(20);
    QHBoxLayout *lc=new QHBoxLayout();
    lc->addWidget(m_edit);
    la->addLayout(lc);
    la->addLayout(lb);
    la->setMargin(20);
    isHex=0;
    m_box->addItem("十进制");
    m_box->addItem("十六进制");
    m_la->setFixedSize(500,40);
    m_box->setFixedSize(100,40);
    connect(m_box, QOverload<int>::of(&QComboBox::currentIndexChanged),
          [=](int index){isHex=index;});
    connect(m_button,SIGNAL(clicked(bool)),this,SLOT(onbutton()));
}

Widget::~Widget()
{
}





void modular_exponentiation(mpz_t* a, mpz_t* b, mpz_t* n, mpz_t* d)
{
    mpz_init_set_ui(*d, 1);
    int i = MAX_BIT_INDEX;
    for (; i >= 0; i--)
    {
        if (mpz_tstbit(*b, i))
            break;
    }
    for (; i >= 0; i--)
    {
        mpz_mul(*d, *d, *d);
        mpz_mod(*d, *d, *n);
        if (mpz_tstbit(*b, i))
        {
            // d = (d * a) % n;
            mpz_mul(*d, *d, *a);
            mpz_mod(*d, *d, *n);
        }
    }
}
static int witness(mpz_t* a, mpz_t* n)
{
    ui64 t = 1,ret=0;
    mpz_t u, x, y, tem;
    mpz_init(u);
    mpz_init(x);
    mpz_init(y);
    mpz_init(tem);
    mpz_sub_ui(u, *n, 1);
    mpz_div_2exp(u, u, 1);
    while (!mpz_tstbit(u, 0))
    {
        mpz_div_2exp(u, u, 1);
        t++;
    }
    modular_exponentiation(a, &u, n, &x);
    mpz_init_set_ui(y, 1);
    mpz_sub_ui(tem, *n, 1);
    for (ui64 i = 1; i <= t; i++)
    {
        mpz_mul(y, x, x);
        mpz_mod(y, y, *n);
        if (mpz_cmp_ui(y, 1)==0 && (mpz_cmp_ui(x, 1)!=0) && (mpz_cmp(x, tem) != 0)) {
            mpz_clear(u);
            mpz_clear(x);
            mpz_clear(y);
            mpz_clear(tem);
            return 1;
        }

        mpz_set(x, y);
    }
    if (mpz_cmp_ui(y , 1)!=0)
        ret= 1;
    mpz_clear(u);
    mpz_clear(x);
    mpz_clear(y);
    mpz_clear(tem);
    return ret;
}
int miller_rabin(mpz_t *n, ui64 s)
{
    if (mpz_cmp_ui(*n,2)==0)
        return 1;
    if (mpz_cmp_ui(*n, 2) < 0 || !mpz_tstbit(*n, 0))
        return 0;
    clock_t time = clock();
    int ret=1;
    gmp_randstate_t grt;
    gmp_randinit_default(grt);
    gmp_randseed_ui(grt, time);
    mpz_t key,maxr;
    mpz_init(key);
    mpz_init(maxr);
    mpz_sub_ui(maxr, *n, 1);
    for (ui64 j = 1; j <= s; j++)
    {
        //ui64 a = rand64(1,n-1);
        mpz_urandomm(key, grt, maxr);
        mpz_add_ui(key,key,1);
        if (witness(&key, n))
            ret=0;
    }
    mpz_clear(key);
    mpz_clear(maxr);
    return ret;
}

void Widget::onbutton()
{
    QString text=m_edit->toPlainText();
    if(text.isEmpty())
      return;
    mpz_t v;
    mpz_init(v);
    if(isHex)
    {
        mpz_set_str(v,text.toUtf8().data(),16);
    }else {
        mpz_set_str(v,text.toUtf8().data(),10);
    }
    if (miller_rabin(&v,10))
        m_la->setText("是素数");
    else
        m_la->setText("不是素数");
    mpz_clear(v);
}


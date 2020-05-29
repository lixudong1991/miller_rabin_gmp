#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include "gmp.h"
typedef unsigned long long ui64;
#define MAX_BIT_INDEX 1023

extern "C" __declspec(dllimport) int getNp(int argc,char **argv);
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    setFixedSize(800,400);
    QVBoxLayout *la=new QVBoxLayout(this);
    m_edit=new QTextEdit();
    m_la=new QLabel();
    m_button=new QPushButton(("测试"));
    m_button2=new QPushButton("下一个素数");
    m_button3=new QPushButton("上一个素数");
    m_button4=new QPushButton("进制转换");
    m_box=new QComboBox();
    QHBoxLayout *lb=new QHBoxLayout();
    lb->addWidget(m_la);
    lb->addStretch();
    lb->addWidget(m_box);
    lb->addWidget(m_button);
    lb->addWidget(m_button3);
    lb->addWidget(m_button2);
    lb->addWidget(m_button4);
    lb->setMargin(20);
    QHBoxLayout *lc=new QHBoxLayout();
    lc->addWidget(m_edit);
    la->addLayout(lc);
    la->addLayout(lb);

    isHex=0;
    m_box->addItem("十进制");
    m_box->addItem("十六进制");
    m_la->setFixedSize(500,40);
    m_box->setFixedSize(80,30);


    v=new QTextEdit();
    v1=new QTextEdit();
    v2=new QTextEdit();
    m_box1=new QComboBox();
    m_button1=new QPushButton(("计算"));
    m_box1->addItem("+");
    m_box1->addItem("-");
    m_box1->addItem("*");
    m_box1->addItem("/");
    m_box1->addItem("%");
    m_box1->addItem("pow");
    m_box1->addItem("sqrt");
    m_box1->addItem("逆元");
    QHBoxLayout *ld=new QHBoxLayout();
    ld->addWidget(v);
    ld->addWidget(m_box1);
    ld->addWidget(v1);
    ld->addWidget(new QLabel("="));
    ld->addWidget(v2);
    ld->addWidget(m_button1);
    la->addLayout(ld);
    la->setMargin(20);
    connect(m_box, QOverload<int>::of(&QComboBox::currentIndexChanged),
          [=](int index){isHex=index;});
    connect(m_button,SIGNAL(clicked(bool)),this,SLOT(onbutton()));
    connect(m_button1,SIGNAL(clicked(bool)),this,SLOT(onbutton1()));
    connect(m_button2,SIGNAL(clicked(bool)),this,SLOT(onbutton2()));
    connect(m_button3,SIGNAL(clicked(bool)),this,SLOT(onbutton3()));
    connect(m_button4,SIGNAL(clicked(bool)),this,SLOT(onbutton4()));

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
void createArgab()
{
    clock_t time = clock();
    gmp_randstate_t grt;
    gmp_randinit_default(grt);
    gmp_randseed_ui(grt, time);
    mpz_t a,b,maxr,c,d;
    mpz_init(a);
    mpz_init(b);
    mpz_init(c);
    mpz_init(d);
    mpz_init_set_str(maxr,"fffffffeffffffffffffffffffffffffffffffff000000010000000000007061",16);
 k: do
    {
         mpz_urandomm(a, grt, maxr);
         mpz_urandomm(b, grt, maxr);
         mpz_pow_ui(c, a, 3);
         mpz_mul_ui(c,c,4);
         mpz_pow_ui(d, b, 2);
         mpz_mul_ui(d,d,27);
         mpz_add(c,c,d);
         mpz_mod(c,c,maxr);
         if(mpz_cmp_ui(c,0)!=0)
            break;
    }while(1);
    qDebug()<<"p=\n"<<mpz_get_str(NULL,10,maxr);
    qDebug()<<"a=\n"<<mpz_get_str(NULL,10,a);
    qDebug()<<"b=\n"<<mpz_get_str(NULL,10,b);

    char* arg[5]={"sea",mpz_get_str(NULL,10,a),mpz_get_str(NULL,10,b),"-i","test256.pol"};
    if(getNp(5,arg)!=1)
        goto k;


    mpz_clear(maxr);
    mpz_clear(c);
    mpz_clear(d);
    mpz_clear(a);
    mpz_clear(b);
}

int getSqrt_mod(mpz_t g,mpz_t p,mpz_t *ret)
{
    mpz_t tmp,y,tmp1;
    int r=0;
    mpz_init(tmp);
    mpz_init(tmp1);
    mpz_init_set_ui(y,0);
    mpz_mod_ui(tmp,p,4);
    if(mpz_cmp_ui(tmp,3)==0)
    {
        mpz_sub_ui(tmp,p,3);
        mpz_div_ui(tmp,tmp,4);
        mpz_add_ui(tmp,tmp,1);
        mpz_powm(y,g,tmp,p);
        mpz_pow_ui(tmp,y,2);
        mpz_mod(tmp,tmp,p);
        if(mpz_cmp(tmp,g)==0)
        {
            mpz_set(*ret,y);
            r=1;
            goto re;
        }
    }
    mpz_mod_ui(tmp,p,8);
    if(mpz_cmp_ui(tmp,5)==0)
    {
        mpz_sub_ui(tmp,p,5);
        mpz_div_ui(tmp,tmp,8);
        mpz_mul_ui(y,tmp,2);
        mpz_add_ui(y,y,1);
        mpz_powm(y,g,y,p);
        if(mpz_cmp_ui(y,1)==0)
        {
            mpz_add_ui(tmp,tmp,1);
            mpz_powm(y,g,tmp,p);
            mpz_set(*ret,y);
            r=1;
            goto re;
        }
        mpz_sub_ui(tmp1,p,1);
        if(mpz_cmp(y,tmp1)==0)
        {
            mpz_mul_ui(y,g,4);
            mpz_powm(y,y,tmp,p);
            mpz_add_ui(tmp,tmp,1);
            mpz_powm(y,g,tmp,p);
            mpz_mul_ui(y,y,2);
            mpz_mul(y,y,g);
            mpz_mul(y,y,p);
            mpz_set(*ret,y);
            r=1;
        }
    }
re: mpz_clear(y);
    mpz_clear(tmp);
    mpz_clear(tmp1);
    return r;
}

void createXY()
{
    clock_t time = clock();
    gmp_randstate_t grt;
    gmp_randinit_default(grt);
    gmp_randseed_ui(grt, time);
    mpz_t x,y,maxr,c,d,a,b;
    mpz_init(x);
    mpz_init(y);
    mpz_init(maxr);
    mpz_init(c);
    mpz_init(d);
    mpz_init(a);
    mpz_init(b);
    mpz_set_str(a,"f37cebd048ea0746a03188497bb9382129743160439992b7d9e3b244acad0e4b",16);
    mpz_set_str(b,"2a32330de2514087a91ae09bee819cf71e0aea23aa278893ee7dc5229afc2410",16);
    mpz_set_str(maxr,"ffffffffffffffffffffffffffffffffffffffff00000001000000000000a677",16);

    do{
        mpz_set_ui(c,0);
        mpz_set_ui(d,0);
        mpz_set_ui(x,0);
        mpz_set_ui(y,0);
        mpz_urandomm(x, grt, maxr);
        mpz_pow_ui(c,x,3);
        mpz_mul(d,a,x);
        mpz_add(c,c,d);
        mpz_add(c,c,b);
        mpz_mod(c,c,maxr);
        if(mpz_cmp_ui(c,0)==0)
            break;
        if(getSqrt_mod(c,maxr,&y))
            break;
    }while(1);
    qDebug()<<"x="<<mpz_get_str(NULL,16,x)<<"\n";
    qDebug()<<"y="<<mpz_get_str(NULL,16,y)<<"\n";
    mpz_clear(x);
    mpz_clear(y);
    mpz_clear(maxr);
    mpz_clear(c);
    mpz_clear(d);
    mpz_clear(a);
    mpz_clear(b);
}

void Widget::onbutton()
{
    QString text=m_edit->toPlainText();
    if(text.isEmpty())
      return;
    mpz_t vv;
    mpz_init(vv);
    if(isHex)
    {
        mpz_set_str(vv,text.toUtf8().data(),16);
    }else {
        mpz_set_str(vv,text.toUtf8().data(),10);
    }
    if (miller_rabin(&vv,10))
        m_la->setText("是素数");
    else
        m_la->setText("不是素数");
    mpz_clear(vv);
   // createArgab();
   // createXY();
}
typedef struct GcdResult
{
    mpz_t d;
    mpz_t x, y;
}GcdResult;

void gcd_Result(mpz_t a, mpz_t b, GcdResult* res)
{
    mpz_t c, d,e,x1,y1;
    mpz_init(c);
    mpz_init(d);
    mpz_init(e);
    mpz_init(x1);
    mpz_init(y1);
    mpz_set(d,b);
    if (mpz_cmp_ui(d,0)==0)
    {
       // res->d = a;
        mpz_set(res->d,a);
       // res->x = 1;
        mpz_set_ui(res->x,1);
        //res->y = 0;
        mpz_set_ui(res->y,0);
    }
    else
    {
        mpz_mod(e,a,b);
        gcd_Result(d, e,res);
        mpz_set(x1,res->x);
        mpz_set(y1,res->y);
        mpz_set(res->x,y1);

        mpz_div(c,a,b);
        mpz_mul(c,c,y1);
        mpz_sub(res->y,x1,c);
    }
    mpz_clear(x1);
    mpz_clear(y1);
    mpz_clear(e);
    mpz_clear(c);
    mpz_clear(d);
}

void Widget::onbutton1()
{
    QString text=v->toPlainText();
    QString text1=v1->toPlainText();
    if(text.isEmpty())
      return;
    if(text1.isEmpty())
      return;
    mpz_t val,val1;
    mpz_init(val);
    mpz_init(val1);
    if(isHex)
    {
        mpz_set_str(val,text.toUtf8().data(),16);
        mpz_set_str(val1,text1.toUtf8().data(),16);
    }else {
        mpz_set_str(val,text.toUtf8().data(),10);
        mpz_set_str(val1,text1.toUtf8().data(),10);
    }
    if(m_box1->currentIndex()==0)
    {
        mpz_add(val,val,val1);
    }
    else if(m_box1->currentIndex()==1)
    {
        mpz_sub(val,val,val1);
    }
    else if(m_box1->currentIndex()==2)
    {
        mpz_mul(val,val,val1);
    }
    else if(m_box1->currentIndex()==3)
    {
        mpz_div(val,val,val1);
    }else if(m_box1->currentIndex()==4)
    {
        mpz_mod(val,val,val1);
    }else if(m_box1->currentIndex()==5)
    {
        mpz_pow_ui(val,val,text1.toInt());
    }
    else if(m_box1->currentIndex()==6)
    {
        mpz_sqrt(val,val);
    }
    else if(m_box1->currentIndex()==7)
    {
        GcdResult res;
        mpz_init(res.d);
        mpz_init(res.x);
        mpz_init(res.y);
        gcd_Result(val1, val,  &res);
        mpz_set(val,res.y);
        mpz_clear(res.d);
        mpz_clear(res.x);
        mpz_clear(res.y);
    }
    if(isHex)
    v2->setPlainText(mpz_get_str(NULL,16,val));
    else
    v2->setPlainText(mpz_get_str(NULL,10,val));
    mpz_clear(val);
    mpz_clear(val1);
}

void Widget::onbutton2()
{

    QString text=m_edit->toPlainText();
    if(text.isEmpty())
      return;
    mpz_t vv;
    mpz_init(vv);
    if(isHex)
    {
        mpz_set_str(vv,text.toUtf8().data(),16);
    }else {
        mpz_set_str(vv,text.toUtf8().data(),10);
    }
    if(mpz_even_p(vv))
            mpz_add_ui(vv,vv,1);
    else
            mpz_add_ui(vv,vv,2);
    while(!miller_rabin(&vv,10))
    {
            mpz_add_ui(vv,vv,2);
    }
    if(isHex)
    m_edit->setPlainText(mpz_get_str(NULL,16,vv));
    else
    m_edit->setPlainText(mpz_get_str(NULL,10,vv));
    mpz_clear(vv);
}

void Widget::onbutton3()
{
    QString text=m_edit->toPlainText();
    if(text.isEmpty())
      return;
    mpz_t vv;
    mpz_init(vv);
    if(isHex)
    {
        mpz_set_str(vv,text.toUtf8().data(),16);
    }else {
        mpz_set_str(vv,text.toUtf8().data(),10);
    }
    if(mpz_cmp_ui(vv,2)>0)
    {
        if(mpz_even_p(vv))
            mpz_sub_ui(vv,vv,1);
        else
            mpz_sub_ui(vv,vv,2);
        while(!miller_rabin(&vv,10))
        {
            mpz_sub_ui(vv,vv,2);
        }
    }
    if(isHex)
    m_edit->setPlainText(mpz_get_str(NULL,16,vv));
    else
    m_edit->setPlainText(mpz_get_str(NULL,10,vv));
    mpz_clear(vv);
}

void Widget::onbutton4()
{
    QString text=m_edit->toPlainText();
    if(text.isEmpty())
      return;
    mpz_t vv;
    mpz_init(vv);
    if(isHex)
    {
        mpz_set_str(vv,text.toUtf8().data(),16);
        m_edit->setPlainText(mpz_get_str(NULL,10,vv));
    }else {
        mpz_set_str(vv,text.toUtf8().data(),10);
        m_edit->setPlainText(mpz_get_str(NULL,16,vv));
    }
    mpz_clear(vv);
}



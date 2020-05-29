#include "widget.h"

#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
      QApplication a(argc, argv);   
   /*   mpz_t aa, b, c, d;
      mpz_init(aa);
      mpz_init(b);
      mpz_init(c);
      mpz_init(d);
      //计算2的1000次方
      mpz_init_set_ui(aa, 2);
      gmp_printf("aa = %Zd\n", aa);
      mpz_pow_ui(aa, aa, 1024);
      gmp_printf("aa = %Zd\n", aa);


      //计算12345678900987654321*98765432100123456789
      mpz_init_set_str(b, "12345678900987654321", 10);//10进制
      mpz_init_set_str(c, "98765432100123456789", 10);
      mpz_mul(d, b, c);
      gmp_printf("d = %Zd\n", d);

      int v=mpz_tstbit(d,1023);
      v=mpz_tstbit(c,1023);
      mpz_init_set_ui(b, 100);
      mpz_init_set_ui(c, 3);
      mpz_mod(b,b,c);
      mpz_init_set_ui(c, 1);
      mpz_mul_2exp(c,c,3);//左移
      gmp_printf("c = %Zd\n", c);
      mpz_div_2exp(c,c,2);//右移
      gmp_printf("c = %Zd\n", c);
      qDebug()<<"cmp ui: "<<mpz_cmp_ui(c, 4);
      char *str="1219326311225422953654138088831112635269";
      char str1[]="FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123";
      mpz_set_str(c,str1,16);
      if (miller_rabin(&c,10))
          qDebug()<< str1<<"是素数";
      else
          qDebug()<< str1<<"不是素数";
      mpz_clear(aa);
      mpz_clear(b);
      mpz_clear(c);
      mpz_clear(d);*/

    Widget w;
    w.show();
    return a.exec();
}

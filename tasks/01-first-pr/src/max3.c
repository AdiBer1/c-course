#include "max3.h"

int max3(int a, int b, int c)
{
   if (a>b){
    if (a>c){
        return a;
    }
    return c;
   }
   if (b>c){
    return b;
   }
   return c;
}

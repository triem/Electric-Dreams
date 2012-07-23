#include <stdio.h>

void main()
{
  int x = 64;
  char buf[256];

   x++;
   sprintf(buf,"mkdir '\0%d'", x);
   system(buf);
} 

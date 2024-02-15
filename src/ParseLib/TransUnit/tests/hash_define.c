#define _TEST_H_

#define TRUE 1

#define INC(X) X=X+1

#define ADD(X,Y) X=X+Y

#define SKIP_SPACES(p, limit)     \
do { char *lim = (limit);         \
     while (p < lim) {            \
       if (*p++ != ' ') {         \
         p--; break; }}}          \
while (0)
  
void f()
{
  int a;
  int b;
  char line[] = "      Hello";
  
  INC(a);
  ADD(a,b);
  SKIP_SPACES(line, &line[10]);
  
  a = TRUE;
}

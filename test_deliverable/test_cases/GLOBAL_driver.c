extern int a; /* global variable from 'global.c' */
int b = 1;

int g(); /* should return b */

#define SUM 2 /* SUM = a + b */

int main(){

    int sum = a + g();

    return (sum - SUM) ? 1:0;
}
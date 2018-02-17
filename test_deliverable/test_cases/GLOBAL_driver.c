int g();
/*
    returns c - global variable below
    also g var's 'a' and 'b' declared in global.c, both = 3
*/

int c = 3;

#define SUM 9
// SUM = c + a + b

int main(){

    int sum = a + b + g();

    return (sum - SUM) ? 1:0;
}
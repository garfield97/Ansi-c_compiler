int f(int x,int y){
    x=5;
    return x+y;
}

int g(int y,int x){
    y = 7;
    return y+x;
}

int main(){
    
    int x;
	int y;
    x = f(x,5);
    y = g(x,12);
    return x+y;
}
int f(int x);


int main(){
    int x;
    int y;
    x = f(5);
    return x;
    
}



int f(int x){
    
    if(x !=0){
        f(x-1);
    }
}
    



int a(int n);

int b(int n);


int main(){
        
        int n = 5;
        
        if(a(n)<12){
                
                n = b(n);
        }
        
        return n;
}

                
int a(int n){
        
        return n+5;

}

int b(int n){
        
        return a(n)+5;

}


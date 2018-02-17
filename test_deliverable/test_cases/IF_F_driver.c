int IF_F(); // expect 1 to be returned

int main(){
    int val = IF_F();
    
    // if IF_T 'failed' and returned 0, ternary operator makes main return 10
    return (val) ? 0:1;    
}
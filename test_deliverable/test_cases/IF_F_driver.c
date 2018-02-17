int IF_F(); // expect 1 to be returned

int main(){
    int val = IF_F();
    
    // if IF_F 'failed' and returned 0, ternary operator makes main return 1
    return (val) ? 0:1;    
}
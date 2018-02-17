int IF_T(); // expect 1 to be returned

int main(){
    int val = IF_T();
    
    // if IF_T 'failed' and returned 0, ternary operator makes main return 10
    return (val) ? 0:10;    
}
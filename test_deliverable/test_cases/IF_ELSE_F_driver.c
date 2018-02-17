int IF_ELSE_F(); // expect 1 to be returned

int main(){
    int val = IF_ELSE_F();
    
    // if IF_ELSE_F 'failed' and returned 0, ternary operator makes main return 1
    return (val) ? 0:1;    
}
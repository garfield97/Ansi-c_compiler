int IF_ELSE_T(); // expect 1 to be returned

int main(){
    int val = IF_ELSE_T();
    
    // if IF_ELSE_T 'failed' and returned 0, ternary operator makes main return 1
    return (val) ? 0:1;    
}
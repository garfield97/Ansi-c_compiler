int IF_ELSE_T(){
    //should always evaluate to true - so take true branch
    if(1) return 1;
    else return 0;  // 'if' broken on compiler - false branch when always true
}
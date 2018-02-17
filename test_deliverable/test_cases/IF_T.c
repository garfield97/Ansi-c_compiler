int IF_T(){
    if(1){ //should always evaluate to true - so take if
        return 1;
    }
    else return 0;  // if broken on compiler
}
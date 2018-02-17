int local_array(int index){ // 0 <= index <= 9

    int val; // not used until set

    // local array - also tests array initialisation
    int la [10] = {1,1,1,1,1,1,1,1,1,1};

    val = la[index]; // tests read from local array

    // tests write to local array
    la[0] = 0;
    la[1] = 0;
    la[2] = 0;
    la[3] = 0;
    la[4] = 0;
    la[5] = 0;
    la[6] = 0;
    la[7] = 0;
    la[8] = 0;
    la[9] = 0;
    // add la[index] to sum (should have no effect if write working)

    val = val + la[index]; // value should be 1

    return val; // return should be 1

    /* could have done 'return sum + la[index];'
       but this would require the compiler to ensure return works for more than just one variable
    */
}
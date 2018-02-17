int local_array(int index);
/* MUST HAVE 0 <= index <= 9 - otherwise segmentation fault
   return should be 1
   dependancy - add for int
   dependancy - function parameters working on compiler being tested
   - what if compiler works for element X, but not element Y?
*/

#define RUNS 10
// equal to array size used in local_array

int main(){

    int sum = 0;

    // call local_array $RUNS times testing with index = 0 to 9 iteratively
    for(int i = 0; i < RUNS; i++){
        sum += local_array(i);
        // if local_array worked , sum should increase by 1
    }

    //returns 0 if local_array returned 1 $RUNS times
    return (sum == RUNS) ? 0:1;
}
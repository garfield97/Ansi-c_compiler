
int main () {

   char grade = 'B';
   int x;

   switch(grade) {
   
      case 'A' :
            x=1;
            
      case 'B' :
            x=2;
      
      case 'C' :
            x=3;
            
      case 'D' :
            x=4;

      case 'F' :
            x=5;


      default :
            x=9999;
   }
   
    return x;  
 }



#include "histogram.hpp"

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iomanip>

// Use a hash-table to maintain a word -> count mapping.
// This deliberately uses std::unordered_map rather than std::map,
// because we don't need to enumerate the entries in key order.
// This allows us to use a hash-table, so we get O(1) operations
// rather than O(log n).
typedef std::unordered_map<std::string,unsigned> histogram_type;


// Define the instance of the variable that is declared in the histogram.hpp header
TokenValue yylval;

int main()
{
    histogram_type histogram; // Create the hash-table instance
    double sum=0; // Our running sum

    while(1){
        TokenType type=(TokenType)yylex();

        if(type==None){
            break; // No more tokens

        }else if(type==Keyword){
            
     
            
        }else if(type==Identifier){
      
        }else if(Decimal_constant){    
            
        }else if(Octal_constant){     
       
        }else if(Hexadecimal_constant){
        
        
        }else if(Character_constant){


        }else{
            assert(0); // There are only three token types.
            return 1;
        }
    }


    // TODO: print out `sum` to std::cout with three decimal digits
	std::cout.setf(std::ios::fixed, std::ios::floatfield);
	std::cout.precision(3);
   	std::cout << sum << std::endl;

    // Build a vector of (word,count) entries based on the hash-table
    std::vector<std::pair<std::string,unsigned> > sorted(histogram.begin(), histogram.end());

    // Create a custom sort order which orders by count first, then by string
    auto sort_order=[](const std::pair<std::string,unsigned> &a, const std::pair<std::string,unsigned> &b){
        if(a.second > b.second)
            return true;
        if(a.second < b.second)
            return false;
        return a.first < b.first;
    };

    // Sort the array using our custom sort order
    std::sort(sorted.begin(), sorted.end(), sort_order);

    auto it=sorted.begin();
    while(it!=sorted.end()){
        std::string name=it->first;
        unsigned count=it->second;
        // TODO: Print out `name` and `count` to std::cout
        std::cout << "[" << name << "] " << count << '\n';
        
        ++it;
    }

    return 0;
}

Keyword                 auto|double|int|struct|break|else|long|switch|case|enum|register|typedef|char|extern|return|union|const|float|short|unsigned|continue|for|signed|void|default|goto|sizeof|volatile|do|if|static|while

identifier              [_a-zA-Z]+[0-9_a-zA-Z]*
//C++ always start off with either none digit or underscore as c variables cannot start off with digits
// go back to hard stuff 

floating-constant       [+-]?( ([0-9]+[.][0-9]*) | ([.][0-9]+) | ([0-9]+([.][0-9]*)?[eE][+-]?[0-9]+) | ([.][0-9]+[eE][+-]?[0-9]+) )[lfLF]?

integer-suffix          [u|U][l|L]? | [l|L][u|U]?

decimal                 [1-9][0-9]*[integer-suffix]?

character-constant      []






















octal                   [0][0-7]*[integer-suffix]?

hexadecimal-constant    0x[0-9a-fA-F]+ | 0X[0-9a-fA-F]+


keyword                 auto|double|int|struct|break|else|long|switch|case|enum|register|typedef|char|extern|return|union|const|float|short|unsigned|continue|for|signed|void|default|goto|sizeof|volatile|do|if|static|while

identifier              [_a-zA-Z][0-9_a-zA-Z]*
// cannot match a keyword

// CONSTANTS
floating-constant       [+-]?( ([0-9]+[.][0-9]*) | ([.][0-9]+) | ([0-9]+([.][0-9]*)?[eE][+-]?[0-9]+) | ([.][0-9]+[eE][+-]?[0-9]+) )[lfLF]?

// used for integer-constants
integer-suffix          ([uU][lL]?) | ([lL][uU]?)

decimal-constant        [1-9][0-9]*[integer-suffix]?

octal-constants         [0][0-7]*[integer-suffix]?

hexadecimal-constant    (0x|0X)[0-9a-fA-F]+[integer-suffix]?

character-constant      []
























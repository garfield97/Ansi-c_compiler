#ifndef literal_type_hpp
#define literal_type_hpp

// use to identify tpye a primary expression is
typedef enum{
    S,   // string
    I,   // integer
    U,  // unsigned integer
    LI,  // long integer
    UL,  // unsigned long
    C,   // character
    F    // float
} LITERAL_TYPE;

#endif
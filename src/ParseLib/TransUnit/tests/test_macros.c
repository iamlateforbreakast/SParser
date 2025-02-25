#include <stdio.h>

// Simple macro definition
#define PI 3.14159

// Macro with arguments
#define SQUARE(x) ((x) * (x))

// Multi-line macro using backslashes
#define PRINT_HELLO_WORLD() \
    printf("Hello, ");      \
    printf("World!\n");

// Macro with stringification (#)
#define STRINGIFY(x) #x

// Macro with concatenation (##)
#define CONCAT(x, y) x##y

// Macro with variable arguments
#define PRINT_VALUES(...) printf(__VA_ARGS__)

// Macro that checks for definition
#ifdef DEBUG
    #define DEBUG_PRINT(format, ...) printf("DEBUG: " format, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(format, ...) /* Empty macro when not in debug mode */
#endif

// Example of a macro that redefines itself (be careful with this!)
#define MY_MACRO
#ifdef MY_MACRO
  #undef MY_MACRO
  #define MY_MACRO_DEFINED
#endif


int main() {
    // Using simple macro
    printf("Pi: %f\n", PI);

    // Using macro with arguments
    int num = 5;
    printf("Square of %d: %d\n", num, SQUARE(num));
    printf("Square of %f: %f\n", 2.5f, SQUARE(2.5f));  //Illustrates floating-point use.

    // Using multi-line macro
    PRINT_HELLO_WORLD();

    // Using stringification
    printf("Stringified PI: %s\n", STRINGIFY(PI));
    printf("Stringified SQUARE(x): %s\n", STRINGIFY(SQUARE(x))); //Important test case!

    // Using concatenation
    int var1 = 10;
    int var2 = 20;
    int var12;  // To store the concatenated result
    CONCAT(var1, var2) = var1 + var2; // Note: L-value required for concatenation
    printf("Concatenated value: %d\n", var12);


    // Using variable arguments macro
    PRINT_VALUES("Value 1: %d, Value 2: %f\n", 10, 3.14);

    // Using conditional macro (DEBUG_PRINT)
    DEBUG_PRINT("Value of num: %d\n", num); // Will only print if DEBUG is defined

    //Demonstrating macro redefinition
    #ifdef MY_MACRO_DEFINED
        printf("MY_MACRO was redefined successfully.\n");
    #else
        printf("MY_MACRO was NOT redefined.\n"); //Should not be printed.
    #endif


    //Illustrating potential issues with macro side effects.
    int x = 5;
    printf("SQUARE(x++): %d\n", SQUARE(x++)); //x will be incremented TWICE!
    printf("Value of x after SQUARE(x++): %d\n", x); //Important to understand.

    return 0;
}

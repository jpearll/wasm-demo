
// Include the standard I/O library for printing to the console.
#include <stdio.h>
// Include the Emscripten library for WebAssembly-specific functions.
#include <emscripten.h>

// When compiling C++ code, functions have to be declared as 'extern "C"' in order to 
// avoid name mangling and ensure that the functions can be called from JavaScript.
// If we are compiling C code, this is not necessary.
#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

// The main function is the entry point for C and C++ programs.
// For WebAssembly modules, it is called once when the module is loaded.
// In this case, it doesn't do anything.
int main() {
    return 0;
}

// 'EXTERN' will either be 'extern "C"' (for C++) or empty (for C).
// 'EMSCRIPTEN_KEEPALIVE' is an Emscripten directive that prevents the function from 
// being eliminated during dead code elimination. It's needed for functions that 
// are called from JavaScript.
EXTERN EMSCRIPTEN_KEEPALIVE void custom_function() {
    // Print a message to the console. This will actually be output to the browser's console.
    printf("This function is triggered from C!\n");
}

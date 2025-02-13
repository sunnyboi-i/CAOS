# IEEE 754 Floating-Point Classification

This project provides a C implementation for classifying double-precision floating-point numbers according to the IEEE 754 standard. It categorizes numbers into classes such as Zero, Infinity, Regular, Denormal, and NaN.

## Files

- **ieee754_clf.c**: Contains the implementation of the floating-point classification function.
- **ieee754_clf.h**: Header file defining the enumeration and function prototype for classification.
- **main.c**: Demonstrates the usage of the classification function and includes test cases.
- **Makefile**: Simplifies the build and test process.

## Functions

### `classify`
```c
float_class_t classify(double x);
Classifies a given double-precision floating-point number into one of the IEEE 754 categories.
```
### `format_float_class`
```c
void format_float_class(float_class_t c, char buf[], size_t len);
Formats the classification result into a human-readable string.
```
### test_clf
```c
bool test_clf(double x, float_class_t expected_class);
Tests the `classify` function by comparing the actual classification result with the expected result.
```

### run_all_tests
```c
bool run_all_tests();
Runs a suite of predefined test cases to verify the correctness of the classification function.
```
### Usage:
Build and run the program:
```c
make
make run

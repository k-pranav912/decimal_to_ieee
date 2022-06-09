# Decimal to IEEE-FP representation

C Program to convert a decimal fraction to IEEE-754 FP representation in a
given configuration with the rounding to nearest with ties-to-even rounding mode.

## Makefile commands:
make - compiles the binary <br>
make clean - deletes the current existing compiled binary

## Input:
Input is through a text file containing the decimal number and the configuration of the FP format.

## Input Format:
decimal-number #bits_in_FP_representation #bits_exponent #bits_fraction

### test.txt <br>
6.5 8 4 3 <br>
.0546875 8 4 3 <br>
.013671875 8 4 3 <br>
6.375 8 4 3 <br>
8.5 8 4 3 <br>
9.5 8 4 3 <br>

## Execution:
### $./fifth test.txt

## Output
01001101 <br>
00010110 <br>
00000111 <br>
01001101 <br>
01010000 <br>
01010010 <br>
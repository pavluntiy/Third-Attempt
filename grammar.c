
hexadecimal_digit ::=
		  decimal_digit
		  hexadecimal_additional_digit
decimal_digit ::=
		nonzero_decimal_digit
		zero
octal_digit ::= 
		nonzero_octal_digit
		zero

binary_digti ::= 
		unity
		zero


number ::= integer |
	   float |
	   rational 

integer ::= 
	    binary
	    octal
	    decimal
	    hexadecimal

binary::= 
	binary_prefix binary_digit[(binary_digit|'_')*][constant_suffix]
binart_prefix ::=	
 		0b | 0B

octal ::=
	octal_prefix octal_digit[(octal_digit|'_')*][constant_suffix]
octal_prefix::= 0''

decimal ::=
	decimal_digit[(decimal_digit|'_')*][constant_suffix]

hexadecimal ::= 
	hexadecimal_prefix hexadecimal_digit[(hexadecimal_digit|'_')*][constant_suffix]


identifier ::= 
	letter[(letter|digit)*]

letter ::= 
	latin_letter|underscore

	


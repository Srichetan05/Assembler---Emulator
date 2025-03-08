;sampletest1.asm
; this test is only for checking any errors
ldc     0xa5    		;checking for lower case hex
ldc     0xA5 			;checking for upper case hex
adc     -5 				;negative value in adc
ldc     +0 				;+0 is acceptable
ldc     -0 				;-0 is acceptable
adj     022   			;+022 is treated as Octal

HALT

Start:

loop2:  SET -10				;set loop2 to the value of -10
call loop2 				;calling loop2

var1: data -1000000000 	;setting var1 equal to -1000000000

call Start 				;calling start
HALT
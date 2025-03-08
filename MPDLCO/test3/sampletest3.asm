;sampletest3.asm
;Multiply two numbers
loop: 
	ldc res	
   ldnl 0   ;Load res into A
   adc var1 ;Add var1 to A
   ldc res  
   stnl 0   ;Store A into res
   ldc var2 
   ldnl 0   ;Load var2 into A
   adc -1   
   ldc var2  
   stnl 0   ; Store A into var2
   ldc var2 
   ldnl 0   ; Load var2 into A
   brz exit ; exit if A = 0
   br loop ;repeat
exit: 
	HALT     ; Terminate 

var1: SET 2
var2: data 3
res: data 0
;sampletest4.asm
;sum of first n numbers

call sumofnum
HALT

sumofnum:   stl 0       ;store return address (from A) into stack
            adj -1      
loop:       ldc num
            ldnl 0      ;load num into A
            ldc sum
            ldnl 0      ;load sum into A, B = num
            add         ;A = B + A
            ldc sum
            stnl 0      ;store new value of sum
            ldc num
            ldnl 0      ;load num into A
            adc -1      ;decrement A
            brz done    ;if A is 0,done
            ldc num
            stnl 0      ;otherwise update num value
            br loop     ;repeat
done:   
            adj 1       ;adjust stack pointer
            ldl 0       ; load address to A
            return      

num: data 100       ; number upto which sum is calculated
sum: data 0         ; sum is stored here
;sampletest5.asm
;This function gives a poynomial outpu(32x+3)
call calc       ; function call
HALT

calc:   stl 0           ; store return address (from A) into stack
        ldc var         ; load address of variable var
        ldnl 0          ; load value of var
        ldc a 
        ldnl 0
        add
        ldc b 
        ldnl 0
        add
        ldc c 
        ldnl 0
        add
        ldc d 
        ldnl 0
        add    
        ldc res    
        stnl 0          
        ldl 0           ;load return address
        return          ;return
        
var: data 5
a: data 5
b: data 5
c: data 5
d: data 5
res: data 0
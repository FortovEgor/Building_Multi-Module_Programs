section .rodata
    mul03 dq 0.35
    mul07 dq 0.7
    mul09 dq 0.95
    mul3 dq 3.0
    chis27 dq 2.7
    chis2 dq 2.0
    chis1 dq 1.0
    zero dq 0.0
section .text
global f1
f1:;0.35x^2-0.95x+2.7
    push ebp
    mov ebp , esp
    finit
    fld qword[ebp+8]
    fld ST0
    fmulp
    fld qword[mul03]
    fmulp
    fld qword[ebp+8]
    fld qword[mul09]
    fchs
    fmulp
    faddp
    fld qword[chis27]
    faddp
    mov esp , ebp
    pop ebp
    ret
    
global f2
f2:;3x+1
    push ebp
    mov ebp , esp
    finit
    fld1
    fld qword[ebp+8]
    fld qword[mul3]
    fmulp
    faddp
    mov esp , ebp
    pop ebp
    ret
    
global f3   
f3:;1/(x+2)
    push ebp
    mov ebp , esp
    finit
    fld1
    fld qword[ebp+8]
    fld qword[chis2]
    faddp
    fdivp
    mov esp , ebp
    pop ebp
    ret
    
global f1_div_1   
f1_div_1:;0.7x-0.95
    push ebp
    mov ebp , esp
    finit
    fld qword[mul09]
    fchs
    fld qword[mul07]
    fld qword[ebp+8]
    fmulp
    faddp
    mov esp , ebp
    pop ebp
    ret
    
global f2_div_1   
f2_div_1:;3
    push ebp
    mov ebp , esp
    finit
    fld qword[mul3]
    mov esp , ebp
    pop ebp
    ret

global f3_div_1  
f3_div_1:;-1/(x+2)^2
    push ebp
    mov ebp , esp
    finit
    fld1  ; ST0 = 1
    fld qword[ebp+8]  ; ST0 = x; ST1 = 1
    fld qword[chis2]  ; ST0 = 2; ST1 = x; ST2 = 1
    faddp  ; ST0 = ST0 + ST1 = x + 2; ST1 = 1
    fdivp  ; ST0 = ST1 / ST0 = 1 / (x + 2); ST1 = 1
    fld ST0  ;  ST0 = 1 / (x+2); ST1 = 1/(x+2); ST2=1
    fmulp  ; ST0 = ST0 * ST1 = 1/(x+2)**2; ST1=1
    fchs  ; ST0 = -1/(x+2)**2
    mov esp , ebp
    pop ebp
    ret
    
; f''(x)
global f1_div_2  
f1_div_2:;0.7
    push ebp
    mov ebp, esp
    finit
    fld qword[mul07]
    mov esp , ebp
    pop ebp
    ret
    
global f2_div_2  
f2_div_2:; 0
    push ebp
    mov ebp, esp
    finit
    fld qword[zero]
    mov esp, ebp
    pop ebp
    ret   

global f3_div_2 
f3_div_2:; 2/(x+2)^3
    push ebp
    mov ebp , esp
    finit
    fld1  ; ST0 = 1
    fld qword[ebp+8]  ; ST0 = x; ST1 = 1
    fld qword[chis2]  ; ST0 = 2; ST1 = x; ST2 = 1
    faddp  ; ST0 = ST0 + ST1 = x + 2; ST1 = 1
    fdiv ST1, ST0  ; ST1 = 1 / (x+2)
    fdiv ST1, ST0  ; ST1 = 1 / (x+2)**2
    fdiv ST1, ST0  ; ST1 = 1 / (x+2)**3
    fxch ST1  ; ST0 = 1 / (x+2)**3
    fld qword[chis2]  ; ST0 = 2  St1 = 1/(x+2)**3
    fmulp ST1, ST0  ; ST0 = 2 / (x+2)**3
    mov esp , ebp
    pop ebp
    ret 

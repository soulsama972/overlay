%ifdef RCX  ; check if we are in 64bit 
extern Jump:proc
extern GetEntity:proc
%else ; if we arnt in 64bit we need to include .model flat so we can compile the dll
.model flat
%endif



.data
jumpBack dq 00 ; use for jumpBack to orignalFunc
saveReg db 128 dup (0) ; for macro 





.code ; add your functions here

%ifdef RCX
DarkSoulEntity proc
mov [rbx+000003D8h],eax
push rbp
mov rbp,rsp
sub rsp,512
call PushReg
;pushf
mov rcx,rbx
call GetEntity
mov jumpBack,rax
;popf
call PopReg
mov rsp,rbp
pop rbp
jmp jumpBack
DarkSoulEntity endp




PushReg proc ; macro for pushad in 64bit
mov qword ptr [saveReg], rbp
mov qword ptr [saveReg+8], rax
mov qword ptr [saveReg+16], rbx
mov qword ptr [saveReg+24], rdx
mov qword ptr [saveReg+32], rcx
mov qword ptr [saveReg+40], rsi
mov qword ptr [saveReg+48], rdi
mov qword ptr [saveReg+56], r8
mov qword ptr [saveReg+64], r9
mov qword ptr [saveReg+72], r10
mov qword ptr [saveReg+80], r11
mov qword ptr [saveReg+88], r12
mov qword ptr [saveReg+96], r13
mov qword ptr [saveReg+104], r14
mov qword ptr [saveReg+112], r15
ret
PushReg endp

PopReg proc ; macro for popad in 64bit
mov rbp, qword ptr [saveReg] 
mov rax, qword ptr [saveReg+8] 
mov rbx, qword ptr [saveReg+16] 
mov rdx, qword ptr [saveReg+24] 
mov rcx, qword ptr [saveReg+32] 
mov rsi, qword ptr [saveReg+40] 
mov rdi, qword ptr [saveReg+48] 
mov r8 , qword ptr [saveReg+56] 
mov r9 , qword ptr [saveReg+64] 
mov r10, qword ptr [saveReg+72]
mov r11, qword ptr [saveReg+80] 
mov r12, qword ptr [saveReg+88]
mov r13, qword ptr [saveReg+96] 
mov r14, qword ptr [saveReg+104]
mov r15, qword ptr [saveReg+112]
ret
PopReg endp
%endif

end



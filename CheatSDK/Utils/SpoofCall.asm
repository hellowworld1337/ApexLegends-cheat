extern legitTrampoline:QWORD ; add rsp, 68h, ret
extern legitTrampoline2:QWORD ; jmp rbx

.data
RBX_Saved QWORD 0
RET_Saved QWORD 0

.code
_spoofer_stub PROC
	mov rax, [rsp]
	xor rax, rbx
	mov RET_Saved, rax
	mov rax, legitTrampoline2
	mov RBX_Saved, rbx
	mov [rsp], rax
	lea rbx, PostCallRBX

	sub     rsp, 68h
	mov     r10, [rsp+144]
	mov     rax, [rsp+200]
	mov     [rsp+88], rax
	mov     rax, [rsp+192]
	mov     r11, [r10]
	mov     [rsp+80], rax
	mov     rax, [rsp+184]
	mov     [rsp+72], rax
	mov     rax, [rsp+176]
	mov     [rsp+64], rax
	mov     rax, [rsp+168]
	mov     [rsp+56], rax
	mov     rax, [rsp+160]
	mov     [rsp+48], rax
	mov     rax, [rsp+152]
	mov     [rsp+40], rax
	mov     rax, [r10+8]
	mov     [rsp+32], rax
	push    legitTrampoline
	jmp     r11

	PostCallRBX:
	mov rbx, RBX_Saved
	mov rcx, RET_Saved
	xor rcx, rbx
	jmp rcx
_spoofer_stub ENDP
END

int64 j,k;


char message1[]  = "Why hello there why does this cut \n";
char message2[]  = "This is message #2 \n";

char *ball;


int64 l = 10*2+3;

void exit() {

  asm("
        mov       rax, 60 
        xor       rdi, rdi                ; exit code 0
        syscall                           ; invoke operating system to exit
  ");
}



void printf(char* msg) {
  
  asm("


        mov     rdi, [rsp+8]        ; string1 to destination index
        xor     rcx, rcx            ; zero rcx
        not     rcx                 ; set rcx = -1
        xor     al,al               ; zero the al register (initialize to NUL)
        cld                         ; clear the direction flag
        repnz   scasb               ; get the string length (dec rcx through NUL)
        not     rcx                 ; rev all bits of negative results in absolute value
        dec     rcx                 ; -1 to skip the null-terminator, rcx contains length
        mov     rdx, rcx            ; put length in rdx

        mov       rax, 1    
        mov       rdi, 1                  ; file handle 1 is stdout
        mov       rsi, [rsp+8]            ; address of string to output

        syscall                           ; invoke operating system to do the write
  ");
}


int itoa() {
asm("
; Input
; EAX = pointer to the int to convert
; EDI = address of the result
; Output:
; None

   mov edi, message1
   mov eax, 128

    xor   ebx, ebx        ; clear the ebx, I will use as counter for stack pushes
.push_chars:
    xor rdx, rdx          ; clear edx
    mov rcx, 10           ; ecx is divisor, devide by 10
    div rcx               ; devide edx by ecx, result in eax remainder in edx
    add rdx, 0x30         ; add 0x30 to edx convert int => ascii
    push rdx              ; push result to stack
    inc rbx               ; increment my stack push counter
    test rax, rax         ; is eax 0?
    jnz .push_chars       ; if eax not 0 repeat

.pop_chars:
    pop rax               ; pop result from stack into eax
    stosb                 ; store contents of eax in at the address of num which is in EDI
    dec rbx               ; decrement my stack push counter
    cmp rbx, 0            ; check if stack push counter is 0
    jg .pop_chars         ; not 0 repeat
    mov rax, 0x0a
    stosb                 ; add line feed

    mov eax,0
    stosb

");

}


int main() {
  j = 5;
  k=j;
  printf(&message2);  
  itoa();
  printf(&message1);  
  exit();

}


int64 j,k;


char message1[]  = "Why hello there why does this cut ";
char message2[]  = "This is message #2 ";

char *ball;


int64 l = 10*2+3;

void exit() {

  asm("
;       mov       rax, 60                 ; system call for exit
        mov       rax, 0x02000001         ; system call for exit
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

        mov       rax, 0x02000004     
        mov       rdi, 1                  ; file handle 1 is stdout
        mov       rsi, [rsp+8]            ; address of string to output

        syscall                           ; invoke operating system to do the write
      ;      mov       rax, 60                 ; system call for exit
        mov       rax, 0x02000001         ; system call for exit
        xor       rdi, rdi                ; exit code 0
        syscall                           ; invoke operating system to exit
  ");
}


int main() {
  j = 5;
  k=j;
  printf(&message1);  

  exit();

}

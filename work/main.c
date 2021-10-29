
int j,k;


char message[]  = "Why hello there why does this cut ";



int l = 10*2+3;

void exit() {

  asm("
;          mov       rax, 60                 ; system call for exit
            mov       rax, 0x02000001         ; system call for exit
          xor       rdi, rdi                ; exit code 0
          syscall                           ; invoke operating system to exit
  ");
}

void printf(char* msg) {
  j = 4;
  asm("


    mov     rdi, [rel msg]        ; string1 to destination index
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
            mov       rsi, [rel msg]            ; address of string to output

            syscall                           ; invoke operating system to do the write
      ;      mov       rax, 60                 ; system call for exit
            mov       rax, 0x02000001         ; system call for exit
                  xor       rdi, rdi                ; exit code 0
            syscall                           ; invoke operating system to exit
  ");
}


int main() {
  j = 5;
  k = j;
  asm("
      mov rdi, message
    mov [rel msg], rdi

  ");
  printf();
  exit();

}

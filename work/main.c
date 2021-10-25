
int j,k;

int l;

void exit() {
  asm("
          mov       rax, 60                 ; system call for exit
          xor       rdi, rdi                ; exit code 0
          syscall                           ; invoke operating system to exit
  ");
}

void printf() {
  j = 4;
  asm("
            mov       rax, 1                  ; system call for write
            mov       rdi, 1                  ; file handle 1 is stdout
            mov       rsi, message            ; address of string to output
            mov       rdx, 13                 ; number of bytes
            syscall                           ; invoke operating system to do the write
            mov       rax, 60                 ; system call for exit
            xor       rdi, rdi                ; exit code 0
            syscall                           ; invoke operating system to exit
  ");
}


int main() {
  j = 5;
  k = j;

  printf();
  exit();

  asm("
message:  db        'Hello, World', 10
  ");
}


int j,k;

void printf() {
  asm("
    mov ax,10h
    int 21h

  ");
}


int mainn() {
  j = 5;
  k = j;
}

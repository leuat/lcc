
int j,k;

int l;

void printf() {
  asm("
    mov ax,10h
    int 21h

  ");
}


int main() {
  j = 5;
  k = j;
  printf();
}

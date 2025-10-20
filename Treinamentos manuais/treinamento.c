#include <stdio.h>
#include <locale.h>
int main(){
    setlocale(LC_ALL, "pt_BR.UTF-8");
    int numeros[5] = {1,2,3,4,5};
    int *ptr = numeros;
    printf("Endereço de ptr: %p\n", &ptr);
    printf("Endereço do array numero: %p\n", numeros);
    printf("Endereço apontado por ptr: %p\n", ptr);
    for(int i=0; i<5; i++){
        printf("Índice %d -> Endereço: %p | Valor: %d\n", i, (ptr+i), *(ptr+i));
    }
    getchar();
    return 0;
}
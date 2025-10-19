#include <stdio.h>
#include <locale.h>
int main(){
    setlocale(LC_ALL, "pt_BR.UTF-8");
    int numeros[5] = {1, 2, 3, 4, 5};
    int *ptr = numeros;
    for(int i=0; i<5; i++){
        printf("Números[%d] = %d\n", i, numeros[i]);
    }
    for(int i=0; i<5; i++){
        printf("*(ptr + %d) = %d\n", i, *(ptr + i));
    }
    printf("Pressione qualquer tecla para fechar...");
    getchar();

    return 0;
}
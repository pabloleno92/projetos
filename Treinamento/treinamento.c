#include <stdio.h>
int main() {
    int x = 10;
    int y = 10;
    
    if (x < y) {
        printf("x é menor que y!\n");
    } else if (x > y) {
        printf("x é maior que y!\n");
    } else {
        printf("x é igual a y!\n");
    }
    
    return 0;
}
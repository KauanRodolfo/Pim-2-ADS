/* ==========================================================
   util.c
   Funcoes de apoio para leitura do teclado.
   ========================================================== */
#include "util.h"
#include <stdio.h>
#include <string.h>


/* ---------- ENTRADA DE DADOS ---------- */
void tratar_texto(char *variavel) {
    int i, j = 0, c;

    /* Tira o Enter do final. Se nao tinha Enter, o texto era grande
       demais: descarta o que sobrou no teclado. */
    i = strlen(variavel);
    if (i > 0 && variavel[i - 1] == '\n') {
        variavel[i - 1] = '\0';
    } else {
        while ((c = getchar()) != '\n' && c != EOF);
    }

    /* Tira as aspas simples ('), porque elas quebrariam o comando SQL */
    for (i = 0; variavel[i] != '\0'; i++) {
        if (variavel[i] != '\'') {
            variavel[j] = variavel[i];
            j++;
        }
    }
    variavel[j] = '\0';

}

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // descarta os caracteres residuais no buffer
    }
}

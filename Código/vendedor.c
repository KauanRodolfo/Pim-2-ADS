/* ==========================================================
   vendedor.c
   Tela do vendedor. Por enquanto so o menu esta pronto: as opcoes
   ainda nao fazem nada (as funcoes de cada uma serao feitas depois).
   ========================================================== */
#include "vendedor.h"
#include "funcoes.h"
#include "funcoes_db.h"
#include <stdio.h>

void menu_vendedor() {
    int op;

    do {
        printf("\n=== MENU vendedor ===\n");
        printf("1 - Gerenciar clientes\n");
        printf("2 - Gerenciar contratos\n");
        printf("3 - Ver Ranking Geral\n");
        printf("0 - Sair da conta\n");
        scanf("%d", &op);

        switch (op) {
        case 1:
            // AQUI: chamar a funcao 
            break;

        case 2:
            // AQUI: chamar a funcao 
            break;
        case 3:
            // AQUI: chamar a funcao 
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

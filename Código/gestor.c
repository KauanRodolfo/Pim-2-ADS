/* ==========================================================
   gestor.c
   Tela do gestor. Por enquanto so o menu esta pronto: as opcoes
   ainda nao fazem nada (as funcoes de cada uma serao feitas depois).
   ========================================================== */
#include "gestor.h"
#include "funcoes.h"
#include "funcoes_db.h"
#include <stdio.h>

void menu_gestor() {
    int op;

    do {
        printf("\n=== MENU gestor ===\n");
        printf("1 - Gerenciar Vendedores\n");
        printf("2 - Gerenciar Contratos\n");
        printf("4 - Gerenciar Ranking da equipe\n");
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

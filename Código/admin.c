/* ==========================================================
   admin.c
   Tela do admin. Por enquanto so o menu esta pronto: as opcoes
   ainda nao fazem nada (as funcoes de cada uma serao feitas depois).
   ========================================================== */
#include "admin.h"
#include "funcoes.h"
#include "funcoes_db.h"
#include <stdio.h>

void menu_admin() {
    int op;

    do {
        printf("\n=== MENU ADMIN ===\n");
        printf("1 - Gerenciar Equipes\n");
        printf("2 - Gerenciar Gestores\n");
        printf("3 - Ranking Geral\n");
        printf("4 - Consultar Log\n");
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
        case 4:
            // AQUI: chamar a funcao 
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

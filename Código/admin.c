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
    char opcao[TAM];

    do {
        printf("\n=== MENU ADMIN ===\n");
        printf("1 - Gerenciar Equipes\n");
        printf("2 - Gerenciar Gestores\n");
        printf("3 - Gerenciar Usuarios\n");
        printf("4 - Ranking Geral\n");
        printf("5 - Consultar Log\n");
        printf("0 - Sair da conta\n");
        // ler_texto("Escolha: ", opcao, TAM);
        scanf("%d", &op);

        switch (op) {
        case 1:
            // AQUI: chamar a funcao de Gerenciar Equipes
            break;

        case 2:
            // AQUI: chamar a funcao de Gerenciar Gestores
            break;
        case 3:
            // AQUI: chamar a funcao de Gerenciar Usuarios
            break;
        case 4:
            // AQUI: chamar a funcao de Ranking Geral
            break;
        case 5:
            // AQUI: chamar a funcao de Consultar Log
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida.\n");
        }
    } while (opcao[0] != '0');
}

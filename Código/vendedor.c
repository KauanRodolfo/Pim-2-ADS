/* ==========================================================
   vendedor.c
   Menus do vendedor.
   ========================================================== */
#include "vendedor.h"
#include "cliente.h"
#include "contrato.h"
#include "ranking.h"
#include "util.h"
#include "banco.h"
#include <stdio.h>


/* ==========================================================
   Menu principal do vendedor
   ========================================================== */

void menu_vendedor(Usuario u) {
    int op;

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|                      MENU VENDEDOR                       |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Gerenciar clientes                                 |\n");
        printf("|   2 - Gerenciar contratos                                |\n");
        printf("|   3 - Ver Ranking Geral                                  |\n");
        printf("|   4 - Ver Ranking da Equipe                              |\n");
        printf("|   0 - Sair da conta                                      |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                gerenciar_clientes();
                break;

            case 2:
                contratos();
                break;

            case 3:
                ranking_geral();
                break;

            case 4:
                ranking_equipe(u.equipe_id);
                break;

            case 0:
                printf("|                    Saindo da conta...                    |\n");
                break;

            default:
                printf("|                     Opcao invalida.                      |\n");
        }

    } while (op != 0);
}

/* ==========================================================
   Gerenciamento de contratos
   ========================================================== */

void contratos() {
    int op;

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|                   GERENCIAR CONTRATOS                    |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Cadastrar contratos                                |\n");
        printf("|   2 - Consultar contratos                                |\n");
        printf("|   0 - Voltar                                             |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                cadastro_contratos();
                break;

            case 2:
                consultar('t', NULL);
                break;

            case 0:
                printf("|             Voltando ao menu do vendedor...              |\n");
                break;

            default:
                printf("|                     Opcao invalida.                      |\n");
        }

    } while (op != 0);
}

/* ==========================================================
   Gerenciamento de clientes
   ========================================================== */

void gerenciar_clientes() {
    int op;

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|                    GERENCIAR CLIENTES                    |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Cadastrar cliente                                  |\n");
        printf("|   2 - Consultar clientes                                 |\n");
        printf("|   0 - Voltar                                             |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                cadastro_clientes();
                break;

            case 2:
                consultar('c', NULL);
                break;

            case 0:
                printf("|             Voltando ao menu do vendedor...              |\n");
                break;

            default:
                printf("|                     Opcao invalida.                      |\n");
        }

    } while (op != 0);
}

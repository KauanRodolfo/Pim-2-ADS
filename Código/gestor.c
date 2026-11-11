/* ==========================================================
   gestor.c
   Menus do gestor.
   ========================================================== */
#include "gestor.h"
#include "usuario.h"
#include "contrato.h"
#include "ranking.h"
#include "util.h"
#include "banco.h"
#include <stdio.h>


/* ==========================================================
Menu principal do gestor
========================================================== */

void menu_gestor(Usuario u) {
    int op;

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|                       MENU GESTOR                        |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Gerenciar Vendedores                               |\n");
        printf("|   2 - Gerenciar Contratos                                |\n");
        printf("|   3 - Ranking da Equipe                                  |\n");
        printf("|   0 - Sair da conta                                      |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                gerenciar_vendedores();
                break;

            case 2:
                gerenciar_contratos(u);
                break;

            case 3:
                ranking_equipe_gestor(u);
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
Gerenciamento de vendedores
========================================================== */

void gerenciar_vendedores() {
    int op;

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|                   GERENCIAR VENDEDORES                   |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Listar vendedores                                  |\n");
        printf("|   2 - Cadastrar vendedor                                 |\n");
        printf("|   3 - Atualizar vendedor                                 |\n");
        printf("|   4 - Arquivar vendedor                                  |\n");
        printf("|   0 - Voltar                                             |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                consultar('u', "nivel = 'vendedor'");
                break;

            case 2:
                cadastro_vendedor();
                break;

            case 3:
                atualizar_vendedor();
                break;

            case 4:
                //excluir();
                arquivar_vendedor();
                break;

            case 0:
                printf("|              Voltando ao menu do gestor...               |\n");
                break;

            default:
                printf("|                     Opcao invalida.                      |\n");
        }

    } while (op != 0);
}

void gerenciar_contratos(Usuario u) {
    int op;

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|                   GERENCIAR CONTRATOS                    |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Listar contratos                                   |\n");
        printf("|   2 - Aprovar ou reprovar contrato                       |\n");
        printf("|   3 - Consultar contratos da equipe                      |\n");
        printf("|   0 - Voltar                                             |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                consultar('t', NULL);//lista todos os tipos de contratos da propria equipe
                break;

            case 2:
                avaliar_contrato(u);
                break;

            case 3:
                consultar('t', NULL);
                break;

            case 0:
                printf("|              Voltando ao menu do gestor...               |\n");
                break;

            default:
                printf("|                     Opcao invalida.                      |\n");
        }

    } while (op != 0);
}

void ranking_equipe_gestor(Usuario u) {
    int op;
    

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|                    RANKING DA EQUIPE                     |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Ver ranking da equipe                              |\n");
        printf("|   2 - Ver comissoes dos vendedores                       |\n");
        printf("|   0 - Voltar                                             |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                ranking_equipe(u.equipe_id);
                break;

            case 2:
                comissao_equipe(u.equipe_id);
                break;

            case 0:
                printf("|              Voltando ao menu do gestor...               |\n");
                break;

            default:
                printf("|                     Opcao invalida.                      |\n");
        }

    } while (op != 0);
}

/* ==========================================================
   admin.c
   Menus do admin.
   ========================================================== */
#include "admin.h"
#include "usuario.h"
#include "equipe.h"
#include "util.h"
#include "banco.h"
#include "competicao.h"
#include "ranking.h"
#include <stdio.h>
#include <string.h>


/* ==========================================================
   Menu principal do admin
   ========================================================== */

void menu_admin(Usuario u) {
    int op;

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|                        MENU ADMIN                        |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Gerenciar Equipes                                  |\n");
        printf("|   2 - Gerenciar Gestores                                 |\n");
        printf("|   3 - Gerenciar Competicoes                              |\n");
        printf("|   4 - Ranking Geral                                      |\n");
        printf("|   5 - Consultar Log                                      |\n");
        printf("|   0 - Sair da conta                                      |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        if (scanf("%d", &op) != 1) {
            op = -1;
        }
        limpar_buffer();

        switch (op) {
            case 1:
                gerenciar_equipes();
                break;

            case 2:
                gerenciar_gestores();
                break;

            case 3:
                gerenciar_competicoes(u);
                break;

            case 4:
                ranking_geral_admin();//talvez nao exista
                break;

            case 5:
                consultar_log();
                break;

            case 0:
                printf("|   Saindo da conta...                                     |\n"); //futuro logout
                break;

            default:
                printf("|   Opcao invalida.                                        |\n");
        }

    } while (op != 0);
}

/* ==========================================================
   Gerenciamento de gestores
   ========================================================== */

void gerenciar_gestores() {
    int op;

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|                    GERENCIAR GESTORES                    |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Cadastrar gestor                                   |\n");
        printf("|   2 - Consultar gestores                                 |\n");
        printf("|   3 - Atualizar gestor                                   |\n");
        printf("|   4 - Arquivar gestor                                    |\n");
        printf("|   0 - Voltar                                             |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        if (scanf("%d", &op) != 1) {
            op = -1;
        }
        limpar_buffer();

        switch (op) {
            case 1:
                cadastro_gestor(); // id da equipe nao pode ser menor que zero
                break;

            case 2:
                consultar('u', "nivel = 'gestor'");
                break;

            case 3:
                atualizar_gestor();
                break;

            case 4:
                arquivar_gestor();
                break;

            case 0:
                printf("|   Voltando ao menu do admin...                           |\n");
                break;

            default:
                printf("|   Opcao invalida.                                        |\n");
        }

    } while (op != 0);
}

void gerenciar_equipes() {
    int op;

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|                    GERENCIAR EQUIPES                     |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Cadastrar equipe                                   |\n");
        printf("|   2 - Consultar equipes                                  |\n");
        printf("|   3 - Definir gestor da equipe                           |\n");
        printf("|   4 - Mover vendedor de equipe                           |\n");
        printf("|   5 - Arquivar equipe                                    |\n");
        printf("|   0 - Voltar                                             |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        if (scanf("%d", &op) != 1) {
            op = -1;
        }
        limpar_buffer();
        

        switch (op) {
            case 1:
                cadastro_equipe();
                break;

            case 2:
                consultar('e', NULL);
                break;

            case 3:
                definir_gestor_equipe();
                break;

            case 4:
                mover_vendedor_equipe();
                break;

            case 5:
                arquivar_equipe();
                break;

            case 0:
                printf("|   Voltando ao menu do admin...                           |\n");
                break;

            default:
                printf("|   Opcao invalida.                                        |\n");
        }

    } while (op != 0);
}

/* ==========================================================
   Gerenciamento de competicoes
   ========================================================== */

void gerenciar_competicoes(Usuario u) {
    int op;

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|                 GERENCIAR COMPETICOES                    |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Cadastrar competicao geral                         |\n");
        printf("|   2 - Encerrar competicao                                |\n");
        printf("|   3 - Consultar competicoes                              |\n");
        printf("|   0 - Voltar                                             |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        if (scanf("%d", &op) != 1) {
            op = -1;
        }
        limpar_buffer();

        switch (op) {
            case 1:
                competicao_geral(u);
                break;

            case 2:
                printf("|   Encerramento de competicao em desenvolvimento.         |\n");
                break;

            case 3:
                consultar('o', NULL);
                break;

            case 0:
                printf("|   Voltando ao menu do admin...                           |\n");
                break;

            default:
                printf("|   Opcao invalida.                                        |\n");
        }

    } while (op != 0);
}

/* ==========================================================
   Visualizacao do Ranking Geral
   ========================================================== */

void ranking_geral_admin() {
    int op;
    int equipe_id;

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|                      RANKING GERAL                       |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Ver ranking geral                                  |\n");
        printf("|   2 - Ver ranking por equipe                             |\n");
        printf("|   0 - Voltar                                             |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        if (scanf("%d", &op) != 1) {
            op = -1;
        }
        limpar_buffer();

        switch (op) {
            case 1:
                ranking_geral();
                break;

            case 2:
                printf("|   ID da equipe: ");
                if (scanf("%d", &equipe_id) != 1) {
                    equipe_id = 0;
                }
                limpar_buffer();
                ranking_equipe(equipe_id);
                break;

            case 0:
                printf("|   Voltando ao menu do admin...                           |\n");
                break;

            default:
                printf("|   Opcao invalida.                                        |\n");
        }

    } while (op != 0);
}

/* ==========================================================
   Consulta dos logs de auditoria
   ========================================================== */

/* Mostra todos os eventos do log, de 10 em 10 */
void consultar_log() {
    printf("\n+----------------------------------------------------------+\n");
    printf("|                      CONSULTAR LOG                       |\n");
    printf("+----------------------------------------------------------+\n");

    consultar('l', NULL);
}

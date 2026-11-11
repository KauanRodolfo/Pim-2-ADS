/* ==========================================================
   admin.c
   Menus do admin.
   ========================================================== */
#include "admin.h"
#include "usuario.h"
#include "equipe.h"
#include "util.h"
#include "banco.h"
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
        printf("|   3 - Gerenciar Ciclos de Apuracao                       |\n");
        printf("|   4 - Ranking Geral                                      |\n");
        printf("|   5 - Consultar Log                                      |\n");
        printf("|   0 - Sair da conta                                      |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                gerenciar_equipes();
                break;

            case 2:
                gerenciar_gestores();
                break;

            case 3:
                gerenciar_ciclos();//falta implementar
                break;

            case 4:
                ranking_geral_admin();//talvez nao exista
                break;

            case 5:
                consultar_log();
                break;

            case 0:
                printf("|                    Saindo da conta...                    |\n"); //futuro logout
                break;

            default:
                printf("|                     Opcao invalida.                      |\n");
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
        scanf("%d", &op);

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
                printf("|               Voltando ao menu do admin...               |\n");
                break;

            default:
                printf("|                     Opcao invalida.                      |\n");
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
        scanf("%d", &op);
        

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
                printf("|               Voltando ao menu do admin...               |\n");
                break;

            default:
                printf("|                     Opcao invalida.                      |\n");
        }

    } while (op != 0);
}

/* ==========================================================
   Gerenciamento de ciclos de apuracao
   ========================================================== */

void gerenciar_ciclos() {
    int op;

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|               GERENCIAR CICLOS DE APURACAO               |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Abrir ciclo                                        |\n");
        printf("|   2 - Encerrar ciclo                                     |\n");
        printf("|   3 - Consultar ciclos                                   |\n");
        printf("|   0 - Voltar                                             |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                printf("|          Abertura de ciclo em desenvolvimento.           |\n");
                break;

            case 2:
                printf("|        Encerramento de ciclo em desenvolvimento.         |\n");
                break;

            case 3:
                consultar('o', NULL);
                break;

            case 0:
                printf("|               Voltando ao menu do admin...               |\n");
                break;

            default:
                printf("|                     Opcao invalida.                      |\n");
        }

    } while (op != 0);
}

/* ==========================================================
   Visualizacao do Ranking Geral
   ========================================================== */

void ranking_geral_admin() {
    int op;

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|                      RANKING GERAL                       |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Ver ranking geral                                  |\n");
        printf("|   2 - Ver ranking por equipe                             |\n");
        printf("|   0 - Voltar                                             |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                printf("|            Ranking geral em desenvolvimento.             |\n");
                break;

            case 2:
                printf("|          Ranking por equipe em desenvolvimento.          |\n");
                break;

            case 0:
                printf("|               Voltando ao menu do admin...               |\n");
                break;

            default:
                printf("|                     Opcao invalida.                      |\n");
        }

    } while (op != 0);
}

/* ==========================================================
   Consulta dos logs de auditoria
   ========================================================== */

void consultar_log() {
    int op;
    int usuario_id;
    char data[TAM_DATA + 2];
    char filtro[100];

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|                      CONSULTAR LOG                       |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Ver todos os eventos                               |\n");
        printf("|   2 - Filtrar por usuario                                |\n");
        printf("|   3 - Filtrar por data                                   |\n");
        printf("|   0 - Voltar                                             |\n");

        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                consultar('l', NULL);
                break;

            case 2:
                printf("|   Id do usuario: ");
                if (scanf("%d", &usuario_id) == 1) {
                    snprintf(filtro, sizeof(filtro), "usuario_id = %d", usuario_id);
                    consultar('l', filtro);
                } else {
                    printf("|                       Id invalido.                       |\n");
                    limpar_buffer();
                }
                break;

            case 3:
                printf("|   Data (AAAA-MM-DD): ");
                limpar_buffer();
                fgets(data, sizeof(data), stdin);
                tratar_texto(data);
                snprintf(filtro, sizeof(filtro), "date(data_hora) = '%s'", data);
                consultar('l', filtro);
                break;

            case 0:
                printf("|               Voltando ao menu do admin...               |\n");
                break;

            default:
                printf("|                     Opcao invalida.                      |\n");
        }

    } while (op != 0);
}

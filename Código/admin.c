/* ==========================================================
   admin.c
   Tela do admin. Os menus e submenus estao prontos; as opcoes
   ainda nao fazem nada (as funcoes de cada uma serao feitas depois).
   ========================================================== */
#include "admin.h"
#include "equipe.h"
#include "funcoes.h"
#include "funcoes_db.h"
#include <stdio.h>
#include <string.h>

/* ==========================================================
   Menu principal do admin
   ========================================================== */

void menu_admin(Usuario u) {
    int op;

    do {
        printf("\n=== MENU ADMIN ===\n");
        printf("1 - Gerenciar Equipes\n");
        printf("2 - Gerenciar Gestores\n");
        printf("3 - Gerenciar Ciclos de Apuracao\n");
        printf("4 - Ranking Geral\n");
        printf("5 - Consultar Log\n");
        printf("0 - Sair da conta\n");

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
                printf("Saindo da conta...\n"); //futuro logout
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (op != 0);
}

/* ==========================================================
   Gerenciamento de gestores
   ========================================================== */

void gerenciar_gestores() {
    int op;

    do {
        printf("\n=== GERENCIAR GESTORES ===\n");
        printf("1 - Cadastrar gestor\n");
        printf("2 - Consultar gestores\n");
        printf("3 - Atualizar gestor\n");
        printf("4 - Arquivar gestor\n");
        printf("0 - Voltar\n");

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
                printf("Voltando ao menu do admin...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (op != 0);
}

int cadastro_gestor() {
    Usuario gestor;
    char confirmar;
    int i;
    int rc;

    limpar_buffer();

    printf("\n=== CADASTRAR GESTOR ===\n");

    printf("Nome do gestor: ");
    fgets(gestor.nome, sizeof(gestor.nome), stdin);

    printf("Nome de usuario do gestor: ");
    fgets(gestor.username, sizeof(gestor.username), stdin);

    printf("Email do gestor: ");
    fgets(gestor.email, sizeof(gestor.email), stdin);

    printf("CPF do gestor: ");
    fgets(gestor.cpf, sizeof(gestor.cpf), stdin);

    for (i = 6; i < 11; i++) {
        gestor.senha[i - 6] = gestor.cpf[i];
    }
    gestor.senha[5] = '\0';
    gestor.equipe_id = 0;
    strcpy(gestor.nivel, "gestor");
    strcpy(gestor.status, "ativo");

    printf("\n=== CONFIRMAR DADOS ===\n");
    printf("Nome: %s", gestor.nome);
    printf("Nome de usuario: %s", gestor.username);
    printf("Email: %s", gestor.email);
    printf("CPF: %s", gestor.cpf);

    printf("\nConfirmar cadastro? (s/n): ");
    scanf(" %c", &confirmar);

    if (confirmar != 's' && confirmar != 'S') {
        printf("\nCadastro cancelado.\n");
        return 1;
    }

    rc = salvar('u', NULL, &gestor, NULL, NULL, NULL);
    if (rc == 0) {
        printf("\nGestor cadastrado com sucesso.\n");
        return 0;
    }

    return 1;
}

/* ==========================================================
   Gerenciamento de ciclos de apuracao
   ========================================================== */

void gerenciar_ciclos() {
    int op;

    do {
        printf("\n=== GERENCIAR CICLOS DE APURACAO ===\n");
        printf("1 - Abrir ciclo\n");
        printf("2 - Encerrar ciclo\n");
        printf("3 - Consultar ciclos\n");
        printf("0 - Voltar\n");

        scanf("%d", &op);

        switch (op) {
            case 1:
                printf("Abertura de ciclo em desenvolvimento.\n");
                break;

            case 2:
                printf("Encerramento de ciclo em desenvolvimento.\n");
                break;

            case 3:
                consultar('o', NULL);
                break;

            case 0:
                printf("Voltando ao menu do admin...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (op != 0);
}

/* ==========================================================
   Visualizacao do Ranking Geral
   ========================================================== */

void ranking_geral_admin() {
    int op;

    do {
        printf("\n=== RANKING GERAL ===\n");
        printf("1 - Ver ranking geral\n");
        printf("2 - Ver ranking por equipe\n");
        printf("0 - Voltar\n");

        scanf("%d", &op);

        switch (op) {
            case 1:
                printf("Ranking geral em desenvolvimento.\n");
                break;

            case 2:
                printf("Ranking por equipe em desenvolvimento.\n");
                break;

            case 0:
                printf("Voltando ao menu do admin...\n");
                break;

            default:
                printf("Opcao invalida.\n");
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
        printf("\n=== CONSULTAR LOG ===\n");
        printf("1 - Ver todos os eventos\n");
        printf("2 - Filtrar por usuario\n");
        printf("3 - Filtrar por data\n");
        printf("0 - Voltar\n");

        scanf("%d", &op);

        switch (op) {
            case 1:
                consultar('l', NULL);
                break;

            case 2:
                printf("Id do usuario: ");
                if (scanf("%d", &usuario_id) == 1) {
                    snprintf(filtro, sizeof(filtro), "usuario_id = %d", usuario_id);
                    consultar('l', filtro);
                } else {
                    printf("Id invalido.\n");
                    limpar_buffer();
                }
                break;

            case 3:
                printf("Data (AAAA-MM-DD): ");
                limpar_buffer();
                fgets(data, sizeof(data), stdin);
                tratar_texto(data);
                snprintf(filtro, sizeof(filtro), "date(data_hora) = '%s'", data);
                consultar('l', filtro);
                break;

            case 0:
                printf("Voltando ao menu do admin...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (op != 0);
}
void arquivar_gestor() {
    Usuario gestor;
    char filtro[TAM];
    int op;
    printf("Selecione o gestor a ser arquivado:\n");
    consultar('u', NULL);
    printf("Entre com o ID do gestor a ser arquivado: ");

    do{
        scanf("%d", &gestor.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND status = 'ativo'", gestor.id);
        if(registro_existe('u', filtro, &gestor) == 0) {
            printf("ID do gestor nao encontrado. \n gostaria de tentar novamente? (1 para sim, 0 para nao): ");
            scanf("%d", &op);
            limpar_buffer();
            if(op == 0) {
                printf("Operacao cancelada.\n");
                return;
            }
        } else {
            break;
        }
    }while(op);

    printf("Deseja realmente arquivar o gestor %s? (1 para sim, 0 para nao): ", gestor.nome);
    scanf("%d", &op);
    limpar_buffer();

    if(op == 1) {
        if(excluir('u', gestor.id)){
            printf("Erro ao arquivar o gestor.\n");
        } else {
            printf("gestor arquivado com sucesso.\n");
        }
    } else {
        printf("Operacao cancelada.\n");
    }
}
void atualizar_gestor() {
    Usuario gestor;
    char filtro[TAM];
    int op, id;
    printf("Selecione o gestor a ser atualizado:\n");
    snprintf(filtro, sizeof(filtro), "nivel = 'gestor'");
    consultar('u', filtro);
    printf("Entre com o ID do gestor a ser atualizado: ");

    do{
        scanf("%d", &id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND status = 'ativo'", id);
        if(registro_existe('u', filtro, &gestor) == 0) {
            printf("ID do gestor nao encontrado. \n gostaria de tentar novamente? (1 para sim, 0 para nao): ");
            scanf("%d", &op);
            limpar_buffer();
            if(op == 0) {
                printf("Operacao cancelada.\n");
                return;
            }
        }else{
           break;
        }
    }while(op);

    printf("Deseja realmente atualizar o gestor %s? (1 para sim, 0 para nao): ", gestor.nome);
    scanf("%d", &op);
    limpar_buffer();

    if(op == 1) {
        if(editar('u', gestor.id) == 0) {
            printf("Gestor atualizado com sucesso.\n");
        } else {
            printf("Erro ao atualizar o gestor.\n");
        }
    } else {
        printf("Operacao cancelada.\n");
    }

}
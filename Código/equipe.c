/* ==========================================================
   equipe.c
   Gerenciamento de equipes (menu do admin).
   ========================================================== */
#include "equipe.h"
#include "funcoes.h"
#include "funcoes_db.h"
#include <stdio.h>
#include <string.h>

void gerenciar_equipes() {
    int op;

    do {
        printf("\n=== GERENCIAR EQUIPES ===\n");
        printf("1 - Cadastrar equipe\n");
        printf("2 - Consultar equipes\n");
        printf("3 - Definir gestor da equipe\n");
        printf("4 - Mover vendedor de equipe\n"); 
        printf("5 - Arquivar equipe\n");
        printf("0 - Voltar\n");

        scanf("%d", &op);
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
                printf("Voltando ao menu do admin...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (op != 0);
}


int editar_equipe_vendedor(int usuario_id, int equipe_id) {
    sqlite3 *db;
    char *erro = NULL;
    char sql[256];
    int rc;
    
    rc = sqlite3_open("pim2.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Nao foi possivel abrir o banco: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    snprintf(sql, sizeof(sql),
             "update Usuarios SET equipe_id = %d WHERE id = %d;",
             equipe_id, usuario_id);

    rc = sqlite3_exec(db, sql, NULL, NULL, &erro);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao atualizar equipe do usuario: %s\n", erro);
        sqlite3_free(erro);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_close(db);
    return 1; 
}

int editar_equipe_gestor(int equipe_id, int gestor_id) {
    sqlite3 *db;
    char *erro = NULL;
    char sql[256];
    int rc;
    
    rc = sqlite3_open("pim2.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Nao foi possivel abrir o banco: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    snprintf(sql, sizeof(sql),
             "update Equipes SET gestor_id = %d WHERE id = %d;",
             gestor_id, equipe_id);

    rc = sqlite3_exec(db, sql, NULL, NULL, &erro);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao atualizar equipe do usuario: %s\n", erro);
        sqlite3_free(erro);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_close(db);
    return 1; 
}

/* Define o gestor de uma equipe (opcao 3 do menu de equipes) */
void definir_gestor_equipe() {
    int op;
    Usuario gestor;
    Equipe equipe;
    char filtro[TAM];

    printf("Selecione a equipe para definir o gestor:\n");
    consultar('e', NULL);
    printf("Entre com o ID da equipe: ");
    do {
        scanf("%d", &equipe.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND status = 'ativa'", equipe.id);
        if(registro_existe('e', filtro, &equipe) == 0) {
            printf("ID da equipe nao encontrado. \n gostaria de tentar novamente? (1 para sim, 0 para nao): ");
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

    printf("Selecione o gestor:\n");
    consultar('u', "nivel = 'gestor'");
    printf("Entre com o ID do gestor: ");
    do {
        scanf("%d", &gestor.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND nivel = 'gestor'", gestor.id);
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

    printf("Deseja definir o gestor %s para a equipe %s? (1 para sim, 0 para nao): ", gestor.nome, equipe.nome);
    scanf("%d", &op);
    limpar_buffer();

    switch(op) {
        case 1:
            if(editar_equipe_gestor(equipe.id, gestor.id) == 0) {
                printf("Gestor definido com sucesso.\n");
                break;
            } else {
                printf("Erro ao definir o gestor.\n");
            }
            break;
        case 0:
            printf("Operacao cancelada.\n");
            break;
        default:
            printf("Opcao invalida.\n");
    }
}

/* Move um vendedor para outra equipe (opcao 4 do menu de equipes) */
void mover_vendedor_equipe() {
    int op;
    char filtro[TAM];
    Usuario vendedor;
    Equipe equipe;

    printf("Selecione o vendedor a ser movido:\n");
    consultar('u', "nivel = 'vendedor'");
    printf("Entre com o ID do vendedor: ");

    do{
        scanf("%d", &vendedor.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND nivel = 'vendedor'", vendedor.id);

        if(registro_existe('u', filtro, &vendedor) == 0) {
            printf("ID do vendedor nao encontrado. \n gostaria de tentar novamente? (1 para sim, 0 para nao): ");
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

    printf("Vendedor selecionado: %s\n", vendedor.nome);
    printf("Selecione a equipe de destino:\n");
    consultar('e', NULL);
    printf("Entre com o ID do equipe: ");

    do{
        scanf("%d", &equipe.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND status = 'ativa'", equipe.id);

        if(registro_existe('e', filtro, &equipe) == 0) {
            printf("ID da equipe nao encontrado. \nGostaria de tentar novamente? (1 para sim, 0 para nao): ");
            scanf("%d", &op);
            limpar_buffer();
            if(op == 0) {
                printf("Operacao cancelada.\n");
                return;
            }
        } else {
            break;
        }
    }while (op);

    printf("Deseja realmente mover o vendedor %s para a equipe %s? (1 para sim, 0 para nao): ", vendedor.nome, equipe.nome);
    scanf("%d", &op);
    limpar_buffer();

    if(op == 1) {
        if(editar_equipe_vendedor(vendedor.id, equipe.id) == 0) {
            printf("Vendedor movido com sucesso.\n");
            return;
        } else {
            printf("Erro ao mover o vendedor.\n");
        }
    } else {
        printf("Operacao cancelada.\n");
        return;
    }
}

/* Arquiva uma equipe (opcao 5 do menu de equipes) */
void arquivar_equipe() {
    Equipe equipe;
    char filtro[TAM];
    int op;
    printf("Selecione a equipe a ser arquivada:\n");
    consultar('e', NULL);
    printf("Entre com o ID da equipe a ser arquivada: ");

    do{
        scanf("%d", &equipe.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND status = 'ativa'", equipe.id);
        if(registro_existe('e', filtro, &equipe) == 0) {
            printf("ID da equipe nao encontrado. \n gostaria de tentar novamente? (1 para sim, 0 para nao): ");
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

    printf("Deseja realmente arquivar a equipe %s? (1 para sim, 0 para nao): ", equipe.nome);
    scanf("%d", &op);
    limpar_buffer();

    if(op == 1) {
        if(excluir('e', equipe.id)){
            printf("Erro ao arquivar a equipe.\n");
        } else {
            printf("Equipe arquivada com sucesso.\n");
        }
    } else {
        printf("Operacao cancelada.\n");
    }
}


int cadastro_equipe() {
    Equipe equipe;
    Usuario gestor;
    char confirmar;
    int rc, op;
    char filtro[TAM];

    limpar_buffer();

    printf("\n=== CADASTRAR EQUIPE ===\n");

    printf("Nome da equipe: ");
    fgets(equipe.nome, sizeof(equipe.nome), stdin);
    printf("Entre com o ID do gestor da equipe\n");
    printf("1 - Digitar ID do gestor 2 - Consultar gestores 0 - Sem gestor\n");
    scanf("%d", &op);   
    limpar_buffer(); 
    switch(op) {
        case 0:
            equipe.gestor_id = 0;
            break;
        case 1:
            printf("Entre com o ID do gestor: ");
            do {
                scanf("%d", &equipe.gestor_id);
                limpar_buffer();
                snprintf(filtro, sizeof(filtro), "id = %d AND nivel = 'gestor'", equipe.gestor_id);
                if(registro_existe('u', filtro, &gestor) == 0) {
                    printf("ID do gestor nao encontrado. \n gostaria de tentar novamente? (1 para sim, 0 para nao): ");
                    scanf("%d", &op);
                    limpar_buffer();
                    if(op == 0) {
                        printf("Operacao cancelada.\n");
                        return 1;
                    }
                } else {
                    break;
                }
            }while(op);

            break;
        case 2:
            consultar('u', "nivel = 'gestor'");
            printf("Entre com o ID do gestor: ");
            do {
                scanf("%d", &equipe.gestor_id);
                limpar_buffer();
                snprintf(filtro, sizeof(filtro), "id = %d AND nivel = 'gestor'", equipe.gestor_id);
                if(registro_existe('u', filtro, &gestor) == 0) {
                    printf("ID do gestor nao encontrado. \n gostaria de tentar novamente? (1 para sim, 0 para nao): ");
                    scanf("%d", &op);
                    limpar_buffer();
                    if(op == 0) {
                        printf("Operacao cancelada.\n");
                        return 1;
                    }
                } else {
                    break;
                }
            }while(op);
            break;
        default:
            printf("Opcao invalida.\n");
            return 1;
    }
    limpar_buffer();

    strcpy(equipe.status, "ativa");
    printf("\n=== CONFIRMAR DADOS ===\n");
    printf("Nome: %s", equipe.nome);
    printf("Gestor ID: %d Nome: %s\n ", equipe.gestor_id, gestor.nome);
    printf("Status: %s\n", equipe.status);

    printf("\nConfirmar cadastro? (s/n): ");
    scanf(" %c", &confirmar);

    if (confirmar != 's' && confirmar != 'S') {
        printf("\nCadastro cancelado.\n");
        return 1;
    }

    rc = salvar('e', &equipe, NULL, NULL, NULL, NULL);
    if (rc == 0) {
        printf("\nEquipe cadastrada com sucesso.\n");
        return 0;
    }

    return 1;
}
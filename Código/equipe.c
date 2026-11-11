/* ==========================================================
   equipe.c
   Cadastro e manutencao de equipes.
   ========================================================== */
#include "equipe.h"
#include "util.h"
#include "banco.h"
#include <stdio.h>
#include <string.h>




int editar_equipe_vendedor(int usuario_id, int equipe_id) {
    sqlite3 *db;
    char *erro = NULL;
    char sql[256];
    int rc;
    
    rc = sqlite3_open("pim2.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "|   Nao foi possivel abrir o banco: %-23s|\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    snprintf(sql, sizeof(sql),
             "update Usuarios SET equipe_id = %d WHERE id = %d;",
             equipe_id, usuario_id);

    rc = sqlite3_exec(db, sql, NULL, NULL, &erro);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "|   Erro ao atualizar equipe do usuario: %-18s|\n", erro);
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
        fprintf(stderr, "|   Nao foi possivel abrir o banco: %-23s|\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    snprintf(sql, sizeof(sql),
             "update Equipes SET gestor_id = %d WHERE id = %d;",
             gestor_id, equipe_id);

    rc = sqlite3_exec(db, sql, NULL, NULL, &erro);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "|   Erro ao atualizar equipe do usuario: %-18s|\n", erro);
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
    char filtro[TAM_FILTRO];

    printf("|        Selecione a equipe para definir o gestor:         |\n");
    printf("|   1 - Digitar ID da equipe                               |\n");
    printf("|   2 - Consultar equipes                                  |\n");
    printf("|   0 - Cancelar                                           |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Escolha uma opcao: ");
    scanf("%d", &op);
    limpar_buffer();
    if (op == 2) {
        consultar('e', "status = 'ativa'");
    } else if (op != 1) {
        printf("|                   Operacao cancelada.                    |\n");
        return;
    }
    printf("|   Entre com o ID da equipe: ");
    do {
        scanf("%d", &equipe.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND status = 'ativa'", equipe.id);
        if(registro_existe('e', filtro, &equipe) == 0) {
            printf("|               ID da equipe nao encontrado.               |\n|   gostaria de tentar novamente? (1 para sim, 0 para nao): ");
            scanf("%d", &op);
            limpar_buffer();
            if(op == 0) {
                printf("|                   Operacao cancelada.                    |\n");
                return;
            }
        } else {
            break;
        }
    }while(op);

    printf("|                   Selecione o gestor:                    |\n");
    printf("|   1 - Digitar ID do gestor                               |\n");
    printf("|   2 - Consultar gestores                                 |\n");
    printf("|   0 - Cancelar                                           |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Escolha uma opcao: ");
    scanf("%d", &op);
    limpar_buffer();
    if (op == 2) {
        consultar('u', "nivel = 'gestor' AND status = 'ativo'");
    } else if (op != 1) {
        printf("|                   Operacao cancelada.                    |\n");
        return;
    }
    printf("|   Entre com o ID do gestor: ");
    do {
        scanf("%d", &gestor.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND nivel = 'gestor' AND status = 'ativo'", gestor.id);
        if(registro_existe('u', filtro, &gestor) == 0) {
            printf("|               ID do gestor nao encontrado.               |\n|   gostaria de tentar novamente? (1 para sim, 0 para nao): ");
            scanf("%d", &op);
            limpar_buffer();
            if(op == 0) {
                printf("|                   Operacao cancelada.                    |\n");
                return;
            }
        } else {
            break;
        }
    }while(op);

    printf("|   Deseja definir o gestor %s para a equipe %s? (1 para sim, 0 para nao): ", gestor.nome, equipe.nome);
    scanf("%d", &op);
    limpar_buffer();

    switch(op) {
        case 1:
            if(editar_equipe_gestor(equipe.id, gestor.id) == 0) {
                printf("|               Gestor definido com sucesso.               |\n");
                break;
            } else {
                printf("|                Erro ao definir o gestor.                 |\n");
            }
            break;
        case 0:
            printf("|                   Operacao cancelada.                    |\n");
            break;
        default:
            printf("|                     Opcao invalida.                      |\n");
    }
}

/* Move um vendedor para outra equipe (opcao 4 do menu de equipes) */
void mover_vendedor_equipe() {
    int op;
    char filtro[TAM_FILTRO];
    Usuario vendedor;
    Equipe equipe;

    printf("|            Selecione o vendedor a ser movido:            |\n");
    printf("|   1 - Digitar ID do vendedor                             |\n");
    printf("|   2 - Consultar vendedores                               |\n");
    printf("|   0 - Cancelar                                           |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Escolha uma opcao: ");
    scanf("%d", &op);
    limpar_buffer();
    if (op == 2) {
        consultar('u', "nivel = 'vendedor' AND status = 'ativo'");
    } else if (op != 1) {
        printf("|                   Operacao cancelada.                    |\n");
        return;
    }
    printf("|   Entre com o ID do vendedor: ");

    do{
        scanf("%d", &vendedor.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND nivel = 'vendedor' AND status = 'ativo'", vendedor.id);

        if(registro_existe('u', filtro, &vendedor) == 0) {
            printf("|              ID do vendedor nao encontrado.              |\n|   gostaria de tentar novamente? (1 para sim, 0 para nao): ");
            scanf("%d", &op);
            limpar_buffer();
            if(op == 0) {
                printf("|                   Operacao cancelada.                    |\n");
                return;
            }
        } else {
            break;
        }
    }while(op);

    printf("|   Vendedor selecionado: %-33s|\n", vendedor.nome);
    printf("|              Selecione a equipe de destino:              |\n");
    printf("|   1 - Digitar ID da equipe                               |\n");
    printf("|   2 - Consultar equipes                                  |\n");
    printf("|   0 - Cancelar                                           |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Escolha uma opcao: ");
    scanf("%d", &op);
    limpar_buffer();
    if (op == 2) {
        consultar('e', "status = 'ativa'");
    } else if (op != 1) {
        printf("|                   Operacao cancelada.                    |\n");
        return;
    }
    printf("|   Entre com o ID da equipe: ");

    do{
        scanf("%d", &equipe.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND status = 'ativa'", equipe.id);

        if(registro_existe('e', filtro, &equipe) == 0) {
            printf("|               ID da equipe nao encontrado.               |\n|   Gostaria de tentar novamente? (1 para sim, 0 para nao): ");
            scanf("%d", &op);
            limpar_buffer();
            if(op == 0) {
                printf("|                   Operacao cancelada.                    |\n");
                return;
            }
        } else {
            break;
        }
    }while (op);

    printf("|   Deseja realmente mover o vendedor %s para a equipe %s? (1 para sim, 0 para nao): ", vendedor.nome, equipe.nome);
    scanf("%d", &op);
    limpar_buffer();

    if(op == 1) {
        if(editar_equipe_vendedor(vendedor.id, equipe.id) == 0) {
            printf("|               Vendedor movido com sucesso.               |\n");
            return;
        } else {
            printf("|                Erro ao mover o vendedor.                 |\n");
        }
    } else {
        printf("|                   Operacao cancelada.                    |\n");
        return;
    }
}

/* Arquiva uma equipe (opcao 5 do menu de equipes) */
void arquivar_equipe() {
    Equipe equipe;
    char filtro[TAM];
    int op;
    printf("|           Selecione a equipe a ser arquivada:            |\n");
    printf("|   1 - Digitar ID da equipe                               |\n");
    printf("|   2 - Consultar equipes                                  |\n");
    printf("|   0 - Cancelar                                           |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Escolha uma opcao: ");
    scanf("%d", &op);
    limpar_buffer();
    if (op == 2) {
        consultar('e', "status = 'ativa'");
    } else if (op != 1) {
        printf("|                   Operacao cancelada.                    |\n");
        return;
    }
    printf("|   Entre com o ID da equipe a ser arquivada: ");

    do{
        scanf("%d", &equipe.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND status = 'ativa'", equipe.id);
        if(registro_existe('e', filtro, &equipe) == 0) {
            printf("|               ID da equipe nao encontrado.               |\n|   gostaria de tentar novamente? (1 para sim, 0 para nao): ");
            scanf("%d", &op);
            limpar_buffer();
            if(op == 0) {
                printf("|                   Operacao cancelada.                    |\n");
                return;
            }
        } else {
            break;
        }
    }while(op);

    printf("|   Deseja realmente arquivar a equipe %s? (1 para sim, 0 para nao): ", equipe.nome);
    scanf("%d", &op);
    limpar_buffer();

    if(op == 1) {
        if(excluir('e', equipe.id)){
            printf("|                Erro ao arquivar a equipe.                |\n");
        } else {
            printf("|              Equipe arquivada com sucesso.               |\n");
        }
    } else {
        printf("|                   Operacao cancelada.                    |\n");
    }
}


int cadastro_equipe() {
    Equipe equipe;
    Usuario gestor;
    char confirmar;
    int rc, op;
    char filtro[TAM_FILTRO];

    limpar_buffer();

    printf("\n+----------------------------------------------------------+\n");
    printf("|                     CADASTRAR EQUIPE                     |\n");
    printf("+----------------------------------------------------------+\n");

    printf("|   Nome da equipe: ");
    fgets(equipe.nome, sizeof(equipe.nome), stdin);
    tratar_texto(equipe.nome);
    printf("|   A equipe precisa de um lider. Entre com o ID do gestor da equipe|\n");
    printf("|   1 - Digitar ID do gestor                               |\n");
    printf("|   2 - Consultar gestores                                 |\n");
    printf("|   0 - Sem gestor                                         |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Escolha uma opcao: ");
    scanf("%d", &op);   
    limpar_buffer(); 
    if (op == 0) {
        equipe.gestor_id = 0;
        strcpy(gestor.nome, "sem gestor");
    } else {
        if (op == 2) {
            consultar('u', "nivel = 'gestor' AND status = 'ativo'");
        } else if (op != 1) {
            printf("|                     Opcao invalida.                      |\n");
            return 1;
        }
        printf("|   Entre com o ID do gestor: ");
        do {
            scanf("%d", &equipe.gestor_id);
            limpar_buffer();
            snprintf(filtro, sizeof(filtro), "id = %d AND nivel = 'gestor' AND status = 'ativo'", equipe.gestor_id);
            if(registro_existe('u', filtro, &gestor) == 0) {
                printf("|               ID do gestor nao encontrado.               |\n|   Gostaria de tentar novamente? (1 para sim, 0 para nao): ");
                scanf("%d", &op);
                limpar_buffer();
                if(op == 0) {
                    printf("|                   Operacao cancelada.                    |\n");
                    return 1;
                }
            } else {
                break;
            }
        }while(op);
    }

    strcpy(equipe.status, "ativa");
    printf("\n+----------------------------------------------------------+\n");
    printf("|                     CONFIRMAR DADOS                      |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Nome: %-49s|\n", equipe.nome);
    printf("|   Gestor ID: %-44d|\n", equipe.gestor_id);
    printf("|   Gestor: %-47s|\n", gestor.nome);
    printf("|   Status: %-47s|\n", equipe.status);

    printf("|   Confirmar cadastro? (s/n): ");
    scanf(" %c", &confirmar);

    if (confirmar != 's' && confirmar != 'S') {
        printf("|                   Cadastro cancelado.                    |\n");
        return 1;
    }

    rc = salvar('e', &equipe, NULL, NULL, NULL, NULL);
    if (rc == 0) {
        printf("|              Equipe cadastrada com sucesso.              |\n");
        return 0;
    }

    return 1;
}

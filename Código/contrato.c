/* ==========================================================
   contrato.c
   Cadastro e avaliacao de contratos.
   ========================================================== */
#include "contrato.h"
#include "util.h"
#include "banco.h"
#include <stdio.h>
#include <string.h>
#include <time.h>


/* ==========================================================
   Cadastro de contratos
   ========================================================== */

void cadastro_contratos() {
    Contrato contrato;
    char confirmar;
    char filtro[TAM];
    int op;
    time_t agora;

    printf("\n+----------------------------------------------------------+\n");
    printf("|                    CADASTRAR CONTRATO                    |\n");
    printf("+----------------------------------------------------------+\n");

    printf("|   1 - Digitar ID do cliente                              |\n");
    printf("|   2 - Consultar clientes                                 |\n");
    printf("|   0 - Cancelar                                           |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Escolha uma opcao: ");
    scanf("%d", &op);
    limpar_buffer();
    if (op == 2) {
        consultar('c', "status = 'ativo'");
    } else if (op != 1) {
        printf("|                   Operacao cancelada.                    |\n");
        return;
    }
    printf("|   ID do cliente: ");
    do{
        scanf("%d", &contrato.cliente_id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND status = 'ativo'", contrato.cliente_id);
        if(registro_existe('c', filtro, NULL) == 0) {
            printf("|              ID do cliente nao encontrado.               |\n|   gostaria de tentar novamente? (1 para sim, 0 para nao): ");
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

    printf("|   ID do vendedor: ");  //precisa ser o id do vendedor logado
    if (scanf("%d", &contrato.usuario_id) != 1 ||
        contrato.usuario_id <= 0) {
        printf("|                 ID do vendedor invalido.                 |\n");
        return;
    }

    printf("|   ID do tipo de consorcio: ");
    if (scanf("%d", &contrato.tipo_consorcio_id) != 1 ||
        contrato.tipo_consorcio_id <= 0) {
        printf("|               Tipo de consorcio invalido.                |\n");
        return;
    }

    printf("|   Valor do credito: R$ ");
    if (scanf("%f", &contrato.valor_credito) != 1 ||
        contrato.valor_credito <= 0) {
        printf("|                     Valor invalido.                      |\n");
        return;
    }

    // Status inicial do contrato
    strcpy(contrato.status, "em_analise");

    // Exibe os dados informados
    printf("\n+----------------------------------------------------------+\n");
    printf("|                    DADOS DO CONTRATO                     |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Cliente ID: %-43d|\n", contrato.cliente_id);
    printf("|   Vendedor ID: %-42d|\n", contrato.usuario_id);
    printf("|   Tipo de consorcio ID: %-33d|\n", contrato.tipo_consorcio_id);
    printf("|   Valor do credito: R$ %-34.2f|\n", contrato.valor_credito);
    printf("|   Status: %-47s|\n", contrato.status);

    // Confirmacao do contrato
    printf("|   Confirmar cadastro do contrato? (s/n): ");
    scanf(" %c", &confirmar);

    if (confirmar == 's' || confirmar == 'S') {
        /* Campos que o vendedor nao digita */
        agora = time(NULL);
        strftime(contrato.data_contrato, sizeof(contrato.data_contrato),
                 "%Y-%m-%d", localtime(&agora));
        contrato.avaliado_por = 0;
        contrato.data_avaliacao[0] = '\0';
        contrato.motivo_reprovacao[0] = '\0';

        if (salvar('t', NULL, NULL, NULL, &contrato, NULL) == 0) {
            printf("|            Contrato cadastrado com sucesso.              |\n");
        } else {
            printf("|               Erro ao cadastrar contrato.                |\n");
        }

    } else if (confirmar == 'n' || confirmar == 'N') {
        printf("|             Cadastro do contrato cancelado.              |\n");

    } else {
        printf("|           Opcao invalida. Cadastro cancelado.            |\n");
    }
}

/* ==========================================================
Analise de contratos (aprovar / reprovar)
========================================================== */

/* Lista os contratos em analise da equipe do gestor, o gestor escolhe
   um e decide se aprova ou reprova (com motivo opcional). */
void avaliar_contrato(Usuario u) {
    sqlite3 *db;
    char *erro = NULL;
    char sql[1024];
    char filtro[256];
    char busca[300];
    char motivo[TAM_TEXTO + 2];
    int id, op;
    Contrato contrato;

    snprintf(filtro, sizeof(filtro),
             "status = 'em_analise' AND vendedor_id IN "
             "(SELECT id FROM Usuarios WHERE equipe_id = %d)", u.equipe_id);

    printf("|   1 - Digitar ID do contrato                             |\n");
    printf("|   2 - Consultar contratos em analise                     |\n");
    printf("|   0 - Cancelar                                           |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Escolha uma opcao: ");
    scanf("%d", &op);
    limpar_buffer();
    if (op == 2) {
        printf("|           Contratos em analise da sua equipe:            |\n");
        consultar('t', filtro);
    } else if (op != 1) {
        printf("|                   Operacao cancelada.                    |\n");
        return;
    }

    printf("|   Entre com o ID do contrato: ");
    do{
        scanf("%d", &id);
        limpar_buffer();
        snprintf(busca, sizeof(busca), "id = %d AND %s", id, filtro);
        if(registro_existe('t', busca, &contrato) == 0) {
            printf("|              ID do contrato nao encontrado.              |\n|   gostaria de tentar novamente? (1 para sim, 0 para nao): ");
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

    printf("|   Contrato: %-45d|\n", contrato.id);
    printf("|   Cliente: %-46d|\n", contrato.cliente_id);
    printf("|   Vendedor: %-45d|\n", contrato.usuario_id);
    printf("|   Valor: R$ %-45.2f|\n", contrato.valor_credito);
    printf("|   Data: %-49s|\n", contrato.data_contrato);
    printf("|   Status: %-47s|\n", contrato.status);
    snprintf(busca, sizeof(busca), "id = %d", contrato.cliente_id);
    consultar('c', busca);
    printf("|   1 - Aprovar                                            |\n|   2 - Reprovar                                           |\n|   0 - Cancelar                                           |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Escolha uma opcao: ");
    scanf("%d", &op);
    limpar_buffer();

    if (op == 1) {
        snprintf(sql, sizeof(sql),
                 "UPDATE Contratos SET status = 'aprovado', avaliado_por = %d, "
                 "data_avaliacao = datetime('now','localtime') "
                 "WHERE id = %d;", u.id, id);
    } else if (op == 2) {
        printf("|   Deseja deixar um motivo? (1 para sim, 0 para nao): ");
        scanf("%d", &op);
        limpar_buffer();

        if (op == 1) {
            printf("|   Motivo da reprovacao: ");
            fgets(motivo, sizeof(motivo), stdin);
            tratar_texto(motivo);
            snprintf(sql, sizeof(sql),
                     "UPDATE Contratos SET status = 'reprovado', avaliado_por = %d, "
                     "data_avaliacao = datetime('now','localtime'), motivo_reprovacao = '%s' "
                     "WHERE id = %d;", u.id, motivo, id);
        } else {
            snprintf(sql, sizeof(sql),
                     "UPDATE Contratos SET status = 'reprovado', avaliado_por = %d, "
                     "data_avaliacao = datetime('now','localtime') "
                     "WHERE id = %d;", u.id, id);
        }
    } else {
        printf("|                   Operacao cancelada.                    |\n");
        return;
    }

    if (sqlite3_open("pim2.db", &db) != SQLITE_OK) {
        printf("|   Erro ao abrir o banco: %-32s|\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    if (sqlite3_exec(db, sql, NULL, NULL, &erro) != SQLITE_OK) {
        printf("|   Erro ao atualizar contrato: %-27s|\n", erro);
        sqlite3_free(erro);
    } else {
        printf("|             Contrato atualizado com sucesso.             |\n");
    }

    sqlite3_close(db);
}

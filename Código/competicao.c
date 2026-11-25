/* ==========================================================
   competicao.c
   Competicoes entre vendedores.
   ========================================================== */
#include "competicao.h"
#include "util.h"
#include "banco.h"
#include <stdio.h>
#include <string.h>


/* Cadastra uma competicao interna para a equipe do gestor */
void competicao_equipe(Usuario gestor) {
    Competicao competicao;
    int op;
    char data[TAM_DATA + 2];   /* variavel temporaria */
    char filtro[128];          /* condicao usada na busca */

    if (gestor.equipe_id == 0) {
        printf("|   Voce nao esta vinculado a nenhuma equipe.              |\n");
        return;
    }

    /* So pode cadastrar se a equipe nao tiver competicao em andamento */
    snprintf(filtro, sizeof(filtro),
             "equipe_id = %d AND status = 'aberta' AND data_fim >= date('now','localtime')",
             gestor.equipe_id);
    if (registro_existe('o', filtro, NULL)) {
        printf("|   Ja existe uma competicao em andamento na equipe.       |\n");
        return;
    }

    printf("\n+----------------------------------------------------------+\n");
    printf("|   CADASTRAR COMPETICAO DA EQUIPE                         |\n");
    printf("+----------------------------------------------------------+\n");

    printf("|   Nome da competicao: ");
    fgets(competicao.nome, sizeof(competicao.nome), stdin);

    printf("|   Data de inicio (dd/mm/aaaa): ");
    fgets(data, sizeof(data), stdin);
    sprintf(competicao.data_inicio, "%.4s-%.2s-%.2s", data + 6, data + 3, data);

    printf("|   Data de fim (dd/mm/aaaa): ");
    fgets(data, sizeof(data), stdin);
    sprintf(competicao.data_fim, "%.4s-%.2s-%.2s", data + 6, data + 3, data);

    strcpy(competicao.tipo, "interna");
    competicao.equipe_id = gestor.equipe_id;
    competicao.criada_por = gestor.id;

    printf("|   Deseja cadastrar a competicao? (1 para sim, 0 para nao): ");
    if (scanf("%d", &op) != 1) {
        op = -1;
    }
    limpar_buffer();
    if (op != 1) {
        printf("|   Operacao cancelada.                                    |\n");
        return;
    }

    if (salvar('o', NULL, NULL, NULL, NULL, &competicao) == 0) {
        printf("|   Competicao cadastrada com sucesso.                     |\n");
    } else {
        printf("|   Erro ao cadastrar competicao.                          |\n");
    }
}

/* Cadastra uma competicao geral (vale para todos os vendedores) */
void competicao_geral(Usuario admin) {
    Competicao competicao;
    char data[TAM_DATA + 2];   /* guarda a data digitada (dd/mm/aaaa) */
    int op;

    printf("\n+----------------------------------------------------------+\n");
    printf("|   CADASTRAR COMPETICAO GERAL                             |\n");
    printf("+----------------------------------------------------------+\n");

    /* So pode cadastrar se nao tiver competicao geral em andamento */
    if (registro_existe('o', "tipo = 'geral' AND status = 'aberta' "
                             "AND data_fim >= date('now','localtime')", NULL)) {
        printf("|   Ja existe uma competicao geral em andamento.           |\n");
        return;
    }

    /* 1) Le o nome */
    printf("|   Nome da competicao: ");
    fgets(competicao.nome, sizeof(competicao.nome), stdin);

    /* 2) Le a data de inicio e converte de dd/mm/aaaa para aaaa-mm-dd */
    printf("|   Data de inicio (dd/mm/aaaa): ");
    fgets(data, sizeof(data), stdin);
    sprintf(competicao.data_inicio, "%.4s-%.2s-%.2s", data + 6, data + 3, data);

    /* 3) Le a data de fim e converte do mesmo jeito */
    printf("|   Data de fim (dd/mm/aaaa): ");
    fgets(data, sizeof(data), stdin);
    sprintf(competicao.data_fim, "%.4s-%.2s-%.2s", data + 6, data + 3, data);

    /* 4) Preenche o resto: geral nao tem equipe (0) */
    strcpy(competicao.tipo, "geral");
    competicao.equipe_id = 0;
    competicao.criada_por = admin.id;

    /* 5) Confirma antes de salvar */
    printf("|   Deseja cadastrar a competicao? (1 para sim, 0 para nao): ");
    if (scanf("%d", &op) != 1) {
        op = -1;
    }
    limpar_buffer();

    if (op != 1) {
        printf("|   Operacao cancelada.                                    |\n");
        return;
    }

    /* 6) Salva no banco */
    if (salvar('o', NULL, NULL, NULL, NULL, &competicao) == 0) {
        printf("|   Competicao cadastrada com sucesso.                     |\n");
    } else {
        printf("|   Erro ao cadastrar competicao.                          |\n");
    }
}

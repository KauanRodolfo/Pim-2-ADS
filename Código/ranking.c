/* ==========================================================
   ranking.c
   Rankings e comissoes.
   ========================================================== */
#include "ranking.h"
#include "banco.h"
#include <stdio.h>
#include <stdlib.h>


/* Arquiva as competicoes que ja terminaram e grava uma linha de log
   para cada uma. O "RETURNING id, vencedor_id" no fim do UPDATE faz o
   banco devolver o id e o vencedor de cada competicao arquivada. */
void arquivar_competicoes_encerradas(sqlite3 *db, const char *sql_arquivar) {
    char **resultado = NULL;
    char detalhes[100];
    int linhas = 0, colunas = 0, i;

    if (sqlite3_get_table(db, sql_arquivar, &resultado, &linhas, &colunas, NULL) != SQLITE_OK) {
        sqlite3_free_table(resultado);
        return;
    }

    /* resultado[0] e resultado[1] sao os nomes das colunas (id, vencedor_id).
       A linha i fica em resultado[i * 2] (id) e resultado[i * 2 + 1] (vencedor). */
    for (i = 1; i <= linhas; i++) {
        snprintf(detalhes, sizeof(detalhes), "encerrada automaticamente, vencedor id %s",
                 resultado[i * 2 + 1] != NULL ? resultado[i * 2 + 1] : "nenhum");
        registrar_log("arquivamento", "Competicoes", atoi(resultado[i * 2]), detalhes);
    }

    sqlite3_free_table(resultado);
}

void ranking_geral() {
    sqlite3 *db;
    int posicao = 0;

    /* Conta so os contratos feitos dentro do periodo da competicao aberta */
    const char *sql =
        "SELECT u.id, u.nome, COUNT(c.id) AS contratos, SUM(c.valor_credito) AS total "
        "FROM Usuarios u "
        "JOIN Contratos c ON u.id = c.vendedor_id "
        "JOIN Competicoes co ON co.tipo = 'geral' AND co.status = 'aberta' "
        "WHERE u.nivel = 'vendedor' AND c.status = 'aprovado' "
        "  AND c.data_contrato BETWEEN co.data_inicio AND co.data_fim "
        "GROUP BY u.id "
        "ORDER BY total DESC;";

    /* Grava o vencedor das competicoes gerais que ja terminaram */
    const char *sql_vencedor =
        "UPDATE Competicoes SET vencedor_id = ("
        "  SELECT c.vendedor_id FROM Contratos c "
        "  JOIN Usuarios u ON u.id = c.vendedor_id "
        "  WHERE u.nivel = 'vendedor' AND c.status = 'aprovado' "
        "    AND c.data_contrato BETWEEN Competicoes.data_inicio AND Competicoes.data_fim "
        "  GROUP BY c.vendedor_id ORDER BY SUM(c.valor_credito) DESC LIMIT 1) "
        "WHERE tipo = 'geral' AND status = 'aberta' "
        "  AND date('now','localtime') >= data_fim;";

    /* Depois de definir o vencedor, arquiva a competicao geral que terminou */
    const char *sql_arquivar =
        "UPDATE Competicoes SET status = 'arquivado' "
        "WHERE tipo = 'geral' AND status = 'aberta' "
        "  AND date('now','localtime') >= data_fim "
        "RETURNING id, vencedor_id;";

    if (sqlite3_open("pim2.db", &db) != SQLITE_OK) {
        printf("|   Erro ao abrir o banco: %-32s|\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_exec(db, sql_vencedor, NULL, NULL, NULL);
    arquivar_competicoes_encerradas(db, sql_arquivar);

    /* So mostra o ranking se existir competicao geral aberta */
    if (!registro_existe('o', "tipo = 'geral' AND status = 'aberta'", NULL)) {
        printf("|   Nenhuma competicao geral em andamento.                 |\n");
        sqlite3_close(db);
        return;
    }

    printf("\n+----------------------------------------------------------+\n");
    printf("|   RANKING GERAL                                      |\n");
    printf("+----------------------------------------------------------+\n");

    posicao = mostrar_tabela(db, sql);
    if (posicao == 0) {
        printf("|   Nenhum contrato aprovado ainda.                        |\n");
    }

    sqlite3_close(db);
}

/* Ranking so dos vendedores da equipe informada.
   Vendedor e gestor chamam passando o proprio equipe_id;
   o gestor nao aparece porque o filtro pega apenas nivel = 'vendedor'. */
void ranking_equipe(int equipe_id) {
    sqlite3 *db;
    char sql[1024];
    char sql_vencedor[1024];
    char sql_arquivar[256];
    char filtro[128];
    int posicao = 0;

    if (equipe_id == 0) {
        printf("|   Voce nao esta vinculado a nenhuma equipe.              |\n");
        return;
    }

    /* Conta so os contratos feitos dentro do periodo da competicao aberta da equipe */
    snprintf(sql, sizeof(sql),
        "SELECT u.id, u.nome, COUNT(c.id) AS contratos, SUM(c.valor_credito) AS total "
        "FROM Usuarios u "
        "JOIN Contratos c ON u.id = c.vendedor_id "
        "JOIN Competicoes co ON co.equipe_id = %d AND co.status = 'aberta' "
        "WHERE u.nivel = 'vendedor' AND u.equipe_id = %d AND c.status = 'aprovado' "
        "  AND c.data_contrato BETWEEN co.data_inicio AND co.data_fim "
        "GROUP BY u.id "
        "ORDER BY total DESC;", equipe_id, equipe_id);

    /* Grava o vencedor das competicoes da equipe que ja terminaram */
    snprintf(sql_vencedor, sizeof(sql_vencedor),
        "UPDATE Competicoes SET vencedor_id = ("
        "  SELECT c.vendedor_id FROM Contratos c "
        "  JOIN Usuarios u ON u.id = c.vendedor_id "
        "  WHERE u.nivel = 'vendedor' AND u.equipe_id = %d AND c.status = 'aprovado' "
        "    AND c.data_contrato BETWEEN Competicoes.data_inicio AND Competicoes.data_fim "
        "  GROUP BY c.vendedor_id ORDER BY SUM(c.valor_credito) DESC LIMIT 1) "
        "WHERE equipe_id = %d AND status = 'aberta' "
        "  AND date('now','localtime') >= data_fim;", equipe_id, equipe_id);

    /* Depois de definir o vencedor, arquiva a competicao da equipe que terminou */
    snprintf(sql_arquivar, sizeof(sql_arquivar),
        "UPDATE Competicoes SET status = 'arquivado' "
        "WHERE equipe_id = %d AND status = 'aberta' "
        "  AND date('now','localtime') >= data_fim "
        "RETURNING id, vencedor_id;", equipe_id);

    if (sqlite3_open("pim2.db", &db) != SQLITE_OK) {
        printf("|   Erro ao abrir o banco: %-32s|\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_exec(db, sql_vencedor, NULL, NULL, NULL);
    arquivar_competicoes_encerradas(db, sql_arquivar);

    /* So mostra o ranking se a equipe tiver competicao aberta */
    snprintf(filtro, sizeof(filtro), "equipe_id = %d AND status = 'aberta'", equipe_id);
    if (!registro_existe('o', filtro, NULL)) {
        printf("|   Nenhuma competicao da equipe em andamento.             |\n");
        sqlite3_close(db);
        return;
    }

    printf("\n+----------------------------------------------------------+\n");
    printf("|   RANKING DA EQUIPE                                      |\n");
    printf("+----------------------------------------------------------+\n");

    posicao = mostrar_tabela(db, sql);
    if (posicao == 0) {
        printf("|   Nenhum contrato aprovado na equipe ainda.              |\n");
    }

    sqlite3_close(db);
}

/* ==========================================================
Ranking da equipe
========================================================== */

/* Mostra a comissao (5% dos contratos aprovados) de cada vendedor da equipe */
void comissao_equipe(int equipe_id) {
    sqlite3 *db;
    char sql[1024];
    int registros = 0;

    /* Conta so os contratos feitos dentro do periodo da competicao aberta da equipe */
    snprintf(sql, sizeof(sql),
        "SELECT u.id, u.nome, COUNT(c.id) AS contratos, "
        "SUM(c.valor_credito) AS total_vendido, "
        "SUM(c.valor_credito) * 0.05 AS comissao "
        "FROM Usuarios u "
        "JOIN Contratos c ON u.id = c.vendedor_id "
        "JOIN Competicoes co ON co.equipe_id = %d AND co.status = 'aberta' "
        "WHERE u.nivel = 'vendedor' AND u.equipe_id = %d AND c.status = 'aprovado' "
        "  AND c.data_contrato BETWEEN co.data_inicio AND co.data_fim "
        "GROUP BY u.id "
        "ORDER BY comissao DESC;", equipe_id, equipe_id);

    if (sqlite3_open("pim2.db", &db) != SQLITE_OK) {
        printf("|   Erro ao abrir o banco: %-32s|\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("\n+----------------------------------------------------------+\n");
    printf("|   COMISSOES DA EQUIPE (5%%)                               |\n");
    printf("+----------------------------------------------------------+\n");

    registros = mostrar_tabela(db, sql);
    if (registros == 0) {
        printf("|   Nenhum contrato aprovado na equipe ainda.              |\n");
    }

    sqlite3_close(db);
}




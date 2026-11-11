/* ==========================================================
   ranking.c
   Rankings e comissoes.
   ========================================================== */
#include "ranking.h"
#include "banco.h"
#include <stdio.h>


void ranking_geral() {
    sqlite3 *db;
    int posicao = 0;

    const char *sql =
        "SELECT u.id, u.nome, COUNT(c.id) AS contratos, SUM(c.valor_credito) AS total "
        "FROM Usuarios u "
        "JOIN Contratos c ON u.id = c.vendedor_id "
        "WHERE u.nivel = 'vendedor' AND c.status = 'aprovado' "
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
        "WHERE tipo = 'geral' AND vencedor_id IS NULL "
        "  AND date('now','localtime') >= data_fim;";

    if (sqlite3_open("pim2.db", &db) != SQLITE_OK) {
        printf("|   Erro ao abrir o banco: %-32s|\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_exec(db, sql_vencedor, NULL, NULL, NULL);

    printf("\n+----------------------------------------------------------+\n");
    printf("|                      RANKING GERAL                       |\n");
    printf("+----------------------------------------------------------+\n");

    posicao = mostrar_tabela(db, sql);
    if (posicao == 0) {
        printf("|             Nenhum contrato aprovado ainda.              |\n");
    }

    sqlite3_close(db);
}

/* Ranking so dos vendedores da equipe informada.
   Vendedor e gestor chamam passando o proprio equipe_id;
   o gestor nao aparece porque o filtro pega apenas nivel = 'vendedor'. */
void ranking_equipe(int equipe_id) {
    sqlite3 *db;
    char sql[512];
    char sql_vencedor[1024];
    int posicao = 0;

    if (equipe_id == 0) {
        printf("|        Voce nao esta vinculado a nenhuma equipe.         |\n");
        return;
    }

    snprintf(sql, sizeof(sql),
        "SELECT u.id, u.nome, COUNT(c.id) AS contratos, SUM(c.valor_credito) AS total "
        "FROM Usuarios u "
        "JOIN Contratos c ON u.id = c.vendedor_id "
        "WHERE u.nivel = 'vendedor' AND u.equipe_id = %d AND c.status = 'aprovado' "
        "GROUP BY u.id "
        "ORDER BY total DESC;", equipe_id);

    /* Grava o vencedor das competicoes da equipe que ja terminaram */
    snprintf(sql_vencedor, sizeof(sql_vencedor),
        "UPDATE Competicoes SET vencedor_id = ("
        "  SELECT c.vendedor_id FROM Contratos c "
        "  JOIN Usuarios u ON u.id = c.vendedor_id "
        "  WHERE u.nivel = 'vendedor' AND u.equipe_id = %d AND c.status = 'aprovado' "
        "    AND c.data_contrato BETWEEN Competicoes.data_inicio AND Competicoes.data_fim "
        "  GROUP BY c.vendedor_id ORDER BY SUM(c.valor_credito) DESC LIMIT 1) "
        "WHERE equipe_id = %d AND vencedor_id IS NULL "
        "  AND date('now','localtime') >= data_fim;", equipe_id, equipe_id);

    if (sqlite3_open("pim2.db", &db) != SQLITE_OK) {
        printf("|   Erro ao abrir o banco: %-32s|\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_exec(db, sql_vencedor, NULL, NULL, NULL);

    printf("\n+----------------------------------------------------------+\n");
    printf("|                    RANKING DA EQUIPE                     |\n");
    printf("+----------------------------------------------------------+\n");

    posicao = mostrar_tabela(db, sql);
    if (posicao == 0) {
        printf("|        Nenhum contrato aprovado na equipe ainda.         |\n");
    }

    sqlite3_close(db);
}

/* ==========================================================
Ranking da equipe
========================================================== */

/* Mostra a comissao (5% dos contratos aprovados) de cada vendedor da equipe */
void comissao_equipe(int equipe_id) {
    sqlite3 *db;
    char sql[512];
    int registros = 0;

    snprintf(sql, sizeof(sql),
        "SELECT u.id, u.nome, COUNT(c.id) AS contratos, "
        "SUM(c.valor_credito) AS total_vendido, "
        "SUM(c.valor_credito) * 0.05 AS comissao "
        "FROM Usuarios u "
        "JOIN Contratos c ON u.id = c.vendedor_id "
        "WHERE u.nivel = 'vendedor' AND u.equipe_id = %d AND c.status = 'aprovado' "
        "GROUP BY u.id "
        "ORDER BY comissao DESC;", equipe_id);

    if (sqlite3_open("pim2.db", &db) != SQLITE_OK) {
        printf("|   Erro ao abrir o banco: %-32s|\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("\n+----------------------------------------------------------+\n");
    printf("|                 COMISSOES DA EQUIPE (5%%)                 |\n");
    printf("+----------------------------------------------------------+\n");

    registros = mostrar_tabela(db, sql);
    if (registros == 0) {
        printf("|        Nenhum contrato aprovado na equipe ainda.         |\n");
    }

    sqlite3_close(db);
}




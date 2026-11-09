#include <stdio.h>
#include "sqlite3.h"
#include "ranking.h"
#include "funcoes.h"

void ranking_geral() {
    sqlite3 *db;
    char *erro = NULL;
    int posicao = 0;

    const char *sql =
        "SELECT u.id, u.nome, COUNT(c.id) AS contratos, SUM(c.valor_credito) AS total "
        "FROM Usuarios u "
        "JOIN Contratos c ON u.id = c.vendedor_id "
        "WHERE u.nivel = 'vendedor' AND c.status = 'aprovado' "
        "GROUP BY u.id "
        "ORDER BY total DESC;";

    if (sqlite3_open("pim2.db", &db) != SQLITE_OK) {
        printf("Erro ao abrir o banco: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("\n=== RANKING GERAL ===\n");

    if (sqlite3_exec(db, sql, mostrar_linha, &posicao, &erro) != SQLITE_OK) {
        printf("Erro ao consultar ranking: %s\n", erro);
        sqlite3_free(erro);
    } else if (posicao == 0) {
        printf("Nenhum contrato aprovado ainda.\n");
    }

    sqlite3_close(db);
}

/* Ranking so dos vendedores da equipe informada.
   Vendedor e gestor chamam passando o proprio equipe_id;
   o gestor nao aparece porque o filtro pega apenas nivel = 'vendedor'. */
void ranking_equipe(int equipe_id) {
    sqlite3 *db;
    char *erro = NULL;
    char sql[512];
    int posicao = 0;

    if (equipe_id == 0) {
        printf("Voce nao esta vinculado a nenhuma equipe.\n");
        return;
    }

    snprintf(sql, sizeof(sql),
        "SELECT u.id, u.nome, COUNT(c.id) AS contratos, SUM(c.valor_credito) AS total "
        "FROM Usuarios u "
        "JOIN Contratos c ON u.id = c.vendedor_id "
        "WHERE u.nivel = 'vendedor' AND u.equipe_id = %d AND c.status = 'aprovado' "
        "GROUP BY u.id "
        "ORDER BY total DESC;", equipe_id);

    if (sqlite3_open("pim2.db", &db) != SQLITE_OK) {
        printf("Erro ao abrir o banco: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("\n=== RANKING DA EQUIPE ===\n");

    if (sqlite3_exec(db, sql, mostrar_linha, &posicao, &erro) != SQLITE_OK) {
        printf("Erro ao consultar ranking: %s\n", erro);
        sqlite3_free(erro);
    } else if (posicao == 0) {
        printf("Nenhum contrato aprovado na equipe ainda.\n");
    }

    sqlite3_close(db);
}

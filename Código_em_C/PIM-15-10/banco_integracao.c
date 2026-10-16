/* ==========================================================
   banco_integracao.c
   Codigo das funcoes declaradas em banco_integracao.h.
   ========================================================== */
#include <stdio.h>
#include "banco_integracao.h"
#include "banco_tabelas.h"

/* ---------- CONEXAO ---------- */

int banco_abrir(sqlite3 **db) {
    int resultado = sqlite3_open("pim2.db", db);

    if (resultado != SQLITE_OK) {
        printf("Erro ao abrir o banco: %s\n", sqlite3_errmsg(*db));
        return resultado;
    }

    /* Liga as chaves estrangeiras (o SQLite vem com elas desligadas) */
    sqlite3_exec(*db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);
    return SQLITE_OK;
}

void banco_fechar(sqlite3 *db) {
    sqlite3_close(db);
}

int banco_iniciar(sqlite3 **db) {
    int resultado = banco_abrir(db);
    if (resultado != SQLITE_OK) return resultado;

    return banco_criar_tabelas(*db);
}

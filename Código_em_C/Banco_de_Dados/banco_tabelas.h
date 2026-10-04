/* ==========================================================
   banco_tabelas.h
   Criacao das tabelas do banco pim2.db.
   ========================================================== */
#ifndef BANCO_TABELAS_H
#define BANCO_TABELAS_H

#include "sqlite3.h"

/* ---------- CRIACAO DAS TABELAS ---------- */

/* Cria as tabelas que ainda nao existem. Retorna SQLITE_OK se deu certo. */
int banco_criar_tabelas(sqlite3 *db);

#endif

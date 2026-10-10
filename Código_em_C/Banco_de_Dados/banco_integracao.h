/* ==========================================================
   banco_integracao.h
   Integracao entre o programa em C e o banco SQLite (pim2.db).
   Quem usa o banco inclui este arquivo e chama as funcoes
   abaixo, sem precisar escrever SQL.
   ========================================================== */
#ifndef BANCO_INTEGRACAO_H
#define BANCO_INTEGRACAO_H

#include "sqlite3.h"

/* ---------- CONEXAO ---------- */

/* Abre o banco (cria o pim2.db se nao existir). Retorna SQLITE_OK se deu certo. */
int banco_abrir(sqlite3 **db);

/* Fecha o banco. Chamar antes de encerrar o programa. */
void banco_fechar(sqlite3 *db);

/* Abre o banco e cria as tabelas que faltarem. Chamar uma vez, no inicio do programa. */
int banco_iniciar(sqlite3 **db);

#endif

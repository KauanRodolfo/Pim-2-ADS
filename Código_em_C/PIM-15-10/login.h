/* ==========================================================
   login.h
   Tela inicial do sistema: menu de login.
   ========================================================== */
#ifndef LOGIN_H
#define LOGIN_H

#include "sqlite3.h"
#include "funcoes_db.h"

/* Mostra o menu inicial (Login / Sair) e fica nele ate a pessoa sair. */
// void tela_login(sqlite3 *db); nao existe mais, foi unificado

Usuario fazer_login(sqlite3 *db);

#endif

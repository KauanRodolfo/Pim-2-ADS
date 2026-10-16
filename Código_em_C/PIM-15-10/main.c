/* ==========================================================
   main.c
   Comeco do programa: abre o banco, garante que o admin existe
   e chama a tela de login.
   ========================================================== */
#include <stdio.h>
#include "sqlite3.h"
#include "banco_integracao.h"
#include "funcoes.h"
#include "login.h"

int main(void) {
    sqlite3 *db;

    /* Abre o pim2.db (e cria as tabelas que faltarem) */
    if (banco_iniciar(&db) != SQLITE_OK) {
        printf("Nao foi possivel abrir o banco de dados.\n");
        return 1;
    }

    /* Se nao existir o usuario admin, cria. Se ja existir, segue em frente. */
    garantir_admin(db);

    /* Tela de login (fica aqui ate a pessoa escolher Sair) */
    tela_login(db);

    banco_fechar(db);
    return 0;
}

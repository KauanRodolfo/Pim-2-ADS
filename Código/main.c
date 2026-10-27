/* ==========================================================
   main.c
   Comeco do programa: abre o banco, garante que o admin existe
   e chama a tela de logiin.
   ========================================================== */
#include "admin.h"
#include "funcoes.h"
#include "funcoes_db.h"
#include "login.h"
#include "sqlite3.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    int rc;
    Usuario u;
    sqlite3 *db;

    // Constroi o banco de dados

    if (criar_db(&db) != SQLITE_OK) {
        printf("Nao foi possivel criar o banco de dados.\n");
        // printf("%d",criar_db(&db));
        // return 1;
    }

    // Tela de login (fica aqui ate a pessoa escolher Sair)
    u = fazer_login(db);

    if (strcmp(u.nivel, "admin") == 0) {
        menu_admin();
    } else if (strcmp(u.nivel, "gestor") == 0) {
        menu_gestor();
    } else if (strcmp(u.nivel, "vendedor") == 0) {
        menu_vendedor();
    }

    // fecha o banco
    sqlite3_close(db);
    return 0;
}

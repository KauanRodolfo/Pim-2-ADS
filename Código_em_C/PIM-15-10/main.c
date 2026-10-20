/* ==========================================================
   main.c
   Comeco do programa: abre o banco, garante que o admin existe
   e chama a tela de login.
   ========================================================== */
#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "funcoes_db.h"
#include "funcoes.h"
#include "admin.h"
#include "login.h"

int main(void) {
    int rc;
    Usuario u;
    sqlite3 *db;

    // Constroi o banco de dados

    if (criar_db(&db) != SQLITE_OK) {
        printf("Nao foi possivel criar o banco de dados.\n");
        //printf("%d",criar_db(&db));
        //return 1;
    }

    // Tela de login (fica aqui ate a pessoa escolher Sair)
    u = fazer_login(db);


    if(strcmp(u.nivel ,"admin") == 0){
        menu_admin();
    }



    //fecha o banco
    sqlite3_close(db);
    return 0;
}

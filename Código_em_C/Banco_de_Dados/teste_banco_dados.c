#include <stdio.h>
#include "banco_integracao.h"

int main() {
    sqlite3 *db;

    /* Abre o banco e cria as tabelas, tudo de uma vez */
    if (banco_iniciar(&db) != SQLITE_OK) return 1;
    printf("Banco iniciado com sucesso!\n");

    banco_fechar(db);
    return 0;
}

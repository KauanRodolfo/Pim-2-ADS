/* ==========================================================
   funcoes.c
   Codigo das funcoes declaradas em funcoes.h.
   ========================================================== */
#include <stdio.h>
#include <string.h>
#include "funcoes.h"

/* ---------- ENTRADA DE DADOS ---------- */

void ler_texto(const char *msg, char *dest, int tam) {
    int i, j = 0, c;

    printf("%s", msg);
    if (fgets(dest, tam, stdin) == NULL) {
        dest[0] = '\0';
        return;
    }

    /* Tira o Enter do final. Se nao tinha Enter, o texto era grande
       demais: descarta o que sobrou no teclado. */
    i = strlen(dest);
    if (i > 0 && dest[i - 1] == '\n') {
        dest[i - 1] = '\0';
    } else {
        while ((c = getchar()) != '\n' && c != EOF);
    }

    /* Tira as aspas simples ('), porque elas quebrariam o comando SQL */
    for (i = 0; dest[i] != '\0'; i++) {
        if (dest[i] != '\'') {
            dest[j] = dest[i];
            j++;
        }
    }
    dest[j] = '\0';
}

/* ---------- BANCO DE DADOS ---------- */

int existe_registro(sqlite3 *db, const char *sql) {
    char **resultado = NULL;   /* tabela com o resultado do SELECT */
    int linhas = 0, colunas = 0;

    sqlite3_get_table(db, sql, &resultado, &linhas, &colunas, NULL);
    sqlite3_free_table(resultado);

    return linhas > 0;
}

void garantir_admin(sqlite3 *db) {
    char *erro = NULL;

    /* Ja existe um admin? Entao nao faz nada. */
    if (existe_registro(db, "SELECT id FROM Usuario WHERE username = 'admin';")) {
        return;
    }

    /* Nao existe: cria com a senha padrao admin123 */
    int resultado = sqlite3_exec(db,
        "INSERT INTO Usuario (nome, username, email, senha, nivel) "
        "VALUES ('Administrador', 'admin', 'admin@pim.com', 'admin123', 'admin');",
        NULL, NULL, &erro);

    if (resultado == SQLITE_OK) {
        printf("Usuario admin criado (senha: admin123).\n");
    } else {
        printf("Erro ao criar o admin: %s\n", erro);
        sqlite3_free(erro);
    }
}

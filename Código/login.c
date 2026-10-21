/* ==========================================================
   login.c
   Tela de login. Depois de entrar, chama o menu do nivel da pessoa.
   ========================================================== */
#include "login.h"
#include "admin.h"
#include "funcoes.h"
#include "funcoes_db.h"
#include <stdio.h>
#include <string.h>

Usuario fazer_login(sqlite3 *db) {
    char usuario[TAM], senha[TAM], sql[400];
    char nome[TAM], nivel[TAM_P];
    char **resultado = NULL;
    int linhas = 0, colunas = 0, op;
    sqlite3_stmt *stmt = NULL;
    Usuario u;

    printf("\n=== SISTEMA DE CONSORCIO ===\n");
    printf("1 - Login\n");
    printf("0 - Sair\n");
    scanf("%d", &op);

    switch (op) {
    case 1:

        printf("\n=== LOGIN ===\n");
        printf("CPF (so numeros) ou admin - 0 para voltar: ");

        do {
            scanf("%s", &usuario);

            if (strcmp(usuario, "0") == 0) {
                break;
            }

            tratar_texto(usuario);
            sprintf(sql,
                    "SELECT nome, senha, nivel FROM Usuarios "
                    "WHERE username = '%s' AND status = 'ativo';",
                    usuario);

            sqlite3_get_table(db, sql, &resultado, &linhas, &colunas, NULL);

            if (linhas == 0) {
                printf("Usuario nao encontrado.\nTente novamente:");
                sqlite3_free_table(resultado);
                // return 0;
            }

        } while (linhas == 0);

        /*A função acima: sqlite3_get_table(db, sql, &resultado, &linhas, &colunas, NULL);
        Retorna em resultado o vetor[6] {nome, senha, nivel, valor campo nome, valor campo senha, valor campo nivel}
        usamos na sequencia a função strcpy para copiar para a nossa struct Usuario os dados do banco para fazer a
        verificação da senha*/

        if (linhas) {
            strcpy(u.nome, resultado[3]);
            strcpy(u.senha, resultado[4]);
            strcpy(u.nivel, resultado[5]);

            do {
                printf("\nSenha: ");
                scanf("%s", &senha);

                tratar_texto(senha);

                if (strcmp(u.senha, senha) != 0) {
                    printf("Senha incorreta, tente novamente\n");
                }
            } while (strcmp(u.senha, senha) != 0);

            printf("Login bem sucedido!\n");
        }

        sqlite3_free_table(resultado);
        break;

    case 0:
        printf("Ate logo!\n");
        break;
    default:
        printf("Opcao invalida.\n");
    }

    return u;
}

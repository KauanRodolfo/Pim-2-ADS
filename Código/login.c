/* ==========================================================
   login.c
   Tela de login. Depois de entrar, chama o menu do nivel da pessoa.
   ========================================================== */
#include "login.h"
#include "admin.h"
#include "util.h"
#include "banco.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Usuario fazer_login(sqlite3 *db) {
    char usuario[TAM], senha[TAM], sql[400];
    char **resultado = NULL;
    char confirmar;
    int linhas, colunas, op, voltar_menu, usuario_encontrado;
    Usuario u = {0};

    do {
        printf("\n+----------------------------------------------------------+\n");
        printf("|   SISTEMA DE CONSORCIO                                   |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   1 - Login                                              |\n");
        printf("|   0 - Sair                                               |\n");
        printf("+----------------------------------------------------------+\n");
        printf("|   Escolha uma opcao: ");

        if (scanf("%d", &op) != 1) {
            op = -1;
        }
        limpar_buffer();

        switch (op) {
        case 1:
            printf("\n+----------------------------------------------------------+\n");
            printf("|   LOGIN                                                  |\n");
            printf("+----------------------------------------------------------+\n");
            printf("|   Use 0 para voltar                                      |\n");

            voltar_menu = 0;
            usuario_encontrado = 0;

            do {
                printf("|   Nome de usuario: ");

                if (scanf("%49s", usuario) != 1) {
                    limpar_buffer();
                    return u;
                }
                tratar_texto(usuario);

                if (strcmp(usuario, "0") == 0) {
                    voltar_menu = 1;
                    break;
                }

                snprintf(sql, sizeof(sql),
                    "SELECT nome, senha, nivel, id, equipe_id FROM Usuarios "
                    "WHERE username = '%s' AND status = 'ativo';",
                    usuario);

                resultado = NULL;
                linhas = 0;
                colunas = 0;

                if (sqlite3_get_table(db, sql, &resultado, &linhas,
                                      &colunas, NULL) != SQLITE_OK) {
                    printf("|   Erro ao consultar o usuario.                          |\n");
                    sqlite3_free_table(resultado);
                    resultado = NULL;
                    break;
                }

                if (linhas > 0) {
                    usuario_encontrado = 1;
                } else {
                    printf("|   Usuario nao encontrado. Tente novamente.              |\n");
                    sqlite3_free_table(resultado);
                    resultado = NULL;
                }
            } while (!usuario_encontrado);

            if (voltar_menu || !usuario_encontrado) {
                break;
            }

            strcpy(u.nome, resultado[5]);
            strcpy(u.senha, resultado[6]);
            strcpy(u.nivel, resultado[7]);
            u.id = atoi(resultado[8]);
            u.equipe_id = resultado[9] != NULL ? atoi(resultado[9]) : 0;

            sqlite3_free_table(resultado);

            do {
                printf("|   Senha: ");

                if (scanf("%49s", senha) != 1) {
                    limpar_buffer();
                    return (Usuario){0};
                }
                tratar_texto(senha);

                if (strcmp(u.senha, senha) != 0) {
                    printf("|   Senha incorreta, tente novamente.                    |\n");
                }
            } while (strcmp(u.senha, senha) != 0);

            printf("|   Login bem sucedido!                                   |\n");
            return u;

        case 0:
            printf("|   Deseja realmente sair? (s/n): ");

            if (scanf(" %c", &confirmar) != 1) {
                confirmar = 'n';
            }
            limpar_buffer();

            if (confirmar == 's' || confirmar == 'S') {
                printf("|   Ate logo!                                              |\n");
                return u;
            }

            printf("|   Saida cancelada.                                       |\n");
            break;

        default:
            printf("|   Opcao invalida. Digite 1 ou 0.                         |\n");
            break;
        }
    } while (1);
}

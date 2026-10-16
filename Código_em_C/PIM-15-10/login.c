/* ==========================================================
   login.c
   Tela de login. Depois de entrar, chama o menu do nivel da pessoa.
   ========================================================== */
#include <stdio.h>
#include <string.h>
#include "login.h"
#include "funcoes.h"
#include "admin.h"

/* Pede CPF (ou admin) e senha e confere no banco.
   Se estiver certo, guarda o nome e o nivel da pessoa e retorna 1.
   Se estiver errado (ou ela escolher voltar), retorna 0. */
int fazer_login(sqlite3 *db, char *nome, char *nivel) {
    char usuario[TAM], senha[TAM], sql[400];
    char **resultado = NULL;
    int linhas = 0, colunas = 0;

    printf("\n=== LOGIN ===\n");
    ler_texto("CPF (so numeros) ou admin - 0 para voltar: ", usuario, TAM);
    if (strcmp(usuario, "0") == 0) return 0;
    ler_texto("Senha: ", senha, TAM);

    sprintf(sql,
        "SELECT nome, nivel FROM Usuario "
        "WHERE username = '%s' AND senha = '%s' AND status = 'ativo';",
        usuario, senha);

    sqlite3_get_table(db, sql, &resultado, &linhas, &colunas, NULL);

    if (linhas == 0) {
        printf("Usuario ou senha incorretos (ou usuario nao cadastrado).\n");
        sqlite3_free_table(resultado);
        return 0;
    }

    /* A tabela do resultado vem assim:
       resultado[0] e resultado[1] = titulos das colunas (nome, nivel)
       resultado[2] e resultado[3] = valores da primeira linha */
    strcpy(nome, resultado[2]);
    strcpy(nivel, resultado[3]);

    sqlite3_free_table(resultado);
    return 1;
}

/* Mostra o menu de acordo com o nivel de quem entrou */
void menu_principal(sqlite3 *db, const char *nome, const char *nivel) {
    printf("\n=== Bem-vindo, %s! Nivel: %s ===\n", nome, nivel);

    if (strcmp(nivel, "admin") == 0) {
        menu_admin(db);
    } else if (strcmp(nivel, "gestor") == 0) {
        printf("(Menu do GESTOR: a fazer)\n");
    } else {
        printf("(Menu do VENDEDOR: a fazer)\n");
    }
}

void tela_login(sqlite3 *db) {
    char opcao[TAM], nome[TAM], nivel[TAM];

    do {
        printf("\n=== SISTEMA DE CONSORCIO ===\n");
        printf("1 - Login\n");
        printf("0 - Sair\n");
        ler_texto("Escolha: ", opcao, TAM);

        switch (opcao[0]) {
            case '1':
                if (fazer_login(db, nome, nivel)) {
                    menu_principal(db, nome, nivel);
                }
                break;
            case '0':
                printf("Ate logo!\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao[0] != '0');
}

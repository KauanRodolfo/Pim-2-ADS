/* ==========================================================
   login.c
   Tela de login. Depois de entrar, chama o menu do nivel da pessoa.
   ========================================================== */
#include <stdio.h>
#include <string.h>
#include "login.h"
#include "admin.h"
#include "funcoes.h"
#include "funcoes_db.h"

// FOI PRA MAIN, FOI PRA MAIN, FOI PRA MAIN, FOI PRA MAIN

/* Mostra o menu de acordo com o nivel de quem entrou */ //
                                                         // void menu_principal(sqlite3 *db, const char *nome, const char *nivel) {
                                                         //     printf("\n=== Bem-vindo, %s! Nivel: %s ===\n", nome, nivel);

//     if (strcmp(nivel, "admin") == 0) {
//         menu_admin(db);
//     } else if (strcmp(nivel, "gestor") == 0) {
//         printf("(Menu do GESTOR: a fazer)\n");
//     } else {
//         printf("(Menu do VENDEDOR: a fazer)\n");
//     }
// }

/* Pede CPF (ou admin) e senha e confere no banco.
Se estiver certo, guarda o nome e o nivel da pessoa e retorna 1.
Se estiver errado (ou ela escolher voltar), retorna 0. */
// int fazer_login(sqlite3 *db, char *nome, char *nivel) {
//     char usuario[TAM], senha[TAM], sql[400];
//     char **resultado = NULL;
//     int linhas = 0, colunas = 0;

//     printf("\n=== LOGIN ===\n");
//     ler_texto("CPF (so numeros) ou admin - 0 para voltar: ", usuario, TAM);
//     if (strcmp(usuario, "0") == 0) return 0;
//     ler_texto("Senha: ", senha, TAM);

//     sprintf(sql,
//         "SELECT nome, nivel FROM Usuario "
//         "WHERE username = '%s' AND senha = '%s' AND status = 'ativo';",
//         usuario, senha);

//     sqlite3_get_table(db, sql, &resultado, &linhas, &colunas, NULL);

//     if (linhas == 0) {
//         printf("Usuario ou senha incorretos (ou usuario nao cadastrado).\n");
//         sqlite3_free_table(resultado);
//         return 0;
//     }

//     /* A tabela do resultado vem assim:
//        resultado[0] e resultado[1] = titulos das colunas (nome, nivel)
//        resultado[2] e resultado[3] = valores da primeira linha */
//     strcpy(nome, resultado[2]);
//     strcpy(nivel, resultado[3]);

//     sqlite3_free_table(resultado);
//     return 1;
// }

// void tela_login(sqlite3 *db) {
//     char opcao[TAM], nome[TAM], nivel[TAM];

//     do {
//         printf("\n=== SISTEMA DE CONSORCIO ===\n");
//         printf("1 - Login\n");
//         printf("0 - Sair\n");
//         ler_texto("Escolha: ", opcao, TAM);

//         switch (opcao[0]) {
//             case '1':
//                 if (fazer_login(db, nome, nivel)) {
//                     menu_principal(db, nome, nivel);
//                 }
//                 break;
//             case '0':
//                 printf("Ate logo!\n");
//                 break;
//             default:
//                 printf("Opcao invalida.\n");
//         }
//     } while (opcao[0] != '0');
// }

Usuario fazer_login(sqlite3 *db)
{
    char usuario[TAM], senha[TAM], sql[400];
    char nome[TAM], nivel[TAM];
    char **resultado = NULL;
    int linhas = 0, colunas = 0, op;
    sqlite3_stmt *stmt = NULL;
    Usuario u;


    //do{

    printf("\n=== SISTEMA DE CONSORCIO ===\n");
    printf("1 - Login\n");
    printf("0 - Sair\n");
    scanf("%d", &op);
    // ler_texto("Escolha: ", opcao, TAM);
    //} while (op != 0);

    switch (op)
    {
    case 1:
        char usuario[TAM], senha[TAM];
        printf("\n=== LOGIN ===\n");
        printf("CPF (so numeros) ou admin - 0 para voltar: ");


        do{
            scanf("%s", &usuario);

            //if (strcmp(usuario, "0") == 0){
            //    return u = NULL;
            //}

            tratar_texto(usuario);
            sprintf(sql,
                    "SELECT nome, senha, nivel FROM Usuarios "
                    "WHERE username = '%s' AND status = 'ativo';",
                    usuario);

            sqlite3_get_table(db, sql, &resultado, &linhas, &colunas, NULL);

            if (linhas == 0)
            {
                printf("Usuario não encontrado.\nTente novamente:");
                sqlite3_free_table(resultado);
                //return 0;
            }

        }while(linhas == 0);

        //0NOME 1SENHA 2NIVEL
        //3valor_nome 4valorsenha
        strcpy(u.nome, resultado[3]);
        strcpy(u.senha, resultado[4]);
        strcpy(u.nivel, resultado[5]);

        printf("\n%s",u.senha);

        printf("\nSenha: ");
        scanf("%s", &senha);

        tratar_texto(senha);

        if(strcmp(u.senha, senha) != 0){
            printf("Senha incorreta, tente novamente\n");
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

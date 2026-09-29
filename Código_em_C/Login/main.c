#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "sqlite3.h"

#define TAM 50

static sqlite3 *db;

/* ---------- Utilidades ---------- */

/* Le uma linha do teclado com seguranca (sem estourar o vetor) */
void ler_texto(const char *msg, char *dest, int tam) {
    printf("%s", msg);
    if (fgets(dest, tam, stdin) == NULL) {
        dest[0] = '\0';
        return;
    }
    size_t n = strlen(dest);
    if (n > 0 && dest[n - 1] == '\n') {
        dest[n - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF); /* descarta o excesso */
    }
}

/* Hash simples (djb2) para nao guardar a senha em texto puro.
   Serve para o trabalho; em sistema real use bcrypt/argon2. */
void gerar_hash(const char *texto, char *saida) {
    unsigned long long h = 5381;
    for (const char *p = texto; *p; p++) {
        h = h * 33 + (unsigned char)*p;
    }
    sprintf(saida, "%llu", h);
}

/* Tira pontos, tracos e letras: fica so com os digitos */
void limpar_cpf(const char *origem, char *destino) {
    int j = 0;
    for (int i = 0; origem[i] != '\0' && j < 30; i++) {
        if (isdigit((unsigned char)origem[i])) destino[j++] = origem[i];
    }
    destino[j] = '\0';
}

/* ---------- Banco de dados ---------- */

int abrir_banco(void) {
    if (sqlite3_open("consorcio.db", &db) != SQLITE_OK) {
        printf("Erro ao abrir o banco: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    const char *sql =
        "CREATE TABLE IF NOT EXISTS usuarios ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " nome TEXT NOT NULL,"
        " sobrenome TEXT NOT NULL,"
        " usuario TEXT UNIQUE NOT NULL,"   /* CPF (ou "admin") */
        " senha_hash TEXT NOT NULL,"
        " nivel TEXT NOT NULL CHECK(nivel IN ('vendedor','gestor','admin'))"
        ");";
    char *erro = NULL;
    if (sqlite3_exec(db, sql, NULL, NULL, &erro) != SQLITE_OK) {
        printf("Erro ao criar tabela: %s\n", erro);
        sqlite3_free(erro);
        return 0;
    }
    return 1;
}

int usuario_existe(const char *usuario) {
    sqlite3_stmt *st;
    sqlite3_prepare_v2(db, "SELECT 1 FROM usuarios WHERE usuario = ?;", -1, &st, NULL);
    sqlite3_bind_text(st, 1, usuario, -1, SQLITE_TRANSIENT);
    int existe = (sqlite3_step(st) == SQLITE_ROW);
    sqlite3_finalize(st);
    return existe;
}

int contar_usuarios(void) {
    sqlite3_stmt *st;
    int total = 0;
    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM usuarios;", -1, &st, NULL);
    if (sqlite3_step(st) == SQLITE_ROW) total = sqlite3_column_int(st, 0);
    sqlite3_finalize(st);
    return total;
}

/* ---------- Cadastro (so o admin acessa) ---------- */

void cadastrar(void) {
    char nome[TAM], sobrenome[TAM], cpf_digitado[TAM], cpf[32], cargo[TAM];
    char h_senha[32];

    printf("\n=== CADASTRAR USUARIO ===\n");
    ler_texto("Nome (0 para voltar): ", nome, TAM);
    if (strcmp(nome, "0") == 0) return;
    ler_texto("Sobrenome: ", sobrenome, TAM);
    ler_texto("CPF: ", cpf_digitado, TAM);

    if (nome[0] == '\0' || sobrenome[0] == '\0') {
        printf("Nome e sobrenome nao podem ser vazios.\n");
        return;
    }

    limpar_cpf(cpf_digitado, cpf);
    if (strlen(cpf) != 11) {
        printf("CPF invalido: precisa ter 11 digitos.\n");
        return;
    }
    if (usuario_existe(cpf)) {
        printf("Esse CPF ja esta cadastrado.\n");
        return;
    }

    ler_texto("Cargo (1 = vendedor, 2 = gestor): ", cargo, TAM);
    const char *nivel;
    if (cargo[0] == '1') nivel = "vendedor";
    else if (cargo[0] == '2') nivel = "gestor";
    else {
        printf("Cargo invalido.\n");
        return;
    }

    gerar_hash(&cpf[6], h_senha);   /* &cpf[6] = ultimos 5 digitos do CPF */

    sqlite3_stmt *st;
    sqlite3_prepare_v2(db,
        "INSERT INTO usuarios (nome, sobrenome, usuario, senha_hash, nivel) VALUES (?,?,?,?,?);",
        -1, &st, NULL);
    sqlite3_bind_text(st, 1, nome, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(st, 2, sobrenome, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(st, 3, cpf, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(st, 4, h_senha, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(st, 5, nivel, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(st) == SQLITE_DONE) {
        printf("Usuario cadastrado como %s!\n", nivel);
        printf("Login: %s | Senha inicial: %s\n", cpf, &cpf[6]);
    } else {
        printf("Erro ao cadastrar: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(st);
}

/* ---------- Login ---------- */

/* Retorna 1 se o login deu certo e preenche usuario_logado e nivel */
int fazer_login(char *usuario_logado, char *nivel, char *nome_logado) {
    char usuario[TAM], senha[TAM], h_senha[32];

    printf("\n=== LOGIN ===\n");
    ler_texto("CPF (0 para voltar): ", usuario, TAM);
    if (strcmp(usuario, "0") == 0) return 0;

    if (isdigit((unsigned char)usuario[0])) {   /* comecou com numero: e um CPF */
        char limpo[32];
        limpar_cpf(usuario, limpo);
        strcpy(usuario, limpo);
    }

    ler_texto("Senha: ", senha, TAM);
    gerar_hash(senha, h_senha);

    sqlite3_stmt *st;
    sqlite3_prepare_v2(db,
        "SELECT nivel, nome, sobrenome FROM usuarios WHERE usuario = ? AND senha_hash = ?;",
        -1, &st, NULL);
    sqlite3_bind_text(st, 1, usuario, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(st, 2, h_senha, -1, SQLITE_TRANSIENT);

    int ok = 0;
    if (sqlite3_step(st) == SQLITE_ROW) {
        strcpy(usuario_logado, usuario);
        strcpy(nivel, (const char *)sqlite3_column_text(st, 0));
        snprintf(nome_logado, TAM, "%s %s",
                 (const char *)sqlite3_column_text(st,1),
                 (const char *)sqlite3_column_text(st, 2));
        ok = 1;
    } else {
        printf("Usuario ou senha incorretos (ou usuario nao cadastrado).\n");
    }
    sqlite3_finalize(st);
    return ok;
}

/* ---------- Editar perfis (admin) ---------- */

/* Mostra os funcionarios numerados (1, 2, 3...). Retorna quantos existem. */
int listar_funcionarios(void) {
    sqlite3_stmt *st;
    int n = 0;
    sqlite3_prepare_v2(db,
        "SELECT nome, sobrenome, usuario, nivel FROM usuarios "
        "WHERE nivel != 'admin' ORDER BY id;", -1, &st, NULL);

    printf("\n%-4s %-15s %-15s %-13s %s\n", "N", "Nome", "Sobrenome", "CPF", "Cargo");
    while (sqlite3_step(st) == SQLITE_ROW) {
        n++;
        printf("%-4d %-15s %-15s %-13s %s\n", n,
            (const char *)sqlite3_column_text(st, 0),
            (const char *)sqlite3_column_text(st, 1),
            (const char *)sqlite3_column_text(st, 2),
            (const char *)sqlite3_column_text(st, 3));
    }
    sqlite3_finalize(st);

    if (n == 0) printf("Nenhum funcionario cadastrado.\n");
    return n;
}

/* Mostra a lista, pergunta o numero e devolve o id do banco (0 = voltar) */
int escolher_funcionario(void) {
    char entrada[TAM];
    int total = listar_funcionarios();
    if (total == 0) return 0;

    ler_texto("\nNumero do funcionario (0 para voltar): ", entrada, TAM);
    int escolha = atoi(entrada);
    if (escolha == 0) return 0;
    if (escolha < 0 || escolha > total) {
        printf("Numero invalido.\n");
        return 0;
    }

    sqlite3_stmt *st;
    int id = 0;
    sqlite3_prepare_v2(db,
        "SELECT id FROM usuarios WHERE nivel != 'admin' ORDER BY id LIMIT 1 OFFSET ?;",
        -1, &st, NULL);
    sqlite3_bind_int(st, 1, escolha - 1);
    if (sqlite3_step(st) == SQLITE_ROW) id = sqlite3_column_int(st, 0);
    sqlite3_finalize(st);
    return id;
}

void apagar_usuario(void) {
    int id = escolher_funcionario();
    if (id == 0) return;

    char confirma[TAM];
    ler_texto("Tem certeza que deseja apagar? (s/n): ", confirma, TAM);
    if (confirma[0] != 's' && confirma[0] != 'S') {
        printf("Cancelado.\n");
        return;
    }

    sqlite3_stmt *st;
    sqlite3_prepare_v2(db, "DELETE FROM usuarios WHERE id = ?;", -1, &st, NULL);
    sqlite3_bind_int(st, 1, id);
    sqlite3_step(st);
    sqlite3_finalize(st);
    printf("Usuario apagado.\n");
}

void editar_nome(void) {
    int id = escolher_funcionario();
    if (id == 0) return;

    char nome[TAM], sobrenome[TAM];
    ler_texto("Novo nome: ", nome, TAM);
    ler_texto("Novo sobrenome: ", sobrenome, TAM);
    if (nome[0] == '\0' || sobrenome[0] == '\0') {
        printf("Nome e sobrenome nao podem ser vazios.\n");
        return;
    }

    sqlite3_stmt *st;
    sqlite3_prepare_v2(db,
        "UPDATE usuarios SET nome = ?, sobrenome = ? WHERE id = ?;", -1, &st, NULL);
    sqlite3_bind_text(st, 1, nome, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(st, 2, sobrenome, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(st, 3, id);
    sqlite3_step(st);
    sqlite3_finalize(st);
    printf("Nome alterado.\n");
}

void alterar_cargo(void) {
    int id = escolher_funcionario();
    if (id == 0) return;

    char cargo[TAM];
    ler_texto("Novo cargo (1 = vendedor, 2 = gestor): ", cargo, TAM);
    const char *nivel;
    if (cargo[0] == '1') nivel = "vendedor";
    else if (cargo[0] == '2') nivel = "gestor";
    else {
        printf("Cargo invalido.\n");
        return;
    }

    sqlite3_stmt *st;
    sqlite3_prepare_v2(db, "UPDATE usuarios SET nivel = ? WHERE id = ?;", -1, &st, NULL);
    sqlite3_bind_text(st, 1, nivel, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(st, 2, id);
    sqlite3_step(st);
    sqlite3_finalize(st);
    printf("Cargo alterado para %s.\n", nivel);
}

void menu_editar_perfis(void) {
    char opcao[TAM];
    do {
        printf("\n=== EDITAR PERFIS ===\n");
        printf("1 - Deletar usuario\n");
        printf("2 - Editar nome\n");
        printf("3 - Alterar cargo\n");
        printf("0 - Voltar\n");
        ler_texto("Escolha: ", opcao, TAM);

        switch (opcao[0]) {
            case '1': apagar_usuario(); break;
            case '2': editar_nome(); break;
            case '3': alterar_cargo(); break;
            case '0': break;
            default:  printf("Opcao invalida.\n");
        }
    } while (opcao[0] != '0');
}

void menu_admin(void) {
    char opcao[TAM];
    do {
        printf("\n=== MENU ADMIN ===\n");
        printf("1 - Cadastrar usuario\n");
        printf("2 - Editar perfis\n");
        printf("0 - Sair da conta\n");
        ler_texto("Escolha: ", opcao, TAM);

        switch (opcao[0]) {
            case '1': cadastrar(); break;
            case '2': menu_editar_perfis(); break;
            case '0': break;
            default:  printf("Opcao invalida.\n");
        }
    } while (opcao[0] != '0');
}

/* Aqui entram as telas do sistema de consorcio, por nivel de acesso */
void menu_principal(const char *nome, const char *nivel) {
    printf("\n=== Bem-vindo, %s! Nivel: %s ===\n", nome, nivel);

    if (strcmp(nivel, "admin") == 0) {
        menu_admin();
    } else if (strcmp(nivel, "gestor") == 0) {
        printf("(Menu do GESTOR: a fazer)\n");
    } else {
        printf("(Menu do VENDEDOR: a fazer)\n");
    }
}

/* ---------- Programa principal ---------- */

int main(void) {
    if (!abrir_banco()) return 1;

    char opcao[TAM];
    char usuario_logado[TAM], nivel[TAM], nome_logado[TAM];

    do {
        printf("\n=== SISTEMA DE CONSORCIO ===\n");
        printf("1 - Login\n");
        printf("0 - Sair\n");
        ler_texto("Escolha: ", opcao, TAM);

        switch (opcao[0]) {
            case '1':
                if (fazer_login(usuario_logado, nivel, nome_logado))
                    menu_principal(usuario_logado, nivel);
                break;
            case '0': printf("Ate logo!\n"); break;
            default:  printf("Opcao invalida.\n");
        }
    } while (opcao[0] != '0');

    sqlite3_close(db);
    return 0;
}

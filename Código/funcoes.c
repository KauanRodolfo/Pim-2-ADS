/* ==========================================================
   funcoes.c
   Codigo das funcoes declaradas em funcoes.h.
   ========================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcoes_db.h"

/* ---------- ENTRADA DE DADOS ---------- */
void tratar_texto(char *variavel) {
    int i, j = 0, c;

    /* Tira o Enter do final. Se nao tinha Enter, o texto era grande
       demais: descarta o que sobrou no teclado. */
    i = strlen(variavel);
    if (i > 0 && variavel[i - 1] == '\n') {
        variavel[i - 1] = '\0';
    } else {
        while ((c = getchar()) != '\n' && c != EOF);
    }

    /* Tira as aspas simples ('), porque elas quebrariam o comando SQL */
    for (i = 0; variavel[i] != '\0'; i++) {
        if (variavel[i] != '\'') {
            variavel[j] = variavel[i];
            j++;
        }
    }
    variavel[j] = '\0';

}

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // descarta os caracteres residuais no buffer
    }
}

/* Chamada pelo sqlite3_exec para cada linha do resultado da consulta.
   "registros" conta quantas linhas foram mostradas na pagina. */
int mostrar_linha(void *dado, int colunas, char **valores, char **nomes) {
    int *registros = (int *)dado;
    int coluna;
    char *valor;

    (*registros)++;
    for (coluna = 0; coluna < colunas; coluna++) {
        if (valores[coluna] != NULL) {
            valor = valores[coluna];
        } else {
            valor = "NULL";
        }

        printf("%s: %s", nomes[coluna], valor);

        if (coluna + 1 == colunas) {
            printf("\n");
        } else {
            printf(" | ");
        }
    }
    return 0;
}

/* Chamada pelo sqlite3_exec para ler o resultado de um SELECT COUNT(*). */
int ler_total(void *dado, int colunas, char **valores, char **nomes) {
    int *total = (int *)dado;

    *total = atoi(valores[0]);
    return 0;
}

/* filtro: condicao do WHERE (ex: "nivel = 'gestor'") ou NULL para trazer tudo */
int consultar(char banco, const char *filtro) {
    sqlite3 *db;
    char *erro = NULL;
    char sql[1024];
    const char *tabela;
    char opcao = ' ';
    int rc;
    int offset = 0;
    int total = 0;

    switch (banco) {
        case 'E':
        case 'e':
            tabela = "Equipes";
            break;
        case 'U':
        case 'u':
            tabela = "Usuarios";
            break;
        case 'C':
        case 'c':
            tabela = "Clientes";
            break;
        case 'T':
        case 't':
            tabela = "Contratos";
            break;
        case 'V':
        case 'v':
            tabela = "Eventos";
            break;
        case 'O':
        case 'o':
            tabela = "Competicoes";
            break;
        case 'L':
        case 'l':
            tabela = "LogsAuditoria";
            break;
        default:
            fprintf(stderr, "Banco invalido para consulta.\n");
            return 1;
    }

    rc = sqlite3_open("pim2.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Nao foi possivel abrir o banco: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    /* Monta a contagem com ou sem WHERE, conforme o filtro estar vazio ou nao */
    if (filtro != NULL && filtro[0] != '\0') {
        snprintf(sql, sizeof(sql), "SELECT COUNT(*) FROM %s WHERE %s;", tabela, filtro);
    } else {
        snprintf(sql, sizeof(sql), "SELECT COUNT(*) FROM %s;", tabela);
    }
    /* Executa a contagem para saber se a paginacao e necessaria (total > 10) */
    rc = sqlite3_exec(db, sql, ler_total, &total, &erro);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Nao foi possivel contar os registros: %s\n", erro);
        sqlite3_free(erro);
        sqlite3_close(db);
        return 1;
    }

    do {
        int registros = 0;

        if (filtro != NULL && filtro[0] != '\0') {
            snprintf(sql, sizeof(sql),
                     "SELECT * FROM %s WHERE %s ORDER BY id LIMIT 10 OFFSET %d;",
                     tabela, filtro, offset);
        } else {
            snprintf(sql, sizeof(sql),
                     "SELECT * FROM %s ORDER BY id LIMIT 10 OFFSET %d;",
                     tabela, offset);
        }

        printf("\n--- %s (offset %d) ---\n", tabela, offset);

        rc = sqlite3_exec(db, sql, mostrar_linha, &registros, &erro);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Nao foi possivel consultar os registros: %s\n", erro);
            sqlite3_free(erro);
            sqlite3_close(db);
            return 1;
        }

        if (registros == 0 && offset == 0) {
            printf("Nenhum registro encontrado.\n");
            break;
        }

        if (registros == 0) {
            printf("Nao ha mais registros nesta pagina.\n");
            offset -= 10;
            continue;
        }

        /* So pagina quando ha mais de 10 registros */
        if (total <= 10) {
            break;
        }

        printf("\n[p] proxima pagina | [a] pagina anterior | [q] sair: ");
        if (scanf(" %c", &opcao) != 1) {
            fprintf(stderr, "Nao foi possivel ler a opcao.\n");
            sqlite3_close(db);
            return 1;
        }

        switch (opcao) {
            case 'p':
            case 'P':
                offset += 10;
                break;
            case 'a':
            case 'A':
                if (offset >= 10) {
                    offset -= 10;
                }
                break;
            case 'q':
            case 'Q':
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 'q' && opcao != 'Q');

    sqlite3_close(db);
    return 0;
}

int excluir(char banco, int id) {
    sqlite3 *db;
    char *erro = NULL;
    char sql[256];
    int rc = SQLITE_OK;

    if (id <= 0) {
        fprintf(stderr, "Id invalido para exclusao.\n");
        return 1;
    }

    rc = sqlite3_open("pim2.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Nao foi possivel abrir o banco: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    switch (banco) {
        case 'E':
        case 'e':
            snprintf(sql, sizeof(sql),
                     "UPDATE Equipes SET status = 'arquivado' WHERE id = %d;", id);
            break;

        case 'U':
        case 'u':
            snprintf(sql, sizeof(sql),
                     "UPDATE Usuarios SET status = 'arquivado' WHERE id = %d;", id);
            break;

        case 'C':
        case 'c':
            snprintf(sql, sizeof(sql),
                     "UPDATE Clientes SET status = 'arquivado' WHERE id = %d;", id);
            break;

        case 'T':
        case 't':
            snprintf(sql, sizeof(sql),
                     "UPDATE Contratos SET status = 'arquivado' WHERE id = %d;", id);
            break;

        case 'O':
        case 'o':
            snprintf(sql, sizeof(sql),
                     "UPDATE Competicoes SET status = 'arquivado' WHERE id = %d;", id);
            break;

        default:
            sqlite3_close(db);
            return 1;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &erro);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Nao foi possivel arquivar o registro: %s\n", erro);
        sqlite3_free(erro);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}

int salvar(char banco, Equipe *equipe, Usuario *usuario, Cliente *cliente,
            Contrato *contrato, Competicao *competicao){
    sqlite3 *db;
    char *erro = NULL;
    char sql[2048];
    int rc = SQLITE_OK;

    rc = sqlite3_open("pim2.db", &db); //retorna 0 se a conexo for bem Sucedida

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Nao foi possivel abrir o banco: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    switch (banco) {
        case 'E':
        case 'e':
            if (equipe == NULL) {
                sqlite3_close(db);
                return 1;
            }
            tratar_texto(equipe->nome);

            if (equipe->gestor_id == 0) {
                snprintf(sql, sizeof(sql),
                         "INSERT INTO Equipes (nome, gestor_id, status) "
                         "VALUES ('%s', NULL, '%s');",
                         equipe->nome, equipe->status);
            } else {
                snprintf(sql, sizeof(sql),
                         "INSERT INTO Equipes (nome, gestor_id, status) "
                         "VALUES ('%s', %d, '%s');",
                         equipe->nome, equipe->gestor_id, equipe->status);
            }
            break;

        case 'U':
        case 'u':
            if (usuario == NULL) {
                sqlite3_close(db);
                return 1;
            }
            tratar_texto(usuario->nome);
            tratar_texto(usuario->cpf);
            tratar_texto(usuario->username);
            tratar_texto(usuario->email);
            

            if (usuario->equipe_id == 0) {
                snprintf(sql, sizeof(sql),
                         "INSERT INTO Usuarios "
                         "(nome, cpf, username, email, senha, nivel, equipe_id, status) "
                         "VALUES ('%s', '%s', '%s', '%s', '%s', '%s', NULL, '%s');",
                         usuario->nome, usuario->cpf, usuario->username,
                         usuario->email, usuario->senha, usuario->nivel,
                         usuario->status);
            } else {
                snprintf(sql, sizeof(sql),
                         "INSERT INTO Usuarios "
                         "(nome, cpf, username, email, senha, nivel, equipe_id, status) "
                         "VALUES ('%s', '%s', '%s', '%s', '%s', '%s', %d, '%s');",
                         usuario->nome, usuario->cpf, usuario->username,
                         usuario->email, usuario->senha, usuario->nivel,
                         usuario->equipe_id, usuario->status);
            }
            break;

        case 'C':
        case 'c':
            if (cliente == NULL) {
                sqlite3_close(db);
                return 1;
            }
            tratar_texto(cliente->nome);
            tratar_texto(cliente->cpf);
            tratar_texto(cliente->email);
            tratar_texto(cliente->telefone);
            snprintf(sql, sizeof(sql),
                     "INSERT INTO Clientes "
                     "(nome, cpf, email, telefone, renda, status, cadastrado_por) "
                     "VALUES ('%s', '%s', '%s', '%s', %f, '%s', %d);",
                     cliente->nome, cliente->cpf, cliente->email,
                     cliente->telefone, cliente->renda[0], cliente->status,
                     cliente->cadastrado_por);
            break;

        case 'T':
        case 't':
            if (contrato == NULL) {
                sqlite3_close(db);
                return 1;
            }
            tratar_texto(contrato->data_contrato);
            tratar_texto(contrato->status);
            tratar_texto(contrato->data_avaliacao);
            tratar_texto(contrato->motivo_reprovacao);

            snprintf(sql, sizeof(sql),
                     "INSERT INTO Contratos "
                     "(cliente_id, vendedor_id, tipo_consorcio_id, valor_credito, "
                     "data_contrato, status, avaliado_por, data_avaliacao, motivo_reprovacao) "
                     "VALUES (%d, %d, %d, %f, '%s', '%s', %s, '%s', '%s');",
                     contrato->cliente_id, contrato->usuario_id,
                     contrato->tipo_consorcio_id, contrato->valor_credito,
                     contrato->data_contrato, contrato->status,
                     contrato->avaliado_por == 0 ? "NULL" : "0",
                     contrato->data_avaliacao, contrato->motivo_reprovacao);

            if (contrato->avaliado_por != 0) {
                snprintf(sql, sizeof(sql),
                         "INSERT INTO Contratos "
                         "(cliente_id, vendedor_id, tipo_consorcio_id, valor_credito, "
                         "data_contrato, status, avaliado_por, data_avaliacao, motivo_reprovacao) "
                         "VALUES (%d, %d, %d, %f, '%s', '%s', %d, '%s', '%s');",
                         contrato->cliente_id, contrato->usuario_id,
                         contrato->tipo_consorcio_id, contrato->valor_credito,
                         contrato->data_contrato, contrato->status,
                         contrato->avaliado_por, contrato->data_avaliacao,
                         contrato->motivo_reprovacao);
            }
            break;

        case 'O':
        case 'o':
            if (competicao == NULL) {
                sqlite3_close(db);
                return 1;
            }
            tratar_texto(competicao->nome);
            tratar_texto(competicao->data_inicio);
            tratar_texto(competicao->data_fim);

            if (competicao->equipe_id == 0) {
                snprintf(sql, sizeof(sql),
                         "INSERT INTO Competicoes "
                         "(nome, tipo, equipe_id, data_inicio, data_fim, criada_por) "
                         "VALUES ('%s', '%s', NULL, '%s', '%s', %d);",
                         competicao->nome, competicao->tipo,
                         competicao->data_inicio, competicao->data_fim,
                         competicao->criada_por);
            } else {
                snprintf(sql, sizeof(sql),
                         "INSERT INTO Competicoes "
                         "(nome, tipo, equipe_id, data_inicio, data_fim, criada_por) "
                         "VALUES ('%s', '%s', %d, '%s', '%s', %d);",
                         competicao->nome, competicao->tipo, competicao->equipe_id,
                         competicao->data_inicio, competicao->data_fim,
                         competicao->criada_por);
            }
            break;

        default:
            sqlite3_close(db);
            return 1;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &erro);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Nao foi possivel salvar o registro: %s\n", erro);
        sqlite3_free(erro);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}

int editar(char banco, int id) {
    sqlite3 *db;
    char *erro = NULL;
    char sql[2048];
    char valor[TAM_TEXTO + 2];
    const char *tabela = NULL;
    const char *coluna = NULL;
    int opcao;
    int rc = SQLITE_OK;

    if (id <= 0) {
        fprintf(stderr, "Id invalido para edicao.\n");
        return 1;
    }

    printf("qual campo gostaria de editar?\n");

    switch (banco) {
        case 'E':
        case 'e':
            tabela = "Equipes";
            printf("1 - Nome\n2 - Gestor (id)\n");
            scanf("%d", &opcao);
            switch (opcao) {
                case 1: coluna = "nome"; break;
                case 2: coluna = "gestor_id"; break;
            }
            break;

        case 'U':
        case 'u':
            tabela = "Usuarios";
            printf("1 - Nome\n2 - CPF\n3 - Username\n4 - Email\n5 - Senha\n6 - Nivel\n7 - Equipe (id)\n");
            scanf("%d", &opcao);
            switch (opcao) {
                case 1: coluna = "nome"; break;
                case 2: coluna = "cpf"; break;
                case 3: coluna = "username"; break;
                case 4: coluna = "email"; break;
                case 5: coluna = "senha"; break;
                case 6: coluna = "nivel"; break;
                case 7: coluna = "equipe_id"; break;
            }
            break;

        case 'C':
        case 'c':
            tabela = "Clientes";
            printf("1 - Nome\n2 - CPF\n3 - Email\n4 - Telefone\n5 - Renda\n");
            scanf("%d", &opcao);
            switch (opcao) {
                case 1: coluna = "nome"; break;
                case 2: coluna = "cpf"; break;
                case 3: coluna = "email"; break;
                case 4: coluna = "telefone"; break;
                case 5: coluna = "renda"; break;
            }
            break;

        case 'T':
        case 't':
            tabela = "Contratos";
            printf("1 - Cliente (id)\n2 - Vendedor (id)\n3 - Valor do credito\n4 - Data do contrato\n5 - Status\n6 - Motivo da reprovacao\n");
            scanf("%d", &opcao);
            switch (opcao) {
                case 1: coluna = "cliente_id"; break;
                case 2: coluna = "vendedor_id"; break;
                case 3: coluna = "valor_credito"; break;
                case 4: coluna = "data_contrato"; break;
                case 5: coluna = "status"; break;
                case 6: coluna = "motivo_reprovacao"; break;
            }
            break;

        case 'O':
        case 'o':
            tabela = "Competicoes";
            printf("1 - Nome\n2 - Data de inicio\n3 - Data de fim\n");
            scanf("%d", &opcao);
            switch (opcao) {
                case 1: coluna = "nome"; break;
                case 2: coluna = "data_inicio"; break;
                case 3: coluna = "data_fim"; break;
            }
            break;

        default:
            fprintf(stderr, "Banco invalido para edicao.\n");
            return 1;
    }

    limpar_buffer();

    if (coluna == NULL) {
        printf("Opcao invalida.\n");
        return 1;
    }

    printf("Novo valor: ");
    fgets(valor, sizeof(valor), stdin);
    tratar_texto(valor);

    snprintf(sql, sizeof(sql),
             "UPDATE %s SET %s = '%s' WHERE id = %d;",
             tabela, coluna, valor, id);

    rc = sqlite3_open("pim2.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Nao foi possivel abrir o banco: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &erro);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Nao foi possivel editar o registro: %s\n", erro);
        sqlite3_free(erro);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}
int registro_existe(char ini_tabela, const char *filtro, void *saida) {
    sqlite3 *db;
    char *erro = NULL;
    char **resultado = NULL;
    int linhas = 0, colunas = 0;
    const char *tabela;
    char sql[256];
    int rc;

    switch (ini_tabela) {
        case 'E':
        case 'e':
            tabela = "Equipes";
            break;
        case 'U':
        case 'u':
            tabela = "Usuarios";
            break;
        case 'C':
        case 'c':
            tabela = "Clientes";
            break;
        case 'T':
        case 't':
            tabela = "Contratos";
            break;
        case 'V':
        case 'v':
            tabela = "Eventos";
            break;
        case 'O':
        case 'o':
            tabela = "Competicoes";
            break;
        case 'L':
        case 'l':
            tabela = "LogsAuditoria";
            break;
        default:
            fprintf(stderr, "Banco invalido para consulta.\n");
            return 1;
    }

    rc = sqlite3_open("pim2.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Nao foi possivel abrir o banco: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    if (filtro != NULL && filtro[0] != '\0') {
        if (ini_tabela == 't' || ini_tabela == 'T') {
            /* Contratos nao tem coluna nome */
            snprintf(sql, sizeof(sql),
                     "SELECT id, cliente_id, vendedor_id, valor_credito, data_contrato, status "
                     "FROM %s WHERE %s;", tabela, filtro);
        } else {
            snprintf(sql, sizeof(sql), "SELECT ID, NOME FROM %s WHERE %s;", tabela, filtro);
        }

        // Executa a consulta para verificar se o registro existe
        // A função sqlite3_get_table retorna o resultado da consulta em um array de strings
        // no formato [nome_coluna1, nome_coluna2, valor_coluna1, valor_coluna2]
        sqlite3_get_table(db, sql, &resultado, &linhas, &colunas, NULL);

        if (linhas == 0) {
            sqlite3_free_table(resultado);
            sqlite3_close(db);
            return 0; // Retorna 0 se o usuário não existe
        } 

        if (saida != NULL) {
            switch (ini_tabela) {
                case 'U':
                case 'u':
                    Usuario *u = (Usuario *)saida;
                    u->id = atoi(resultado[2]);
                    strcpy(u->nome, resultado[3]);
                    break; 
                case 'E':
                case 'e':
                    Equipe *e = (Equipe *)saida;
                    e->id = atoi(resultado[2]);
                    strcpy(e->nome, resultado[3]);
                    break;
                case 'T':
                case 't':
                    /* 6 colunas: os valores comecam em resultado[6] */
                    Contrato *t = (Contrato *)saida;
                    t->id = atoi(resultado[6]);
                    t->cliente_id = atoi(resultado[7]);
                    t->usuario_id = atoi(resultado[8]);
                    t->valor_credito = atof(resultado[9]);
                    strcpy(t->data_contrato, resultado[10]);
                    strcpy(t->status, resultado[11]);
                    break;
            }
        }
        sqlite3_free_table(resultado);
        sqlite3_close(db);
        return 1; // Retorna 1 se o registro existe
    }else{
        snprintf(sql, sizeof(sql), "SELECT ID, NOME FROM %s;", tabela);
        rc = sqlite3_exec(db, sql, NULL, NULL, &erro);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "Erro ao verificar existencia do registro: %s\n", erro);
            sqlite3_free(erro);
            sqlite3_close(db);
            return 0;
        }

        sqlite3_close(db);
        return 1; // Retorna 1 se o usuário existe, 0 caso contrário
    }
    
}

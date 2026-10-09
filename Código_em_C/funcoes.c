#include <stdio.h>

#include "structs.h"
#include "sqlite3.h"

int consultar(char banco) {
    sqlite3 *db;
    sqlite3_stmt *stmt = NULL;
    const char *tabela;
    const char *sql;
    char opcao = ' ';
    int rc;
    int offset = 0;

    switch (banco) {
        case 'E':
        case 'e':
            tabela = "Equipe";
            sql = "SELECT * FROM Equipe ORDER BY id LIMIT 10 OFFSET ?;";
            break;
        case 'U':
        case 'u':
            tabela = "Usuario";
            sql = "SELECT * FROM Usuario ORDER BY id LIMIT 10 OFFSET ?;";
            break;
        case 'C':
        case 'c':
            tabela = "Cliente";
            sql = "SELECT * FROM Cliente ORDER BY id LIMIT 10 OFFSET ?;";
            break;
        case 'T':
        case 't':
            tabela = "Contrato";
            sql = "SELECT * FROM Contrato ORDER BY id LIMIT 10 OFFSET ?;";
            break;
        case 'V':
        case 'v':
            tabela = "Evento";
            sql = "SELECT * FROM Evento ORDER BY id LIMIT 10 OFFSET ?;";
            break;
        default:
            fprintf(stderr, "Banco invalido para consulta.\n");
            return 1;
    }

    rc = sqlite3_open("testeaula", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Nao foi possivel abrir o banco: %s\n",
                sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    do {
        int registros = 0;
        int colunas;
        
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Nao foi possivel preparar a consulta: %s\n",
                    sqlite3_errmsg(db));
            sqlite3_close(db);
            return 1;
        }

        rc = sqlite3_bind_int(stmt, 1, offset);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Nao foi possivel definir o offset: %s\n",
                    sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 1;
        }

        colunas = sqlite3_column_count(stmt);
        printf("\n--- %s (offset %d) ---\n", tabela, offset);

        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            int coluna;

            registros++;
            for (coluna = 0; coluna < colunas; coluna++) {
                const char *valor =
                    (const char *)sqlite3_column_text(stmt, coluna);

                printf("%s: %s%s",
                       sqlite3_column_name(stmt, coluna),
                       valor != NULL ? valor : "NULL",
                       coluna + 1 == colunas ? "\n" : " | ");
            }
        }

        sqlite3_finalize(stmt);
        stmt = NULL;

        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Nao foi possivel ler os registros: %s\n",
                    sqlite3_errmsg(db));
            sqlite3_close(db);
            return 1;
        }

        if (registros == 0 && offset > 0) {
            printf("Nao ha mais registros nesta pagina.\n");
            offset -= 10;
        } else {
            printf("\n[p] proxima pagina | [a] pagina anterior | [q] sair: ");
            if (scanf(" %c", &opcao) != 1) {
                fprintf(stderr, "Nao foi possivel ler a opcao.\n");
                sqlite3_close(db);
                return 1;
            }

            if (opcao == 'p' || opcao == 'P') {
                offset += 10;
            } else if ((opcao == 'a' || opcao == 'A') && offset >= 10) {
                offset -= 10;
            } else if (opcao != 'q' && opcao != 'Q' &&
                       opcao != 'a' && opcao != 'A') {
                printf("Opcao invalida.\n");
            }
        }
    } while (opcao != 'q' && opcao != 'Q');

    sqlite3_close(db);
    return 0;
}

int excluir(int id, char banco) {
    sqlite3 *db;
    sqlite3_stmt *stmt = NULL;
    const char *sql;
    int rc;

    if (id <= 0) {
        fprintf(stderr, "Id invalido para exclusao.\n");
        return 1;
    }

    switch (banco) {
        case 'E':
        case 'e':
            sql = "DELETE FROM Equipe WHERE id = ?;";
            break;
        case 'U':
        case 'u':
            sql = "DELETE FROM Usuario WHERE id = ?;";
            break;
        case 'C':
        case 'c':
            sql = "DELETE FROM Cliente WHERE id = ?;";
            break;
        case 'T':
        case 't':
            sql = "DELETE FROM Contrato WHERE id = ?;";
            break;
        case 'V':
        case 'v':
            sql = "DELETE FROM Evento WHERE id = ?;";
            break;
        default:
            fprintf(stderr, "Banco invalido para exclusao.\n");
            return 1;
    }

    rc = sqlite3_open("testeaula", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Nao foi possivel abrir o banco: %s\n",
                sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Nao foi possivel preparar a exclusao: %s\n",
                sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_bind_int(stmt, 1, id);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Nao foi possivel associar o id: %s\n",
                sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Nao foi possivel excluir o registro: %s\n",
                sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

int salvar(char banco, Equipe *equipe, Usuario *usuario, Cliente *cliente,
            Contrato *contrato, Evento *evento){
    sqlite3 *db;
    sqlite3_stmt *stmt = NULL;

    int rc;

    rc = sqlite3_open("testeaula", &db); //retorna 0 se a conexo for bem Sucedida

    if(rc){

        fprintf(stderr, "Nao foi possivel abrir o banco: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    const char *sql;

    switch(banco){
        case 'E':
        case 'e':
            if(equipe == NULL){
                sqlite3_close(db);
                return 1;
            }
            sql = "INSERT INTO Equipe (nome, gestor_id, status, criado_em) VALUES (?, ?, ?, ?);";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            if(rc == SQLITE_OK){
                rc = sqlite3_bind_text(stmt, 1, equipe->nome, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_int(stmt, 2, equipe->gestor_id);
                rc |= sqlite3_bind_text(stmt, 3, equipe->status == STATUS_ATIVO ? "ativa" : "arquivada", -1, SQLITE_STATIC);
                rc |= sqlite3_bind_text(stmt, 4, equipe->criado_em, -1, SQLITE_TRANSIENT);
            }
            break;
        case 'U':
        case 'u':
            if(usuario == NULL){
                sqlite3_close(db);
                return 1;
            }
            sql = "INSERT INTO Usuario (nome, username, email, senha, nivel, equipe_id, status, criado_em) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            if(rc == SQLITE_OK){
                rc = sqlite3_bind_text(stmt, 1, usuario->nome, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_text(stmt, 2, usuario->username, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_text(stmt, 3, usuario->email, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_text(stmt, 4, usuario->senha, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_int(stmt, 5, usuario->nivel);
                rc |= sqlite3_bind_int(stmt, 6, usuario->equipe_id);
                rc |= sqlite3_bind_text(stmt, 7, usuario->status == STATUS_ATIVO ? "ativa" : "arquivada", -1, SQLITE_STATIC);
                rc |= sqlite3_bind_text(stmt, 8, usuario->criado_em, -1, SQLITE_TRANSIENT);
            }
            break;
        case 'C':
        case 'c':
            if(cliente == NULL){
                sqlite3_close(db);
                return 1;
            }
            sql = "INSERT INTO Cliente (nome, cpf, email, telefone, renda, consentimento_lgpd, data_consentimento, status, cadastrado_por, criado_em) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            if(rc == SQLITE_OK){
                rc = sqlite3_bind_text(stmt, 1, cliente->nome, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_text(stmt, 2, cliente->cpf, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_text(stmt, 3, cliente->email, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_text(stmt, 4, cliente->telefone, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_double(stmt, 5, cliente->renda);
                rc |= sqlite3_bind_int(stmt, 6, cliente->consentimento_lgpd);
                rc |= sqlite3_bind_text(stmt, 7, cliente->data_consentimento, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_text(stmt, 8, cliente->status == STATUS_ATIVO ? "ativa" : "arquivada", -1, SQLITE_STATIC);
                rc |= sqlite3_bind_int(stmt, 9, cliente->cadastrado_por);
                rc |= sqlite3_bind_text(stmt, 10, cliente->criado_em, -1, SQLITE_TRANSIENT);
            }
            break;
        case 'T':
        case 't':
            if(contrato == NULL){
                sqlite3_close(db);
                return 1;
            }
            sql = "INSERT INTO Contrato (cliente_id, vendedor_id, tipo_consorcio_id, valor_credito, data_contrato, status, avaliado_por, data_avaliacao, motivo_reprovacao, criado_em) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            if(rc == SQLITE_OK){
                rc = sqlite3_bind_int(stmt, 1, contrato->cliente_id);
                rc |= sqlite3_bind_int(stmt, 2, contrato->vendedor_id);
                rc |= sqlite3_bind_int(stmt, 3, contrato->tipo_consorcio_id);
                rc |= sqlite3_bind_double(stmt, 4, contrato->valor_credito);
                rc |= sqlite3_bind_text(stmt, 5, contrato->data_contrato, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_text(stmt, 6,
                    contrato->status == CONTRATO_EM_ANALISE ? "em_analise" :
                    contrato->status == CONTRATO_APROVADO ? "aprovado" :
                    contrato->status == CONTRATO_REPROVADO ? "reprovado" : "arquivado",
                    -1, SQLITE_STATIC);
                rc |= sqlite3_bind_int(stmt, 7, contrato->avaliado_por);
                rc |= sqlite3_bind_text(stmt, 8, contrato->data_avaliacao, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_text(stmt, 9, contrato->motivo_reprovacao, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_text(stmt, 10, contrato->criado_em, -1, SQLITE_TRANSIENT);
            }
            break;
        case 'V':
        case 'v':
            if(evento == NULL){
                sqlite3_close(db);
                return 1;
            }
            sql = "INSERT INTO Evento (competicao_id, titulo, descricao, premio, data_evento) VALUES (?, ?, ?, ?, ?);";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            if(rc == SQLITE_OK){
                rc = sqlite3_bind_int(stmt, 1, evento->competicao_id);
                rc |= sqlite3_bind_text(stmt, 2, evento->titulo, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_text(stmt, 3, evento->descricao, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_text(stmt, 4, evento->premio, -1, SQLITE_TRANSIENT);
                rc |= sqlite3_bind_text(stmt, 5, evento->data_evento, -1, SQLITE_TRANSIENT);
            }
            break;
        default:
            sqlite3_close(db);
            return 1;
    }

    if(rc != SQLITE_OK){
        fprintf(stderr, "Nao foi possivel associar os valores: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    if(rc != SQLITE_OK || sqlite3_step(stmt) != SQLITE_DONE){
        fprintf(stderr, "Nao foi possivel salvar o contrato: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;

}
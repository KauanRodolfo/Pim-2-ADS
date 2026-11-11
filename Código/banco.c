/* ==========================================================
   banco.c
   Criacao do banco e funcoes genericas de acesso as tabelas.
   ========================================================== */
#include "banco.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//     /* Liga as chaves estrangeiras (o SQLite vem com elas desligadas) */
//     //sqlite3_exec(*db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);

/* ---------- CRIACAO DO BANCO E TABELAS ---------- */

int criar_db(sqlite3 **db) {
    char *erro = NULL; /* guarda a mensagem de erro, se der problema */

    int rc = sqlite3_open("pim2.db", db);

    if (rc != SQLITE_OK) {
        printf("|   Erro ao abrir o banco: %-32s|\n", sqlite3_errmsg(*db));
        return rc;
    }

    const char *sql =
        /* Usuario: equipe_id aponta para a tabela Equipe */
        "CREATE TABLE IF NOT EXISTS Usuarios ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  nome TEXT NOT NULL,"
        "  cpf TEXT NOT NULL," //unique
        "  username TEXT NOT NULL UNIQUE,"
        "  email TEXT NOT NULL UNIQUE,"
        "  senha TEXT NOT NULL,"
        "  nivel TEXT NOT NULL CHECK (nivel IN ('vendedor','gestor','admin')),"
        "  equipe_id INTEGER REFERENCES Equipe(id),"
        "  status TEXT NOT NULL DEFAULT 'ativo' CHECK (status IN ('ativo','arquivado')),"
        "  criado_em TEXT NOT NULL DEFAULT (datetime('now','localtime'))"
        ");"

        /* Equipe: gestor_id aponta para um Usuario */
        "CREATE TABLE IF NOT EXISTS Equipes ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  nome TEXT NOT NULL,"
        "  gestor_id INTEGER REFERENCES Usuarios(id),"
        "  status TEXT NOT NULL DEFAULT 'ativa' CHECK (status IN ('ativa','arquivado')),"
        "  criado_em TEXT NOT NULL DEFAULT (datetime('now','localtime'))"
        ");"


        /* Cliente: quem compra o consorcio (dados pessoais, LGPD) */
        "CREATE TABLE IF NOT EXISTS Clientes ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  nome TEXT NOT NULL,"
        "  cpf TEXT NOT NULL UNIQUE CHECK (length(cpf) = 11),"
        "  email TEXT,"
        "  telefone TEXT,"
        "  renda REAL,"
        "  status TEXT NOT NULL DEFAULT 'ativo' CHECK (status IN ('ativo','arquivado')),"
        "  cadastrado_por INTEGER NOT NULL REFERENCES Usuarios(id),"
        "  criado_em TEXT NOT NULL DEFAULT (datetime('now','localtime'))"
        ");"

        /* Contrato: a venda; o gestor aprova ou reprova */
        "CREATE TABLE IF NOT EXISTS Contratos ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  cliente_id INTEGER NOT NULL REFERENCES Cliente(id),"
        "  vendedor_id INTEGER NOT NULL REFERENCES Usuarios(id),"
        "  tipo_consorcio_id INTEGER NOT NULL REFERENCES TipoConsorcio(id),"
        "  valor_credito REAL NOT NULL CHECK (valor_credito > 0),"
        "  data_contrato TEXT NOT NULL DEFAULT (date('now','localtime')),"
        "  status TEXT NOT NULL DEFAULT 'em_analise'"
        "    CHECK (status IN ('em_analise','aprovado','reprovado','arquivado')),"
        "  avaliado_por INTEGER REFERENCES Usuarios(id),"
        "  data_avaliacao TEXT,"
        "  motivo_reprovacao TEXT,"
        "  criado_em TEXT NOT NULL DEFAULT (datetime('now','localtime'))"
        ");"

        /* Competicao: geral (sem equipe) ou interna (com equipe) */
        "CREATE TABLE IF NOT EXISTS Competicoes ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  nome TEXT NOT NULL,"
        "  tipo TEXT NOT NULL CHECK (tipo IN ('geral','interna')),"
        "  equipe_id INTEGER REFERENCES Equipe(id),"
        "  data_inicio TEXT NOT NULL,"
        "  data_fim TEXT NOT NULL,"
        "  status TEXT NOT NULL DEFAULT 'aberta' CHECK (status IN ('aberta','arquivado')),"
        "  criada_por INTEGER NOT NULL REFERENCES Usuarios(id),"
        "  vencedor_id INTEGER REFERENCES Usuarios(id),"
        "  CHECK ((tipo = 'geral' AND equipe_id IS NULL) OR"
        "         (tipo = 'interna' AND equipe_id IS NOT NULL))"
        ");"


        /* LogAuditoria: registra exclusoes e arquivamentos (LGPD) */
        "CREATE TABLE IF NOT EXISTS LogsAuditoria ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  usuario_id INTEGER NOT NULL REFERENCES Usuarios(id),"
        "  acao TEXT NOT NULL CHECK (acao IN ('exclusao','arquivamento')),"
        "  entidade TEXT NOT NULL,"
        "  entidade_id INTEGER NOT NULL,"
        "  detalhes TEXT,"
        "  data_hora TEXT NOT NULL DEFAULT (datetime('now','localtime'))"
        ");"

        /*REGISTRA O ADMIN*/
        "INSERT INTO Usuarios (id, nome, cpf, username, email, senha, nivel) "
        "VALUES ('1','Administrador', '12312312311', 'admin', 'admin@pim.com', 'admin123', 'admin') "
        "ON CONFLICT (id) DO NOTHING;"

        /* GESTORES (um para cada equipe) */
        "INSERT INTO Usuarios (id, nome, cpf, username, email, senha, nivel, equipe_id) VALUES "
        "('2', 'Carlos Souza', '10000000001', 'gestor1', 'gestor1@pim.com', 'gestor123', 'gestor', '1'),"
        "('3', 'Fernanda Lima', '10000000002', 'gestor2', 'gestor2@pim.com', 'gestor123', 'gestor', '2'),"
        "('4', 'Ricardo Alves', '10000000003', 'gestor3', 'gestor3@pim.com', 'gestor123', 'gestor', '3') "
        "ON CONFLICT (id) DO NOTHING;"

        /* EQUIPES */
        "INSERT INTO Equipes (id, nome, gestor_id) VALUES "
        "('1', 'Equipe Alfa', '2'),"
        "('2', 'Equipe Beta', '3'),"
        "('3', 'Equipe Gama', '4') "
        "ON CONFLICT (id) DO NOTHING;"

        /* VENDEDORES (7 por equipe) */
        "INSERT INTO Usuarios (id, nome, cpf, username, email, senha, nivel, equipe_id) VALUES "
        "('5', 'Ana Silva', '20000000001', 'vendedor1', 'vendedor1@pim.com', 'vendedor123', 'vendedor', '1'),"
        "('6', 'Bruno Costa', '20000000002', 'vendedor2', 'vendedor2@pim.com', 'vendedor123', 'vendedor', '1'),"
        "('7', 'Camila Rocha', '20000000003', 'vendedor3', 'vendedor3@pim.com', 'vendedor123', 'vendedor', '1'),"
        "('8', 'Diego Martins', '20000000004', 'vendedor4', 'vendedor4@pim.com', 'vendedor123', 'vendedor', '1'),"
        "('9', 'Eduarda Pereira', '20000000005', 'vendedor5', 'vendedor5@pim.com', 'vendedor123', 'vendedor', '1'),"
        "('10', 'Felipe Gomes', '20000000006', 'vendedor6', 'vendedor6@pim.com', 'vendedor123', 'vendedor', '1'),"
        "('11', 'Gabriela Ribeiro', '20000000007', 'vendedor7', 'vendedor7@pim.com', 'vendedor123', 'vendedor', '1'),"
        "('12', 'Henrique Carvalho', '20000000008', 'vendedor8', 'vendedor8@pim.com', 'vendedor123', 'vendedor', '2'),"
        "('13', 'Isabela Fernandes', '20000000009', 'vendedor9', 'vendedor9@pim.com', 'vendedor123', 'vendedor', '2'),"
        "('14', 'Joao Oliveira', '20000000010', 'vendedor10', 'vendedor10@pim.com', 'vendedor123', 'vendedor', '2'),"
        "('15', 'Larissa Santos', '20000000011', 'vendedor11', 'vendedor11@pim.com', 'vendedor123', 'vendedor', '2'),"
        "('16', 'Marcos Almeida', '20000000012', 'vendedor12', 'vendedor12@pim.com', 'vendedor123', 'vendedor', '2'),"
        "('17', 'Natalia Barbosa', '20000000013', 'vendedor13', 'vendedor13@pim.com', 'vendedor123', 'vendedor', '2'),"
        "('18', 'Otavio Mendes', '20000000014', 'vendedor14', 'vendedor14@pim.com', 'vendedor123', 'vendedor', '2'),"
        "('19', 'Paula Cardoso', '20000000015', 'vendedor15', 'vendedor15@pim.com', 'vendedor123', 'vendedor', '3'),"
        "('20', 'Rafael Teixeira', '20000000016', 'vendedor16', 'vendedor16@pim.com', 'vendedor123', 'vendedor', '3'),"
        "('21', 'Sabrina Moreira', '20000000017', 'vendedor17', 'vendedor17@pim.com', 'vendedor123', 'vendedor', '3'),"
        "('22', 'Thiago Araujo', '20000000018', 'vendedor18', 'vendedor18@pim.com', 'vendedor123', 'vendedor', '3'),"
        "('23', 'Vanessa Freitas', '20000000019', 'vendedor19', 'vendedor19@pim.com', 'vendedor123', 'vendedor', '3'),"
        "('24', 'Lucas Pinto', '20000000020', 'vendedor20', 'vendedor20@pim.com', 'vendedor123', 'vendedor', '3'),"
        "('25', 'Juliana Duarte', '20000000021', 'vendedor21', 'vendedor21@pim.com', 'vendedor123', 'vendedor', '3') "
        "ON CONFLICT (id) DO NOTHING;"

        /* CLIENTES (um por vendedor, cadastrado por ele) */
        "INSERT INTO Clientes (id, nome, cpf, email, telefone, renda, cadastrado_por) VALUES "
        "('1', 'Alice Nunes', '30000000001', 'alice@email.com', '11990000001', '3250', '5'),"
        "('2', 'Bernardo Lopes', '30000000002', 'bernardo@email.com', '11990000002', '3500', '6'),"
        "('3', 'Cecilia Ramos', '30000000003', 'cecilia@email.com', '11990000003', '3750', '7'),"
        "('4', 'Daniel Farias', '30000000004', 'daniel@email.com', '11990000004', '4000', '8'),"
        "('5', 'Elisa Monteiro', '30000000005', 'elisa@email.com', '11990000005', '4250', '9'),"
        "('6', 'Fabio Castro', '30000000006', 'fabio@email.com', '11990000006', '4500', '10'),"
        "('7', 'Giovana Dias', '30000000007', 'giovana@email.com', '11990000007', '4750', '11'),"
        "('8', 'Hugo Correia', '30000000008', 'hugo@email.com', '11990000008', '5000', '12'),"
        "('9', 'Ingrid Vieira', '30000000009', 'ingrid@email.com', '11990000009', '5250', '13'),"
        "('10', 'Jorge Batista', '30000000010', 'jorge@email.com', '11990000010', '5500', '14'),"
        "('11', 'Karina Moura', '30000000011', 'karina@email.com', '11990000011', '5750', '15'),"
        "('12', 'Leonardo Reis', '30000000012', 'leonardo@email.com', '11990000012', '6000', '16'),"
        "('13', 'Mariana Cunha', '30000000013', 'mariana@email.com', '11990000013', '6250', '17'),"
        "('14', 'Nelson Prado', '30000000014', 'nelson@email.com', '11990000014', '6500', '18'),"
        "('15', 'Olivia Campos', '30000000015', 'olivia@email.com', '11990000015', '6750', '19'),"
        "('16', 'Pedro Machado', '30000000016', 'pedro@email.com', '11990000016', '7000', '20'),"
        "('17', 'Renata Siqueira', '30000000017', 'renata@email.com', '11990000017', '7250', '21'),"
        "('18', 'Samuel Torres', '30000000018', 'samuel@email.com', '11990000018', '7500', '22'),"
        "('19', 'Tatiana Lopes', '30000000019', 'tatiana@email.com', '11990000019', '7750', '23'),"
        "('20', 'Ulisses Rezende', '30000000020', 'ulisses@email.com', '11990000020', '8000', '24'),"
        "('21', 'Viviane Andrade', '30000000021', 'viviane@email.com', '11990000021', '8250', '25') "
        "ON CONFLICT (id) DO NOTHING;"

        /* CONTRATOS (um por cliente, ligado ao vendedor que o cadastrou) */
        "INSERT INTO Contratos (id, cliente_id, vendedor_id, tipo_consorcio_id, valor_credito, data_contrato, status, avaliado_por) VALUES "
        "('1', '1', '5', '1', '55000', '2026-10-02', 'aprovado', '2'),"
        "('2', '2', '6', '1', '60000', '2026-10-03', 'em_analise', NULL),"
        "('3', '3', '7', '1', '65000', '2026-10-05', 'aprovado', '2'),"
        "('4', '4', '8', '1', '70000', '2026-10-06', 'em_analise', NULL),"
        "('5', '5', '9', '1', '75000', '2026-10-08', 'aprovado', '2'),"
        "('6', '6', '10', '1', '80000', '2026-10-09', 'em_analise', NULL),"
        "('7', '7', '11', '1', '85000', '2026-10-12', 'aprovado', '2'),"
        "('8', '8', '12', '1', '90000', '2026-10-13', 'em_analise', NULL),"
        "('9', '9', '13', '1', '95000', '2026-10-14', 'aprovado', '3'),"
        "('10', '10', '14', '1', '100000', '2026-10-15', 'em_analise', NULL),"
        "('11', '11', '15', '1', '105000', '2026-10-16', 'aprovado', '3'),"
        "('12', '12', '16', '1', '110000', '2026-10-19', 'em_analise', NULL),"
        "('13', '13', '17', '1', '115000', '2026-10-20', 'aprovado', '3'),"
        "('14', '14', '18', '1', '120000', '2026-10-21', 'em_analise', NULL),"
        "('15', '15', '19', '1', '125000', '2026-10-22', 'aprovado', '4'),"
        "('16', '16', '20', '1', '130000', '2026-10-23', 'em_analise', NULL),"
        "('17', '17', '21', '1', '135000', '2026-10-26', 'aprovado', '4'),"
        "('18', '18', '22', '1', '140000', '2026-10-27', 'em_analise', NULL),"
        "('19', '19', '23', '1', '145000', '2026-10-28', 'aprovado', '4'),"
        "('20', '20', '24', '1', '150000', '2026-10-29', 'em_analise', NULL),"
        "('21', '21', '25', '1', '155000', '2026-10-30', 'aprovado', '4') "
        "ON CONFLICT (id) DO NOTHING;"

        /* COMPETICOES (uma geral e uma interna da Equipe Alfa, em outubro/2026) */
        "INSERT INTO Competicoes (id, nome, tipo, equipe_id, data_inicio, data_fim, criada_por) VALUES "
        "('1', 'Campeonato Geral de Outubro', 'geral', NULL, '2026-10-01', '2026-10-31', '1'),"
        "('2', 'Desafio Equipe Alfa', 'interna', '1', '2026-10-01', '2026-10-31', '2') "
        "ON CONFLICT (id) DO NOTHING;";

    int resultado = sqlite3_exec(*db, sql, NULL, NULL, &erro);

    if (resultado != SQLITE_OK) {
        printf("|   Erro ao criar tabelas: %-32s|\n", erro);
        sqlite3_free(erro);
    }
    return resultado;
}

/* ---------- TABELA DAS CONSULTAS ---------- */

/* Desenha a linha de borda da tabela:  +----+-------------+
   Cada coluna recebe (largura + 2) tracos: o +2 e o espaco antes e depois do texto. */
void borda_tabela(char **resultado, int colunas, int largura[]) {
    int col, i;

    printf("   +");
    for (col = 0; col < colunas; col++) {
        if (strcmp(resultado[col], "senha") == 0) {
            continue;   /* a coluna senha nao e mostrada */
        }
        for (i = 0; i < largura[col] + 2; i++) {
            printf("-");
        }
        printf("+");
    }
    printf("\n");
}

/* Executa o SELECT e mostra o resultado em forma de tabela:

   +----+-------------+
   | id | nome        |      <- linha 0: nome das colunas
   +----+-------------+
   | 1  | Equipe Alfa |      <- linha 1
   | 2  | Equipe Beta |      <- linha 2
   +----+-------------+

   Cada coluna fica com a largura do seu maior texto.
   Devolve quantas linhas foram mostradas, ou -1 se deu erro.

   O sqlite3_get_table coloca todos os textos, um atras do outro, no vetor "resultado":
     resultado = { "id", "nome", "1", "Equipe Alfa", "2", "Equipe Beta" }
                   |-- linha 0 --|  |---- linha 1 ---|  |---- linha 2 ---|
   Como cada linha tem "colunas" textos, o texto da linha lin, coluna col fica em:
     posicao = lin * colunas + col        (ex.: linha 2, coluna 1 -> 2 * 2 + 1 = 5)  */
int mostrar_tabela(sqlite3 *db, const char *sql) {
    char **resultado;
    int linhas, colunas;
    int largura[MAX_COLUNAS];
    int lin, col, i, posicao, tamanho;
    char *texto;

    if (sqlite3_get_table(db, sql, &resultado, &linhas, &colunas, NULL) != SQLITE_OK) {
        printf("|   Erro ao consultar a tabela.                            |\n");
        return -1;
    }

    if (linhas == 0 || colunas > MAX_COLUNAS) {
        sqlite3_free_table(resultado);
        return 0;
    }

    /* PASSO 1: descobrir a largura de cada coluna = tamanho do seu maior texto */
    for (col = 0; col < colunas; col++) {
        largura[col] = 0;
    }
    for (lin = 0; lin <= linhas; lin++) {
        for (col = 0; col < colunas; col++) {
            posicao = lin * colunas + col;
            texto = resultado[posicao];
            if (texto == NULL) {
                texto = "NULL";
            }
            tamanho = strlen(texto);
            if (tamanho > largura[col]) {
                largura[col] = tamanho;
            }
        }
    }

    /* PASSO 2: desenhar a tabela */
    borda_tabela(resultado, colunas, largura);

    for (lin = 0; lin <= linhas; lin++) {
        printf("   |");
        for (col = 0; col < colunas; col++) {
            if (strcmp(resultado[col], "senha") == 0) {
                continue;   /* a coluna senha nao e mostrada */
            }
            posicao = lin * colunas + col;
            texto = resultado[posicao];
            if (texto == NULL) {
                texto = "NULL";
            }
            printf(" %s", texto);
            /* completa com espacos ate a largura da coluna */
            for (i = strlen(texto); i < largura[col]; i++) {
                printf(" ");
            }
            printf(" |");
        }
        printf("\n");

        if (lin == 0) {
            borda_tabela(resultado, colunas, largura);   /* borda embaixo dos nomes das colunas */
        }
    }

    borda_tabela(resultado, colunas, largura);

    sqlite3_free_table(resultado);
    return linhas;
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
            fprintf(stderr, "|              Banco invalido para consulta.               |\n");
            return 1;
    }

    rc = sqlite3_open("pim2.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "|   Nao foi possivel abrir o banco: %-23s|\n", sqlite3_errmsg(db));
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
        fprintf(stderr, "|   Nao foi possivel contar os registros: %-17s|\n", erro);
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

        printf("|   Tabela: %-15s Offset: %-23d|\n", tabela, offset);

        registros = mostrar_tabela(db, sql);
        if (registros < 0) {
            sqlite3_close(db);
            return 1;
        }

        if (registros == 0 && offset == 0) {
            printf("|               Nenhum registro encontrado.                |\n");
            break;
        }

        if (registros == 0) {
            printf("|           Nao ha mais registros nesta pagina.            |\n");
            offset -= 10;
            continue;
        }

        /* So pagina quando ha mais de 10 registros */
        if (total <= 10) {
            break;
        }

        printf("|   [p] proxima pagina | [a] pagina anterior | [q] sair: ");
        if (scanf(" %c", &opcao) != 1) {
            fprintf(stderr, "|              Nao foi possivel ler a opcao.               |\n");
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
                printf("|                     Opcao invalida.                      |\n");
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
        fprintf(stderr, "|                Id invalido para exclusao.                |\n");
        return 1;
    }

    rc = sqlite3_open("pim2.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "|   Nao foi possivel abrir o banco: %-23s|\n", sqlite3_errmsg(db));
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
        fprintf(stderr, "|   Nao foi possivel arquivar o registro: %-17s|\n", erro);
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
        fprintf(stderr, "|   Nao foi possivel abrir o banco: %-23s|\n", sqlite3_errmsg(db));
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
        fprintf(stderr, "|   Nao foi possivel salvar o registro: %-19s|\n", erro);
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
        fprintf(stderr, "|                 Id invalido para edicao.                 |\n");
        return 1;
    }

    printf("|              qual campo gostaria de editar?              |\n");

    switch (banco) {
        case 'E':
        case 'e':
            tabela = "Equipes";
            printf("|   1 - Nome                                               |\n|   2 - Gestor (id)                                        |\n");
            printf("+----------------------------------------------------------+\n");
            printf("|   Escolha uma opcao: ");
            scanf("%d", &opcao);
            switch (opcao) {
                case 1: coluna = "nome"; break;
                case 2: coluna = "gestor_id"; break;
            }
            break;

        case 'U':
        case 'u':
            tabela = "Usuarios";
            printf("|   1 - Nome                                               |\n|   2 - CPF                                                |\n|   3 - Username                                           |\n|   4 - Email                                              |\n|   5 - Senha                                              |\n|   6 - Nivel                                              |\n|   7 - Equipe (id)                                        |\n");
            printf("+----------------------------------------------------------+\n");
            printf("|   Escolha uma opcao: ");
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
            printf("|   1 - Nome                                               |\n|   2 - CPF                                                |\n|   3 - Email                                              |\n|   4 - Telefone                                           |\n|   5 - Renda                                              |\n");
            printf("+----------------------------------------------------------+\n");
            printf("|   Escolha uma opcao: ");
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
            printf("|   1 - Cliente (id)                                       |\n|   2 - Vendedor (id)                                      |\n|   3 - Valor do credito                                   |\n|   4 - Data do contrato                                   |\n|   5 - Status                                             |\n|   6 - Motivo da reprovacao                               |\n");
            printf("+----------------------------------------------------------+\n");
            printf("|   Escolha uma opcao: ");
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
            printf("|   1 - Nome                                               |\n|   2 - Data de inicio                                     |\n|   3 - Data de fim                                        |\n");
            printf("+----------------------------------------------------------+\n");
            printf("|   Escolha uma opcao: ");
            scanf("%d", &opcao);
            switch (opcao) {
                case 1: coluna = "nome"; break;
                case 2: coluna = "data_inicio"; break;
                case 3: coluna = "data_fim"; break;
            }
            break;

        default:
            fprintf(stderr, "|               Banco invalido para edicao.                |\n");
            return 1;
    }

    limpar_buffer();

    if (coluna == NULL) {
        printf("|                     Opcao invalida.                      |\n");
        return 1;
    }

    printf("|   Novo valor: ");
    fgets(valor, sizeof(valor), stdin);
    tratar_texto(valor);

    snprintf(sql, sizeof(sql),
             "UPDATE %s SET %s = '%s' WHERE id = %d;",
             tabela, coluna, valor, id);

    rc = sqlite3_open("pim2.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "|   Nao foi possivel abrir o banco: %-23s|\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &erro);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "|   Nao foi possivel editar o registro: %-19s|\n", erro);
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
            fprintf(stderr, "|              Banco invalido para consulta.               |\n");
            return 1;
    }

    rc = sqlite3_open("pim2.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "|   Nao foi possivel abrir o banco: %-23s|\n", sqlite3_errmsg(db));
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
            fprintf(stderr, "|   Erro ao verificar existencia do registro: %-13s|\n", erro);
            sqlite3_free(erro);
            sqlite3_close(db);
            return 0;
        }

        sqlite3_close(db);
        return 1; // Retorna 1 se o usuário existe, 0 caso contrário
    }
    
}

#include "funcoes_db.h"
#include <stdio.h>

//     /* Liga as chaves estrangeiras (o SQLite vem com elas desligadas) */
//     //sqlite3_exec(*db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);

/* ---------- CRIACAO DO BANCO E TABELAS ---------- */

int criar_db(sqlite3 **db) {
    char *erro = NULL; /* guarda a mensagem de erro, se der problema */

    int rc = sqlite3_open("pim2.db", db);

    if (rc != SQLITE_OK) {
        printf("Erro ao abrir o banco: %s\n", sqlite3_errmsg(*db));
        return rc;
    }

    const char *sql =
        /* Usuario: equipe_id aponta para a tabela Equipe */
        "CREATE TABLE IF NOT EXISTS Usuarios ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  nome TEXT NOT NULL,"
        "  CPF TEXT NOT NULL,"
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
        "  status TEXT NOT NULL DEFAULT 'ativa' CHECK (status IN ('ativa','arquivada')),"
        "  criado_em TEXT NOT NULL DEFAULT (datetime('now','localtime'))"
        ");"

        /* TipoConsorcio: tipos de consorcio vendidos (imovel, veiculo...) 
        "CREATE TABLE IF NOT EXISTS TiposConsorcio ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  nome TEXT NOT NULL,"
        "  ativo INTEGER NOT NULL DEFAULT 1 CHECK (ativo IN (0,1))"
        ");"*/

        /* Cliente: quem compra o consorcio (dados pessoais, LGPD) */
        "CREATE TABLE IF NOT EXISTS Clientes ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  nome TEXT NOT NULL,"
        "  cpf TEXT NOT NULL UNIQUE CHECK (length(cpf) = 11),"
        "  email TEXT,"
        "  telefone TEXT,"
        "  renda REAL,"
        "  consentimento_lgpd INTEGER NOT NULL DEFAULT 0 CHECK (consentimento_lgpd IN (0,1)),"
        "  data_consentimento TEXT,"
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
        "  status TEXT NOT NULL DEFAULT 'aberta' CHECK (status IN ('aberta','encerrada')),"
        "  criada_por INTEGER NOT NULL REFERENCES Usuarios(id),"
        "  vencedor_id INTEGER REFERENCES Usuarios(id),"
        "  CHECK ((tipo = 'geral' AND equipe_id IS NULL) OR"
        "         (tipo = 'interna' AND equipe_id IS NOT NULL))"
        ");"

        /* Evento: acontecimentos e premios ligados a uma competicao */
        "CREATE TABLE IF NOT EXISTS Eventos ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  competicao_id INTEGER NOT NULL REFERENCES Competicao(id),"
        "  titulo TEXT NOT NULL,"
        "  descricao TEXT,"
        "  premio TEXT,"
        "  data_evento TEXT"
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
        "INSERT INTO Usuarios (id, nome, username, email, senha, nivel) "
        "VALUES ('1','Administrador', 'admin', 'admin@pim.com', 'admin123', 'admin') "
        "ON CONFLICT (id) DO NOTHING;"
        
        "INSERT INTO Usuarios (id, nome, username, email, senha, nivel) "
        "VALUES ('2','Gestor', 'gestor', 'gestor@pim.com', 'gestor123', 'gestor') "
        "ON CONFLICT (id) DO NOTHING;"
        
        "INSERT INTO Usuarios (id, nome, username, email, senha, nivel) "
        "VALUES ('3','Vendedor', 'vendedor', 'vendedor@pim.com', 'vendedor123', 'vendedor') "
        "ON CONFLICT (id) DO NOTHING;";

    int resultado = sqlite3_exec(*db, sql, NULL, NULL, &erro);

    if (resultado != SQLITE_OK) {
        printf("Erro ao criar tabelas: %s\n", erro);
        sqlite3_free(erro);
    }
    return resultado;
}

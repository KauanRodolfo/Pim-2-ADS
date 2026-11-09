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
        "INSERT INTO Contratos (id, cliente_id, vendedor_id, tipo_consorcio_id, valor_credito, status, avaliado_por) VALUES "
        "('1', '1', '5', '1', '55000', 'aprovado', '2'),"
        "('2', '2', '6', '1', '60000', 'em_analise', NULL),"
        "('3', '3', '7', '1', '65000', 'aprovado', '2'),"
        "('4', '4', '8', '1', '70000', 'em_analise', NULL),"
        "('5', '5', '9', '1', '75000', 'aprovado', '2'),"
        "('6', '6', '10', '1', '80000', 'em_analise', NULL),"
        "('7', '7', '11', '1', '85000', 'aprovado', '2'),"
        "('8', '8', '12', '1', '90000', 'em_analise', NULL),"
        "('9', '9', '13', '1', '95000', 'aprovado', '3'),"
        "('10', '10', '14', '1', '100000', 'em_analise', NULL),"
        "('11', '11', '15', '1', '105000', 'aprovado', '3'),"
        "('12', '12', '16', '1', '110000', 'em_analise', NULL),"
        "('13', '13', '17', '1', '115000', 'aprovado', '3'),"
        "('14', '14', '18', '1', '120000', 'em_analise', NULL),"
        "('15', '15', '19', '1', '125000', 'aprovado', '4'),"
        "('16', '16', '20', '1', '130000', 'em_analise', NULL),"
        "('17', '17', '21', '1', '135000', 'aprovado', '4'),"
        "('18', '18', '22', '1', '140000', 'em_analise', NULL),"
        "('19', '19', '23', '1', '145000', 'aprovado', '4'),"
        "('20', '20', '24', '1', '150000', 'em_analise', NULL),"
        "('21', '21', '25', '1', '155000', 'aprovado', '4') "
        "ON CONFLICT (id) DO NOTHING;";

    int resultado = sqlite3_exec(*db, sql, NULL, NULL, &erro);

    if (resultado != SQLITE_OK) {
        printf("Erro ao criar tabelas: %s\n", erro);
        sqlite3_free(erro);
    }
    return resultado;
}

/* ==========================================================
   banco.h
   Structs das tabelas e funcoes de acesso ao banco pim2.db.
   ========================================================== */
#ifndef BANCO_H
#define BANCO_H

#include "sqlite3.h"

/* ---------- CRIACAO DAS TABELAS ---------- */

/* Cria as tabelas que ainda nao existem. Retorna SQLITE_OK se deu certo. */
int criar_db(sqlite3 **db);

/*
 * Structs que representam as tabelas do banco.
 *
 * Convencoes usadas:
 * - Textos sao arrays de char com tamanho fixo (o +1 e o espaco do '\0').
 * - Datas sao texto, no formato que o SQLite usa:
 *     date     -> "AAAA-MM-DD"           (10 caracteres)
 *     datetime -> "AAAA-MM-DD HH:MM:SS"  (19 caracteres)
 * - Chaves estrangeiras que podem ser nulas usam 0 para "sem valor",
 *   ja que os ids gerados pelo banco comecam em 1.
 */

#define COMISSAO 5
#define TAM_P 10
#define TAM 50 /* tamanho maximo dos textos digitados */
#define TAM_NOME 100
#define TAM_USERNAME 50
#define TAM_EMAIL 100
#define TAM_SENHA 50
#define TAM_CPF 11
#define TAM_TELEFONE 20
#define TAM_TITULO 100
#define TAM_PREMIO 100
#define TAM_TEXTO 1000 /* campos "text" (descricao, motivo...) */
#define TAM_DATA 10
#define TAM_DATAHORA 19
#define TAM_FILTRO 256 /* textos de filtro do WHERE montados com snprintf */
#define MAX_COLUNAS 20 /* maximo de colunas que a tabela das consultas consegue mostrar */

/* ---------- Tabelas ---------- */

typedef struct {
    int id;
    char nome[TAM_NOME + 1];
    char cpf[TAM_CPF + 1];
    char username[TAM_USERNAME + 1]; /* unico */
    char email[TAM_EMAIL + 1];       /* unico */
    char senha[TAM_SENHA + 1];
    char nivel[TAM_P];
    int equipe_id; /* FK -> Equipe.id (0 = sem equipe, ex: admin) */
    char status[TAM_P];
    
} Usuario;

typedef struct {
    int id;
    char nome[TAM_NOME + 1];
    int gestor_id;      /* FK -> Usuario.id (0 = sem gestor) */
    char status[TAM_P]; /* ativa | arquivada */
} Equipe;

typedef struct {
    int id;
    char nome[TAM_NOME + 1];
    char cpf[TAM_CPF + 1]; /* unico, so os 11 digitos */
    char email[TAM_EMAIL + 1];
    char telefone[TAM_TELEFONE + 1];
    float renda[TAM_P];
    char status[TAM_P];
    int cadastrado_por; /* FK -> Usuario.id */
} Cliente;

typedef struct {
    int id;
    int cliente_id;        /* FK -> Cliente.id */
    int usuario_id;       /* FK -> Usuario.id */
    int tipo_consorcio_id; /* FK -> TipoConsorcio.id */
    float valor_credito;
    char data_contrato[TAM_DATA + 1];
    char status[TAM_P + 1];                /* +1: "em_analise" tem 10 letras */
    int avaliado_por;                      /* FK -> Usuario.id (0 = ainda nao avaliado) */
    char data_avaliacao[TAM_DATAHORA + 1]; /* vazio se nao avaliado */
    char motivo_reprovacao[TAM_TEXTO + 1]; /* vazio se nao reprovado */
} Contrato;

typedef struct {
    int id;
    char nome[TAM_NOME + 1];
    char tipo[TAM_P];                /* geral | interna */
    int equipe_id;                   /* FK -> Equipe.id (0 = geral, sem equipe) */
    char data_inicio[TAM_DATA + 2];
    char data_fim[TAM_DATA + 2];
    char status[TAM_P];              /* aberta | arquivado */
    int criada_por;                  /* FK -> Usuario.id */
    int vencedor_id;                 /* FK -> Usuario.id (0 = sem vencedor) */
} Competicao;

typedef struct {
    int id;
    int usuario_id;
    char acao[TAM];    
} LogsAuditoria;



/* ---------- ACESSO AS TABELAS ---------- */

/* Executa o SELECT e mostra o resultado em tabela, com cada coluna
   da largura do seu maior texto. Devolve quantas linhas mostrou (-1 se deu erro). */
int mostrar_tabela(sqlite3 *db, const char *sql);

int registro_existe(char ini_tabela, const char *filtro, void *saida);

/* Lista os registros da tabela de 10 em 10.
   banco: e=Equipes u=Usuarios c=Clientes t=Contratos v=Eventos o=Competicoes l=LogsAuditoria
   filtro: condicao do WHERE ou NULL para trazer tudo. */
int consultar(char banco, const char *filtro);

int excluir(char banco, int id);

int editar(char banco, int id);

int salvar(char banco, Equipe *equipe, Usuario *usuario, Cliente *cliente, Contrato *contrato, Competicao *competicao);

#endif

#ifndef MODELOS_H
#define MODELOS_H

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

#define TAM_NOME       100
#define TAM_USERNAME    50
#define TAM_EMAIL      100
#define TAM_SENHA       50
#define TAM_CPF         11
#define TAM_TELEFONE    20
#define TAM_TITULO     100
#define TAM_PREMIO     100
#define TAM_TEXTO     1000   /* campos "text" (descricao, motivo...) */
#define TAM_DATA        10
#define TAM_DATAHORA    19

/* ---------- Enums (campos com valores fixos) ---------- */

/* Usado por Equipe, Usuario e Cliente */
typedef enum {
    STATUS_ATIVO,
    STATUS_ARQUIVADO
} StatusRegistro;

typedef enum {
    NIVEL_VENDEDOR,
    NIVEL_GESTOR,
    NIVEL_ADMIN
} NivelUsuario;

typedef enum {
    CONTRATO_EM_ANALISE,
    CONTRATO_APROVADO,
    CONTRATO_REPROVADO,
    CONTRATO_ARQUIVADO
} StatusContrato;

/* ---------- Tabelas ---------- */

typedef struct {
    int id;
    char nome[TAM_NOME + 1];
    int gestor_id;                    /* FK -> Usuario.id (0 = sem gestor) */
    StatusRegistro status;            /* ativa | arquivada */
    char criado_em[TAM_DATAHORA + 1];
} Equipe;

typedef struct {
    int id;
    char nome[TAM_NOME + 1];
    char username[TAM_USERNAME + 1];  /* unico */
    char email[TAM_EMAIL + 1];        /* unico */
    char senha[TAM_SENHA + 1];
    NivelUsuario nivel;
    int equipe_id;                    /* FK -> Equipe.id (0 = sem equipe, ex: admin) */
    StatusRegistro status;
    char criado_em[TAM_DATAHORA + 1];
} Usuario;

typedef struct {
    int id;
    char nome[TAM_NOME + 1];
    char cpf[TAM_CPF + 1];            /* unico, so os 11 digitos */
    char email[TAM_EMAIL + 1];
    char telefone[TAM_TELEFONE + 1];
    double renda;
    int consentimento_lgpd;           /* 0 = nao, 1 = sim */
    char data_consentimento[TAM_DATAHORA + 1];
    StatusRegistro status;
    int cadastrado_por;               /* FK -> Usuario.id */
    char criado_em[TAM_DATAHORA + 1];
} Cliente;

typedef struct {
    int id;
    int cliente_id;                   /* FK -> Cliente.id */
    int vendedor_id;                  /* FK -> Usuario.id */
    int tipo_consorcio_id;            /* FK -> TipoConsorcio.id */
    double valor_credito;
    char data_contrato[TAM_DATA + 1];
    StatusContrato status;
    int avaliado_por;                 /* FK -> Usuario.id (0 = ainda nao avaliado) */
    char data_avaliacao[TAM_DATAHORA + 1];   /* vazio se nao avaliado */
    char motivo_reprovacao[TAM_TEXTO + 1];   /* vazio se nao reprovado */
    char criado_em[TAM_DATAHORA + 1];
} Contrato;




typedef struct {
    int id;
    int competicao_id;                /* FK -> Competicao.id */
    char titulo[TAM_TITULO + 1];
    char descricao[TAM_TEXTO + 1];
    char premio[TAM_PREMIO + 1];
    char data_evento[TAM_DATAHORA + 1];
} Evento;

int consultar(char banco);
int excluir(int id, char banco);

#endif

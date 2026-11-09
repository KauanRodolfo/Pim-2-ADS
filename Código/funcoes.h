/* ==========================================================
   funcoes.h
   Funcoes de apoio usadas pelos outros arquivos do programa.
   ========================================================== */
#ifndef FUNCOES_H
#define FUNCOES_H

#include "funcoes_db.h"
#include "sqlite3.h"

/* Mostra a mensagem e le o texto digitado em dest. */
void tratar_texto(char *variavel);

void limpar_buffer(void);

/* Callback do sqlite3_exec que imprime cada linha (coluna: valor). */
int mostrar_linha(void *dado, int colunas, char **valores, char **nomes);

int registro_existe(char ini_tabela, const char *filtro, void *saida);

/* Lista os registros da tabela de 10 em 10.
   banco: e=Equipes u=Usuarios c=Clientes t=Contratos v=Eventos o=Competicoes l=LogsAuditoria
   filtro: condicao do WHERE ou NULL para trazer tudo. */
int consultar(char banco, const char *filtro);

int excluir(char banco, int id);

int editar(char banco, int id);

int salvar(char banco, Equipe *equipe, Usuario *usuario, Cliente *cliente, Contrato *contrato, Competicao *competicao);
#endif

/* ==========================================================
   funcoes.h
   Funcoes de apoio usadas pelos outros arquivos do programa.
   ========================================================== */
#ifndef FUNCOES_H
#define FUNCOES_H

#include "sqlite3.h"
#include "funcoes_db.h"



/* Mostra a mensagem e le o texto digitado em dest. */
void tratar_texto(char *variavel);

/* Roda um SELECT e retorna 1 se encontrou pelo menos uma linha, senao 0. */
// int existe_registro(sqlite3 *db, const char *sql);

/* Confere se o usuario admin existe no banco. Se nao existir, cria. */
//void garantir_admin(sqlite3 *db);


int consultar(char banco);

int excluir(int id, char banco);

int salvar(char banco, Equipe *equipe, Usuario *usuario, Cliente *cliente, Contrato *contrato, Evento *evento);
#endif

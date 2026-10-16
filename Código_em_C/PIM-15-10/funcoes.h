/* ==========================================================
   funcoes.h
   Funcoes de apoio usadas pelos outros arquivos do programa.
   ========================================================== */
#ifndef FUNCOES_H
#define FUNCOES_H

#include "sqlite3.h"

#define TAM 50   /* tamanho maximo dos textos digitados */

/* Mostra a mensagem e le o texto digitado em dest. */
void ler_texto(const char *msg, char *dest, int tam);

/* Roda um SELECT e retorna 1 se encontrou pelo menos uma linha, senao 0. */
int existe_registro(sqlite3 *db, const char *sql);

/* Confere se o usuario admin existe no banco. Se nao existir, cria. */
void garantir_admin(sqlite3 *db);

#endif

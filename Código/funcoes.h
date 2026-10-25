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

int consultar(char banco);

int excluir(int id, char banco);

int salvar(char banco, Equipe *equipe, Usuario *usuario, Cliente *cliente, Contrato *contrato);
#endif

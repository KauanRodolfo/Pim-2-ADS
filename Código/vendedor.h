#ifndef VENDEDOR_H
#define VENDEDOR_H

#include "funcoes_db.h"

// Ajuste temporario - Kauan: declara o menu do vendedor para ser chamado pelo main.c
void menu_vendedor(Usuario u);

void contratos();
void cadastro_contratos();
void gerenciar_clientes();
int cadastro_clientes();
void ver_ranking_geral();
void ver_ranking_equipe();

#endif

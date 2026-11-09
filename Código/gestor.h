#ifndef GESTOR_H
#define GESTOR_H

#include "funcoes_db.h"

// Ajuste temporario - Kauan: declara o menu para ser utilizado pelo main.c
void menu_gestor(Usuario u);

void gerenciar_vendedores();
void gerenciar_contratos(Usuario u);
void avaliar_contrato(Usuario u);
void ranking_equipe_gestor(Usuario u);
void comissao_equipe(int equipe_id);
int cadastro_vendedor();
void competicao_equipe(Usuario gestor);

#endif

/* ==========================================================
   admin.h
   Tela do admin.
   ========================================================== */
#ifndef ADMIN_H
#define ADMIN_H

#include "funcoes_db.h"

/* Menu do admin. Fica nele ate o admin escolher sair da conta. */
void menu_admin(Usuario u);

void gerenciar_gestores();
int cadastro_gestor();
void gerenciar_ciclos();
void ranking_geral_admin();
void consultar_log();
void arquivar_gestor();
void atualizar_gestor();

#endif

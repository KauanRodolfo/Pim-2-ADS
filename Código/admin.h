/* ==========================================================
   admin.h
   Menus do admin.
   ========================================================== */
#ifndef ADMIN_H
#define ADMIN_H

#include "banco.h"

void menu_admin(Usuario u);
void gerenciar_gestores();
void gerenciar_equipes();
void gerenciar_competicoes(Usuario u);
void ranking_geral_admin();
void consultar_log();

#endif

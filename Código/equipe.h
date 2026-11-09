/* ==========================================================
   equipe.h
   Gerenciamento de equipes.
   ========================================================== */
#ifndef EQUIPE_H
#define EQUIPE_H

void gerenciar_equipes();
int cadastro_equipe();
int editar_equipe_vendedor(int usuario_id, int equipe_id);
void definir_gestor_equipe();
void mover_vendedor_equipe();
void arquivar_equipe();

#endif

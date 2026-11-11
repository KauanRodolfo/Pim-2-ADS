/* ==========================================================
   equipe.h
   Cadastro e manutencao de equipes.
   ========================================================== */
#ifndef EQUIPE_H
#define EQUIPE_H

int cadastro_equipe();
int editar_equipe_vendedor(int usuario_id, int equipe_id);
int editar_equipe_gestor(int equipe_id, int gestor_id);
void definir_gestor_equipe();
void mover_vendedor_equipe();
void arquivar_equipe();

#endif

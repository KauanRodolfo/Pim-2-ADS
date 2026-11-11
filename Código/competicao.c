/* ==========================================================
   competicao.c
   Competicoes entre vendedores.
   ========================================================== */
#include "competicao.h"
#include "util.h"
#include "banco.h"
#include <stdio.h>
#include <string.h>


/* Cadastra uma competicao interna para a equipe do gestor */
void competicao_equipe(Usuario gestor) {
    Competicao competicao;
    int op;
    char data[TAM_DATA + 2];   /* variavel temporaria */

    if (gestor.equipe_id == 0) {
        printf("|        Voce nao esta vinculado a nenhuma equipe.         |\n");
        return;
    }

    printf("\n+----------------------------------------------------------+\n");
    printf("|              CADASTRAR COMPETICAO DA EQUIPE              |\n");
    printf("+----------------------------------------------------------+\n");

    printf("|   Nome da competicao: ");
    fgets(competicao.nome, sizeof(competicao.nome), stdin);

    printf("|   Data de inicio (dd/mm/aaaa): ");
    fgets(data, sizeof(data), stdin);
    sprintf(competicao.data_inicio, "%.4s-%.2s-%.2s\n", data + 6, data + 3, data);

    printf("|   Data de fim (dd/mm/aaaa): ");
    fgets(data, sizeof(data), stdin);
    sprintf(competicao.data_fim, "%.4s-%.2s-%.2s\n", data + 6, data + 3, data);

    strcpy(competicao.tipo, "interna");
    competicao.equipe_id = gestor.equipe_id;
    competicao.criada_por = gestor.id;

    printf("|   Deseja cadastrar a competicao? (1 para sim, 0 para nao): ");
    scanf("%d", &op);
    limpar_buffer();
    if (op != 1) {
        printf("|                   Operacao cancelada.                    |\n");
        return;
    }

    if (salvar('o', NULL, NULL, NULL, NULL, &competicao) == 0) {
        printf("|            Competicao cadastrada com sucesso.            |\n");
    } else {
        printf("|              Erro ao cadastrar competicao.               |\n");
    }
}


/* ==========================================================
   vendedor.c
   Tela do vendedor.
   ========================================================== */

#include "vendedor.h"
#include "funcoes.h"
#include "funcoes_db.h"
#include <stdio.h>
#include <string.h>

// Declaracao das funcoes
void gerenciar_contratos();
void cadastro_contratos();

// Menu principal do vendedor
void menu_vendedor() {
    int op;

    do {
        printf("\n=== MENU vendedor ===\n");
        printf("1 - Gerenciar clientes\n");
        printf("2 - Gerenciar contratos\n");
        printf("3 - Ver Ranking Geral\n");
        printf("0 - Sair da conta\n");

        scanf("%d", &op);

        switch (op) {
            case 1:
                // Gerenciar clientes
                break;

            case 2:
                gerenciar_contratos();
                break;

            case 3:
                // Ver Ranking Geral
                break;

            case 0:
                break;

            default:
                printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

// Funcao gerenciar contratos
void gerenciar_contratos() {
    int op;

    do {
        printf("\n=== GERENCIAR CONTRATOS ===\n");
        printf("1 - Cadastrar contratos\n");
        printf("2 - Consultar contratos\n");
        printf("0 - Voltar\n");

        scanf("%d", &op);

        switch (op) {
            case 1:
                cadastro_contratos();
                break;

            case 2:
                printf("Consulta de contratos em desenvolvimento.\n");
                break;

            case 0:
                printf("Voltando ao menu do vendedor...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (op != 0);
}

/* ==========================================================
   Cadastro de contratos
   ========================================================== */

void cadastro_contratos() {
    Contrato contrato = {0};

    printf("\n=== CADASTRAR CONTRATO ===\n");

    printf("ID do cliente: ");
    if (scanf("%d", &contrato.cliente_id) != 1 ||
        contrato.cliente_id <= 0) {
        printf("ID do cliente invalido.\n");
        return;
    }

    printf("ID do vendedor: ");
    if (scanf("%d", &contrato.usuario_id) != 1 ||
        contrato.usuario_id <= 0) {
        printf("ID do vendedor invalido.\n");
        return;
    }

    printf("ID do tipo de consorcio: ");
    if (scanf("%d", &contrato.tipo_consorcio_id) != 1 ||
        contrato.tipo_consorcio_id <= 0) {
        printf("Tipo de consorcio invalido.\n");
        return;
    }

    printf("Valor do credito: R$ ");
    if (scanf("%f", &contrato.valor_credito) != 1 ||
        contrato.valor_credito <= 0) {
        printf("Valor invalido.\n");
        return;
    }

    strcpy(contrato.status, "em_analise");

    printf("\n--- DADOS DO CONTRATO ---\n");
    printf("Cliente ID: %d\n", contrato.cliente_id);
    printf("Vendedor ID: %d\n", contrato.usuario_id);
    printf("Tipo de consorcio ID: %d\n",
           contrato.tipo_consorcio_id);
    printf("Valor do credito: R$ %.2f\n",
           contrato.valor_credito);
    printf("Status: %s\n", contrato.status);

    printf("\nDados coletados. Persistencia ainda pendente.\n");
}

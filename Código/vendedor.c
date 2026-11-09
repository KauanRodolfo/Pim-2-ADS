/* ==========================================================
   vendedor.c
   Tela do vendedor.
   ========================================================== */

#include "vendedor.h"
#include "funcoes.h"
#include "funcoes_db.h"
#include "ranking.h"

#include <stdio.h>
#include <string.h>

/* ==========================================================
   Menu principal do vendedor
   ========================================================== */

void menu_vendedor(Usuario u) {
    int op;

    do {
        printf("\n=== MENU VENDEDOR ===\n");
        printf("1 - Gerenciar clientes\n");
        printf("2 - Gerenciar contratos\n");
        printf("3 - Ver Ranking Geral\n");
        printf("4 - Ver Ranking da Equipe\n");
        printf("0 - Sair da conta\n");

        scanf("%d", &op);

        switch (op) {
            case 1:
                gerenciar_clientes();
                break;

            case 2:
                gerenciar_contratos();
                break;

            case 3:
                ranking_geral();
                break;

            case 4:
                ranking_equipe(u.equipe_id);
                break;

            case 0:
                printf("Saindo da conta...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (op != 0);
}

/* ==========================================================
   Gerenciamento de contratos
   ========================================================== */

void contratos() {
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
                consultar('t', NULL);
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
    Contrato contrato;
    char confirmar;
    char filtro[TAM];
    int op;

    printf("\n=== CADASTRAR CONTRATO ===\n");

    printf("ID do cliente: ");
    do{
        scanf("%d", &contrato.cliente_id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND status = 'ativo'", contrato.cliente_id);
        if(registro_existe('c', filtro, NULL) == 0) {
            printf("ID do cliente nao encontrado. \n gostaria de tentar novamente? (1 para sim, 0 para nao): ");
            scanf("%d", &op);
            limpar_buffer();
            if(op == 0) {
                printf("Operacao cancelada.\n");
                return;
            }
        } else {
            break;
        }
    }while(op);

    printf("ID do vendedor: ");  //precisa ser o id do vendedor logado
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

    // Status inicial do contrato
    strcpy(contrato.status, "em_analise");

    // Exibe os dados informados
    printf("\n--- DADOS DO CONTRATO ---\n");
    printf("Cliente ID: %d\n", contrato.cliente_id);
    printf("Vendedor ID: %d\n", contrato.usuario_id);
    printf("Tipo de consorcio ID: %d\n",
           contrato.tipo_consorcio_id);
    printf("Valor do credito: R$ %.2f\n",
           contrato.valor_credito);
    printf("Status: %s\n", contrato.status);

    // Confirmacao do contrato
    printf("\nConfirmar cadastro do contrato? (s/n): ");
    scanf(" %c", &confirmar);

    if (confirmar == 's' || confirmar == 'S') {
        printf("\nContrato confirmado!\n");
        printf("Persistencia no banco ainda pendente.\n");

    } else if (confirmar == 'n' || confirmar == 'N') {
        printf("\nCadastro do contrato cancelado.\n");

    } else {
        printf("\nOpcao invalida. Cadastro cancelado.\n");
    }
}

/* ==========================================================
   Gerenciamento de clientes
   ========================================================== */

void gerenciar_clientes() {
    int op;

    do {
        printf("\n=== GERENCIAR CLIENTES ===\n");
        printf("1 - Cadastrar cliente\n");
        printf("2 - Consultar clientes\n");
        printf("0 - Voltar\n");

        scanf("%d", &op);

        switch (op) {
            case 1:
                cadastro_clientes();
                break;

            case 2:
                consultar('c', NULL);
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
   Cadastro de clientes
   ========================================================== */

int cadastro_clientes() {
    Cliente cliente = {0};
    char confirmar;
    char renda[TAM_P];
    int rc;

    limpar_buffer();

    printf("\n=== CADASTRAR CLIENTE ===\n");

    printf("Nome completo: ");
    fgets(cliente.nome, sizeof(cliente.nome), stdin);

    printf("CPF (11 numeros): ");
    fgets(cliente.cpf, sizeof(cliente.cpf), stdin);
    limpar_buffer();
    
    printf("Email: ");
    fgets(cliente.email, sizeof(cliente.email), stdin);

    printf("Telefone: ");
    fgets(cliente.telefone, sizeof(cliente.telefone), stdin);

    printf("Renda mensal: R$ ");
    fgets(renda, sizeof(renda), stdin);
    sscanf(renda, "%f", &cliente.renda[0]);

    strcpy(cliente.status, "ativo");
    cliente.cadastrado_por = 3;

    printf("\n=== CONFIRMAR DADOS ===\n");
    printf("Nome: %s\n", cliente.nome);
    printf("CPF: %s\n", cliente.cpf);
    printf("Email: %s\n", cliente.email);
    printf("Telefone: %s\n", cliente.telefone);
    printf("Renda: R$ %.2f\n", cliente.renda[0]);

    printf("\nConfirmar cadastro? (s/n): ");
    scanf(" %c", &confirmar);

    if (confirmar == 's' || confirmar == 'S') {
        rc = salvar('c', NULL, NULL, &cliente, NULL, NULL);
        if (rc == 0) {
            printf("\nCliente cadastrado com sucesso.\n");
            return 0;
        }
    } else if (confirmar == 'n' || confirmar == 'N') {
        printf("\nCadastro cancelado.\n");
    } else {
        printf("\nOpcao invalida. Cadastro cancelado.\n");
    }

    return 1;
}

/* ==========================================================
   Visualizacao do Ranking Geral
   ========================================================== */

void ver_ranking_geral() {

    printf("\n=== RANKING GERAL ===\n");
    printf("Periodo de apuracao: mensal\n");
    printf("Ranking geral em desenvolvimento.\n");
    printf("Integracao com o banco de dados pendente.\n");
}

/* ==========================================================
   Visualizacao do Ranking da Equipe
   ========================================================== */

void ver_ranking_equipe() {

    printf("\n=== RANKING DA EQUIPE ===\n");
    printf("Periodo de apuracao: mensal\n");
    printf("Ranking da equipe em desenvolvimento.\n");
    printf("Integracao com o banco de dados pendente.\n");
}

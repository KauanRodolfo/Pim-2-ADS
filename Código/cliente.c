/* ==========================================================
   cliente.c
   Cadastro de clientes.
   ========================================================== */
#include "cliente.h"
#include "util.h"
#include "banco.h"
#include <stdio.h>
#include <string.h>


/* ==========================================================
   Cadastro de clientes
   ========================================================== */

int cadastro_clientes() {
    Cliente cliente = {0};
    char confirmar;
    char renda[TAM_P];
    int rc;

    limpar_buffer();

    printf("\n+----------------------------------------------------------+\n");
    printf("|                    CADASTRAR CLIENTE                     |\n");
    printf("+----------------------------------------------------------+\n");

    printf("|   Nome completo: ");
    fgets(cliente.nome, sizeof(cliente.nome), stdin);
    tratar_texto(cliente.nome);

    printf("|   CPF (11 numeros): ");
    fgets(cliente.cpf, sizeof(cliente.cpf), stdin);
    tratar_texto(cliente.cpf);
    
    printf("|   Email: ");
    fgets(cliente.email, sizeof(cliente.email), stdin);
    tratar_texto(cliente.email);

    printf("|   Telefone: ");
    fgets(cliente.telefone, sizeof(cliente.telefone), stdin);
    tratar_texto(cliente.telefone);

    printf("|   Renda mensal: R$ ");
    fgets(renda, sizeof(renda), stdin);
    sscanf(renda, "%f", &cliente.renda[0]);

    strcpy(cliente.status, "ativo");
    cliente.cadastrado_por = 3;

    printf("\n+----------------------------------------------------------+\n");
    printf("|                     CONFIRMAR DADOS                      |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Nome: %-49s|\n", cliente.nome);
    printf("|   CPF: %-50s|\n", cliente.cpf);
    printf("|   Email: %-48s|\n", cliente.email);
    printf("|   Telefone: %-45s|\n", cliente.telefone);
    printf("|   Renda: R$ %-45.2f|\n", cliente.renda[0]);

    printf("|   Confirmar cadastro? (s/n): ");
    scanf(" %c", &confirmar);

    if (confirmar == 's' || confirmar == 'S') {
        rc = salvar('c', NULL, NULL, &cliente, NULL, NULL);
        if (rc == 0) {
            printf("|             Cliente cadastrado com sucesso.              |\n");
            return 0;
        }
    } else if (confirmar == 'n' || confirmar == 'N') {
        printf("|                   Cadastro cancelado.                    |\n");
    } else {
        printf("|           Opcao invalida. Cadastro cancelado.            |\n");
    }

    return 1;
}

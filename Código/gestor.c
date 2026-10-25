/* ==========================================================
   gestor.c
   Tela do gestor. Por enquanto so o menu esta pronto: as opcoes
   ainda nao fazem nada (as funcoes de cada uma serao feitas depois).
   ========================================================== */
#include "gestor.h"
#include "funcoes.h"
#include "funcoes_db.h"
#include <stdio.h>

void menu_gestor() {
    int op;

    do {
        printf("\n=== MENU gestor ===\n");
        printf("1 - Gerenciar Vendedores\n");
        printf("2 - Gerenciar Contratos\n");
        printf("4 - Gerenciar Ranking da equipe\n");
        printf("0 - Sair da conta\n");
        scanf("%d", &op);

        switch (op) {
        case 1:
            // AQUI: chamar a funcao
            break;

        case 2:
            // AQUI: chamar a funcao
            break;
        case 3:
            // AQUI: chamar a funcao
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

void gerenciar_vendedores(){
    int op;
    do {
        printf("\n=== Gerenciar Vendedores ===\n");
        printf("1 - Listar Vendedores\n");
        printf("2 - Cadastrar Vendedor\n");
        printf("3 - Arquivar Vendedor\n");
        printf("4 - Atualizar Vendedor\n");
        printf("0 - Voltar\n");
        scanf("%d", &op);

        switch (op) {
        case 1:
            consultar('u');  //adicionar filtro where
            break;

        case 2:
            cadastro_vendedor();
            break;

        case 3:
            //excluir();
            break;

        case 0:
            return;
        default:
            printf("Opcao invalida.\n");
        }
    } while (op != 0);
}


int cadastro_vendedor(){

    Usuario vendedor;
    char op;
    int i;

    printf("Nome do vendedor: ");
    fgets(vendedor.nome, sizeof(vendedor.nome), stdin);

    printf("\nNome de usuario do vendedor: ");
    fgets(vendedor.username, sizeof(vendedor.username), stdin);

    printf("\nEmail do vendedor: ");
    fgets(vendedor.email, sizeof(vendedor.email), stdin);

    printf("\nCPF do vendedor: ");
    fgets(vendedor.cpf, sizeof(vendedor.cpf), stdin);

    printf("\nA senha será os 5 ultimos numeros do CPF");
    for (i = 7; i < 12; i++) {
        vendedor.senha[i-7] = vendedor.cpf[i];
    }
    strcpy(vendedor.status, "ativo");
    strcpy(vendedor.nivel, "vendedor");

    printf("\nGostaria de atribuir o vendedor a uma equipe? Sim ou Não? (s ou n)");
    scanf("%c", &op);

    switch(op){
        case 's':
            consultar('e');
            scanf("%d", &vendedor.equipe_id);
            break;
        case 'n':
            vendedor.equipe_id = NULL;
            break;
        default:
            printf("Opcao invalida.\n");
    }

    if (!salvar('u', NULL, &vendedor, NULL, NULL)) {
        return 0;
    }
}

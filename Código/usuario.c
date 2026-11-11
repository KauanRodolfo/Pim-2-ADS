/* ==========================================================
   usuario.c
   Cadastro, atualizacao e arquivamento de gestores e vendedores.
   ========================================================== */
#include "usuario.h"
#include "util.h"
#include "banco.h"
#include <stdio.h>
#include <string.h>


int cadastro_gestor() {
    Usuario gestor;
    Equipe equipe;
    char confirmar;
    int i,rc, op;
    char filtro[TAM_FILTRO];

    limpar_buffer();

    printf("\n+----------------------------------------------------------+\n");
    printf("|                     CADASTRAR GESTOR                     |\n");
    printf("+----------------------------------------------------------+\n");

    printf("|   Nome do gestor: ");
    fgets(gestor.nome, sizeof(gestor.nome), stdin);
    tratar_texto(gestor.nome);

    printf("|   Nome de usuario do gestor: ");
    fgets(gestor.username, sizeof(gestor.username), stdin);
    tratar_texto(gestor.username);

    printf("|   Email do gestor: ");
    fgets(gestor.email, sizeof(gestor.email), stdin);
    tratar_texto(gestor.email);

    printf("|   CPF do gestor: ");
    fgets(gestor.cpf, sizeof(gestor.cpf), stdin);
    tratar_texto(gestor.cpf);

    for (i = 6; i < 11; i++) {
        gestor.senha[i - 6] = gestor.cpf[i];
    }
    gestor.senha[5] = '\0';

    printf("|   Gostaria de atribuir o Gestor como lider de uma equipe? Sim ou Não? (1 para sim, 0 para nao)");// vendedor deve ser atribuido a equipe do gestor
    scanf(" %d", &op);

    switch(op){
        case 1:
            printf("|   1 - Digitar ID da equipe                               |\n");
            printf("|   2 - Consultar equipes                                  |\n");
            printf("|   0 - Cancelar                                           |\n");
            printf("+----------------------------------------------------------+\n");
            printf("|   Escolha uma opcao: ");
            scanf("%d", &op);
            limpar_buffer();
            if (op == 2) {
                consultar('e', "status = 'ativa'");
            } else if (op != 1) {
                printf("|                   Operacao cancelada.                    |\n");
                return 1;
            }
            printf("|   Entre com o ID da equipe: ");
            do {
                scanf("%d", &gestor.equipe_id);
                limpar_buffer();
                snprintf(filtro, sizeof(filtro), "id = %d AND status = 'ativa'", gestor.equipe_id);
                if(registro_existe('e', filtro, &equipe) == 0) {
                    printf("|               ID da equipe nao encontrado.               |\n|   gostaria de tentar novamente? (1 para sim, 0 para nao): ");
                    scanf("%d", &op);
                    limpar_buffer();
                    if(op == 0) {
                        printf("|                   Operacao cancelada.                    |\n");
                        return 1;
                    }
                } else {
                    printf("|   Equipe selecionada: %-35s|\n", equipe.nome);
                    break;
                }
            }while(op);
            break;
        case 0:
            gestor.equipe_id = 0;  // Ajuste temporario - Kauan: equipe_id e int; 0 representa ausencia de equipe
            break;
        default:
            printf("|                     Opcao invalida.                      |\n");
            return 1;
    }
   
    strcpy(gestor.nivel, "gestor");
    strcpy(gestor.status, "ativo");

    printf("\n+----------------------------------------------------------+\n");
    printf("|                     CONFIRMAR DADOS                      |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Nome: %-49s|\n", gestor.nome);
    printf("|   Nome de usuario: %-38s|\n", gestor.username);
    printf("|   Email: %-48s|\n", gestor.email);
    printf("|   CPF: %-50s|\n", gestor.cpf);
    printf("|   Equipe: %-47d|\n", gestor.equipe_id);

    printf("|   Confirmar cadastro? (s/n): ");
    scanf(" %c", &confirmar);

    if (confirmar != 's' && confirmar != 'S') {
        printf("|                   Cadastro cancelado.                    |\n");
        return 1;
    }

    rc = salvar('u', NULL, &gestor, NULL, NULL, NULL);
    if (rc == 0) {
        printf("|              Gestor cadastrado com sucesso.              |\n");
        return 0;
    }

    return 1;
}

void arquivar_gestor() {
    Usuario gestor;
    char filtro[TAM_FILTRO];
    int op;
    printf("|           Selecione o gestor a ser arquivado:            |\n");
    printf("|   1 - Digitar ID do gestor                               |\n");
    printf("|   2 - Consultar gestores                                 |\n");
    printf("|   0 - Cancelar                                           |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Escolha uma opcao: ");
    scanf("%d", &op);
    limpar_buffer();
    if (op == 2) {
        consultar('u', "nivel = 'gestor'");
    } else if (op != 1) {
        printf("|                   Operacao cancelada.                    |\n");
        return;
    }
    printf("|   Entre com o ID do gestor a ser arquivado: ");

    do{
        scanf("%d", &gestor.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND nivel = 'gestor' AND status = 'ativo'", gestor.id);
        if(registro_existe('u', filtro, &gestor) == 0) {
            printf("|               ID do gestor nao encontrado.               |\n|   gostaria de tentar novamente? (1 para sim, 0 para nao): ");
            scanf("%d", &op);
            limpar_buffer();
            if(op == 0) {
                printf("|                   Operacao cancelada.                    |\n");
                return;
            }
        } else {
            break;
        }
    }while(op);

    printf("|   Deseja realmente arquivar o gestor %s? (1 para sim, 0 para nao): ", gestor.nome);
    scanf("%d", &op);
    limpar_buffer();

    if(op == 1) {
        if(excluir('u', gestor.id)){
            printf("|                Erro ao arquivar o gestor.                |\n");
        } else {
            printf("|              gestor arquivado com sucesso.               |\n");
        }
    } else {
        printf("|                   Operacao cancelada.                    |\n");
    }
}
void atualizar_gestor() {
    Usuario gestor;
    char filtro[TAM_FILTRO];
    int op, id;
    printf("|           Selecione o gestor a ser atualizado:           |\n");
    printf("|   1 - Digitar ID do gestor                               |\n");
    printf("|   2 - Consultar gestores                                 |\n");
    printf("|   0 - Cancelar                                           |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Escolha uma opcao: ");
    scanf("%d", &op);
    limpar_buffer();
    if (op == 2) {
        consultar('u', "nivel = 'gestor'");
    } else if (op != 1) {
        printf("|                   Operacao cancelada.                    |\n");
        return;
    }
    printf("|   Entre com o ID do gestor a ser atualizado: ");

    do{
        scanf("%d", &id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND nivel = 'gestor' AND status = 'ativo'", id);
        if(registro_existe('u', filtro, &gestor) == 0) {
            printf("|               ID do gestor nao encontrado.               |\n|   gostaria de tentar novamente? (1 para sim, 0 para nao): ");
            scanf("%d", &op);
            limpar_buffer();
            if(op == 0) {
                printf("|                   Operacao cancelada.                    |\n");
                return;
            }
        }else{
           break;
        }
    }while(op);

    printf("|   Deseja realmente atualizar o gestor %s? (1 para sim, 0 para nao): ", gestor.nome);
    scanf("%d", &op);
    limpar_buffer();

    if(op == 1) {
        if(editar('u', gestor.id) == 0) {
            printf("|              Gestor atualizado com sucesso.              |\n");
        } else {
            printf("|               Erro ao atualizar o gestor.                |\n");
        }
    } else {
        printf("|                   Operacao cancelada.                    |\n");
    }

}

/* ==========================================================
Cadastro de vendedores
========================================================== */

int cadastro_vendedor(){
    Equipe equipe;
    char filtro[TAM];
    Usuario vendedor;
    char confirmar;
    int i,rc , op;

    limpar_buffer();

    printf("|   Nome do vendedor: ");
    fgets(vendedor.nome, sizeof(vendedor.nome), stdin);
    tratar_texto(vendedor.nome);

    printf("|   Nome de usuario do vendedor: ");
    fgets(vendedor.username, sizeof(vendedor.username), stdin);
    tratar_texto(vendedor.username);

    printf("|   Email do vendedor: ");
    fgets(vendedor.email, sizeof(vendedor.email), stdin);
    tratar_texto(vendedor.email);

    printf("|   CPF do vendedor: ");
    fgets(vendedor.cpf, sizeof(vendedor.cpf), stdin);
    tratar_texto(vendedor.cpf);

    printf("|   A senha será os 5 ultimos numeros do CPF");
    for (i = 6; i < 11; i++) {
        vendedor.senha[i-6] = vendedor.cpf[i];
    }

    vendedor.senha[5] = '\0';
    strcpy(vendedor.status, "ativo");
    strcpy(vendedor.nivel, "vendedor");

    printf("|   Gostaria de atribuir o vendedor a uma equipe? Sim ou Não? (1 para sim, 0 para nao)");// vendedor deve ser atribuido a equipe do gestor
    scanf(" %d", &op);

    switch(op){
        case 1:
            printf("|   1 - Digitar ID da equipe                               |\n");
            printf("|   2 - Consultar equipes                                  |\n");
            printf("|   0 - Cancelar                                           |\n");
            printf("+----------------------------------------------------------+\n");
            printf("|   Escolha uma opcao: ");
            scanf("%d", &op);
            limpar_buffer();
            if (op == 2) {
                consultar('e', "status = 'ativa'");
            } else if (op != 1) {
                printf("|                   Operacao cancelada.                    |\n");
                return 1;
            }
            printf("|   Entre com o ID da equipe: ");
            do {
                scanf("%d", &vendedor.equipe_id);
                limpar_buffer();
                snprintf(filtro, sizeof(filtro), "id = %d AND status = 'ativa'", vendedor.equipe_id);
                if(registro_existe('e', filtro, &equipe) == 0) {
                    printf("|               ID da equipe nao encontrado.               |\n|   gostaria de tentar novamente? (1 para sim, 0 para nao): ");
                    scanf("%d", &op);
                    limpar_buffer();
                    if(op == 0) {
                        printf("|                   Operacao cancelada.                    |\n");
                        return 1;
                    }
                } else {
                    printf("|   Equipe selecionada: %-35s|\n", equipe.nome);
                    break;
                }
            }while(op);
            break;
        case 0:
            vendedor.equipe_id = 0;  // Ajuste temporario - Kauan: equipe_id e int; 0 representa ausencia de equipe
            break;
        default:
            printf("|                     Opcao invalida.                      |\n");
            return 1;
    }
    
    

    printf("\n+----------------------------------------------------------+\n");
    printf("|                     CONFIRMAR DADOS                      |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Nome: %-49s|\n", vendedor.nome);
    printf("|   Nome de usuario: %-38s|\n", vendedor.username);
    printf("|   Email: %-48s|\n", vendedor.email);
    printf("|   CPF: %-50s|\n", vendedor.cpf);
    printf("|   Equipe ID: %-44d|\n", vendedor.equipe_id);

    printf("|   Confirmar cadastro? (s/n): ");
    scanf(" %c", &confirmar);

    if (confirmar != 's' && confirmar != 'S') {
        printf("|                   Cadastro cancelado.                    |\n");
        return 1;
    }

    rc = salvar('u', NULL, &vendedor, NULL, NULL, NULL);
    if (!rc) {
        printf("|   Vendedor cadastrado com sucesso");
        return 0;
    }

    return 1;
}
void atualizar_vendedor() {
    Usuario vendedor;
    char filtro[TAM_FILTRO];
    int op;
    printf("|          Selecione o vendedor a ser atualizado:          |\n");
    printf("|   1 - Digitar ID do vendedor                             |\n");
    printf("|   2 - Consultar vendedores                               |\n");
    printf("|   0 - Cancelar                                           |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Escolha uma opcao: ");
    scanf("%d", &op);
    limpar_buffer();
    if (op == 2) {
        consultar('u', "nivel = 'vendedor'");
    } else if (op != 1) {
        printf("|                   Operacao cancelada.                    |\n");
        return;
    }
    printf("|   Entre com o ID do vendedor a ser atualizado: ");

    do{
        scanf("%d", &vendedor.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND nivel = 'vendedor' AND status = 'ativo'", vendedor.id);
        if(registro_existe('u', filtro, &vendedor) == 0) {
            printf("|              ID do vendedor nao encontrado.              |\n|   gostaria de tentar novamente? (1 para sim, 0 para nao): ");
            scanf("%d", &op);
            limpar_buffer();
            if(op == 0) {
                printf("|                   Operacao cancelada.                    |\n");
                return;
            }
        }else{
        break;
        }
    }while(op);

    printf("|   Deseja realmente atualizar o vendedor %s? (1 para sim, 0 para nao): ", vendedor.nome);
    scanf("%d", &op);
    limpar_buffer();

    if(op == 1) {
        if(editar('u', vendedor.id) == 0) {
            printf("|             vendedor atualizado com sucesso.             |\n");
        } else {
            printf("|              Erro ao atualizar o vendedor.               |\n");
        }
    } else {
        printf("|                   Operacao cancelada.                    |\n");
    }

}
void arquivar_vendedor() {
    Usuario vendedor;
    char filtro[TAM_FILTRO];
    int op;
    printf("|          Selecione o vendedor a ser arquivado:           |\n");
    printf("|   1 - Digitar ID do vendedor                             |\n");
    printf("|   2 - Consultar vendedores                               |\n");
    printf("|   0 - Cancelar                                           |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|   Escolha uma opcao: ");
    scanf("%d", &op);
    limpar_buffer();
    if (op == 2) {
        consultar('u', "nivel = 'vendedor'");
    } else if (op != 1) {
        printf("|                   Operacao cancelada.                    |\n");
        return;
    }
    printf("|   Entre com o ID do vendedor a ser arquivado: ");

    do{
        scanf("%d", &vendedor.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND nivel = 'vendedor' AND status = 'ativo'", vendedor.id);
        if(registro_existe('u', filtro, &vendedor) == 0) {
            printf("|              ID do vendedor nao encontrado.              |\n|   gostaria de tentar novamente? (1 para sim, 0 para nao): ");
            scanf("%d", &op);
            limpar_buffer();
            if(op == 0) {
                printf("|                   Operacao cancelada.                    |\n");
                return;
            }
        } else {
            break;
        }
    }while(op);

    printf("|   Deseja realmente arquivar o vendedor %s? (1 para sim, 0 para nao): ", vendedor.nome);
    scanf("%d", &op);
    limpar_buffer();

    if(op == 1) {
        if(excluir('u', vendedor.id)){
            printf("|               Erro ao arquivar o vendedor.               |\n");
        } else {
            printf("|             vendedor arquivado com sucesso.              |\n");
        }
    } else {
        printf("|                   Operacao cancelada.                    |\n");
    }
}

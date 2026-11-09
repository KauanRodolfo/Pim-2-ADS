/* ==========================================================
gestor.c
Tela do gestor. Os menus e submenus estao prontos; a maioria
das opcoes ainda nao faz nada (as funcoes serao feitas depois).
========================================================== */
#include "gestor.h"
#include "ranking.h"
#include "funcoes.h"
#include "funcoes_db.h"
#include <stdio.h>
#include <string.h> // Ajuste temporario - Kauan: necessario para utilizar strcpy()

/* ==========================================================
Menu principal do gestor
========================================================== */

void menu_gestor(Usuario u) {
    int op;

    do {
        printf("\n=== MENU GESTOR ===\n");
        printf("1 - Gerenciar Vendedores\n");
        printf("2 - Gerenciar Contratos\n");
        printf("3 - Ranking da Equipe\n");
        printf("0 - Sair da conta\n");

        scanf("%d", &op);

        switch (op) {
            case 1:
                gerenciar_vendedores();
                break;

            case 2:
                gerenciar_contratos(u);
                break;

            case 3:
                ranking_equipe_gestor(u);
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
Gerenciamento de vendedores
========================================================== */

void gerenciar_vendedores() {
    int op;

    do {
        printf("\n=== GERENCIAR VENDEDORES ===\n");
        printf("1 - Listar vendedores\n");
        printf("2 - Cadastrar vendedor\n");
        printf("3 - Atualizar vendedor\n");
        printf("4 - Arquivar vendedor\n");
        printf("0 - Voltar\n");

        scanf("%d", &op);

        switch (op) {
            case 1:
                consultar('u', "nivel = 'vendedor'");
                break;

            case 2:
                cadastro_vendedor();
                break;

            case 3:
                atualizar_vendedor();
                break;

            case 4:
                //excluir();
                arquivar_vendedor();
                break;

            case 0:
                printf("Voltando ao menu do gestor...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (op != 0);
}

/* ==========================================================
Analise de contratos (aprovar / reprovar)
========================================================== */

/* Lista os contratos em analise da equipe do gestor, o gestor escolhe
   um e decide se aprova ou reprova (com motivo opcional). */
void avaliar_contrato(Usuario u) {
    sqlite3 *db;
    char *erro = NULL;
    char sql[1024];
    char filtro[256];
    char busca[300];
    char motivo[TAM_TEXTO + 2];
    int id, op;
    Contrato contrato;

    snprintf(filtro, sizeof(filtro),
             "status = 'em_analise' AND vendedor_id IN "
             "(SELECT id FROM Usuarios WHERE equipe_id = %d)", u.equipe_id);

    printf("Contratos em analise da sua equipe:\n");
    consultar('t', filtro);

    printf("Entre com o ID do contrato: ");
    do{
        scanf("%d", &id);
        limpar_buffer();
        snprintf(busca, sizeof(busca), "id = %d AND %s", id, filtro);
        if(registro_existe('t', busca, &contrato) == 0) {
            printf("ID do contrato nao encontrado. \n gostaria de tentar novamente? (1 para sim, 0 para nao): ");
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

    printf("\nContrato %d | cliente: %d | vendedor: %d | valor: %.2f | data: %s | status: %s\n",
           contrato.id, contrato.cliente_id, contrato.usuario_id,
           contrato.valor_credito, contrato.data_contrato, contrato.status);
    snprintf(busca, sizeof(busca), "id = %d", contrato.cliente_id);
    consultar('c', busca);
    printf("1 - Aprovar\n2 - Reprovar\n0 - Cancelar\n");
    scanf("%d", &op);
    limpar_buffer();

    if (op == 1) {
        snprintf(sql, sizeof(sql),
                 "UPDATE Contratos SET status = 'aprovado', avaliado_por = %d, "
                 "data_avaliacao = datetime('now','localtime') "
                 "WHERE id = %d;", u.id, id);
    } else if (op == 2) {
        printf("Deseja deixar um motivo? (1 para sim, 0 para nao): ");
        scanf("%d", &op);
        limpar_buffer();

        if (op == 1) {
            printf("Motivo da reprovacao: ");
            fgets(motivo, sizeof(motivo), stdin);
            tratar_texto(motivo);
            snprintf(sql, sizeof(sql),
                     "UPDATE Contratos SET status = 'reprovado', avaliado_por = %d, "
                     "data_avaliacao = datetime('now','localtime'), motivo_reprovacao = '%s' "
                     "WHERE id = %d;", u.id, motivo, id);
        } else {
            snprintf(sql, sizeof(sql),
                     "UPDATE Contratos SET status = 'reprovado', avaliado_por = %d, "
                     "data_avaliacao = datetime('now','localtime') "
                     "WHERE id = %d;", u.id, id);
        }
    } else {
        printf("Operacao cancelada.\n");
        return;
    }

    if (sqlite3_open("pim2.db", &db) != SQLITE_OK) {
        printf("Erro ao abrir o banco: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    if (sqlite3_exec(db, sql, NULL, NULL, &erro) != SQLITE_OK) {
        printf("Erro ao atualizar contrato: %s\n", erro);
        sqlite3_free(erro);
    } else {
        printf("Contrato atualizado com sucesso.\n");
    }

    sqlite3_close(db);
}

void gerenciar_contratos(Usuario u) {
    int op;

    do {
        printf("\n=== GERENCIAR CONTRATOS ===\n");
        printf("1 - Listar contratos\n");
        printf("2 - Aprovar ou reprovar contrato\n");
        printf("3 - Consultar contratos da equipe\n");
        printf("0 - Voltar\n");

        scanf("%d", &op);

        switch (op) {
            case 1:
                consultar('t', NULL);//lista todos os tipos de contratos da propria equipe
                break;

            case 2:
                avaliar_contrato(u);
                break;

            case 3:
                consultar('t', NULL);
                break;

            case 0:
                printf("Voltando ao menu do gestor...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (op != 0);
}

/* ==========================================================
Ranking da equipe
========================================================== */

/* Mostra a comissao (5% dos contratos aprovados) de cada vendedor da equipe */
void comissao_equipe(int equipe_id) {
    sqlite3 *db;
    char *erro = NULL;
    char sql[512];
    int registros = 0;

    snprintf(sql, sizeof(sql),
        "SELECT u.id, u.nome, COUNT(c.id) AS contratos, "
        "SUM(c.valor_credito) AS total_vendido, "
        "SUM(c.valor_credito) * 0.05 AS comissao "
        "FROM Usuarios u "
        "JOIN Contratos c ON u.id = c.vendedor_id "
        "WHERE u.nivel = 'vendedor' AND u.equipe_id = %d AND c.status = 'aprovado' "
        "GROUP BY u.id "
        "ORDER BY comissao DESC;", equipe_id);

    if (sqlite3_open("pim2.db", &db) != SQLITE_OK) {
        printf("Erro ao abrir o banco: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("\n=== COMISSOES DA EQUIPE (5%%) ===\n");

    if (sqlite3_exec(db, sql, mostrar_linha, &registros, &erro) != SQLITE_OK) {
        printf("Erro ao consultar comissoes: %s\n", erro);
        sqlite3_free(erro);
    } else if (registros == 0) {
        printf("Nenhum contrato aprovado na equipe ainda.\n");
    }

    sqlite3_close(db);
}

void ranking_equipe_gestor(Usuario u) {
    int op;
    

    do {
        printf("\n=== RANKING DA EQUIPE ===\n");
        printf("1 - Ver ranking da equipe\n");
        printf("2 - Ver comissoes dos vendedores\n");
        printf("0 - Voltar\n");

        scanf("%d", &op);

        switch (op) {
            case 1:
                ranking_equipe(u.equipe_id);
                break;

            case 2:
                comissao_equipe(u.equipe_id);
                break;

            case 0:
                printf("Voltando ao menu do gestor...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (op != 0);
}

/* ==========================================================
Cadastro de vendedores
========================================================== */

int cadastro_vendedor(){
    Equipe equipe;
    char filtro[TAM];
    Usuario vendedor;
    char op, confirmar;
    int i,rc;

    limpar_buffer();

    printf("Nome do vendedor: ");
    fgets(vendedor.nome, sizeof(vendedor.nome), stdin);

    printf("\nNome de usuario do vendedor: ");
    fgets(vendedor.username, sizeof(vendedor.username), stdin);

    printf("\nEmail do vendedor: ");
    fgets(vendedor.email, sizeof(vendedor.email), stdin);

    printf("\nCPF do vendedor: ");
    fgets(vendedor.cpf, sizeof(vendedor.cpf), stdin);

    printf("\nA senha será os 5 ultimos numeros do CPF");
    for (i = 6; i < 11; i++) {
        vendedor.senha[i-6] = vendedor.cpf[i];
    }

    vendedor.senha[5] = '\0';
    strcpy(vendedor.status, "ativo");
    strcpy(vendedor.nivel, "vendedor");

    printf("\nGostaria de atribuir o vendedor a uma equipe? Sim ou Não? (s ou n)");// vendedor deve ser atribuido a equipe do gestor
    scanf(" %c", &op);

    switch(op){
        case 's':
            consultar('e', "status = 'ativa'");
            printf("Entre com o ID da equipe: ");
            do {
                scanf("%d", &vendedor.equipe_id);
                limpar_buffer();
                snprintf(filtro, sizeof(filtro), "id = %d AND status = 'ativa'", vendedor.equipe_id);
                if(registro_existe('e', filtro, &equipe) == 0) {
                    printf("ID da equipe nao encontrado. \n gostaria de tentar novamente? (1 para sim, 0 para nao): ");
                    scanf("%d", &op);
                    limpar_buffer();
                    if(op == 0) {
                        printf("Operacao cancelada.\n");
                        return;
                    }
                } else {
                    printf("Equipe selecionada: %s\n", equipe.nome);
                    break;
                }
            }while(op);
            break;
        case 'n':
            vendedor.equipe_id = 0;  // Ajuste temporario - Kauan: equipe_id e int; 0 representa ausencia de equipe
            break;
        default:
            printf("Opcao invalida.\n");
            return 1;
    }
    
    

    printf("\n=== CONFIRMAR DADOS ===\n");
    printf("Nome: %s", vendedor.nome);
    printf("Nome de usuario: %s", vendedor.username);
    printf("Email: %s", vendedor.email);
    printf("CPF: %s", vendedor.cpf);
    printf("Equipe ID: %d\n", vendedor.equipe_id);

    printf("\nConfirmar cadastro? (s/n): ");
    scanf(" %c", &confirmar);

    if (confirmar != 's' && confirmar != 'S') {
        printf("\nCadastro cancelado.\n");
        return 1;
    }

    rc = salvar('u', NULL, &vendedor, NULL, NULL, NULL);
    if (!rc) {
        printf("\nVendedor cadastrado com sucesso");
        return 0;
    }

    return 1;
}
void atualizar_vendedor() {
    Usuario vendedor;
    char filtro[TAM];
    int op;
    printf("Selecione o vendedor a ser atualizado:\n");
    snprintf(filtro, sizeof(filtro), "nivel = 'vendedor'");
    consultar('u', filtro);
    printf("Entre com o ID do vendedor a ser atualizado: ");

    do{
        scanf("%d", &vendedor.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND status = 'ativo'", vendedor.id);
        if(registro_existe('u', filtro, &vendedor) == 0) {
            printf("ID do vendedor nao encontrado. \n gostaria de tentar novamente? (1 para sim, 0 para nao): ");
            scanf("%d", &op);
            limpar_buffer();
            if(op == 0) {
                printf("Operacao cancelada.\n");
                return;
            }
        }else{
        break;
        }
    }while(op);

    printf("Deseja realmente atualizar o vendedor %s? (1 para sim, 0 para nao): ", vendedor.nome);
    scanf("%d", &op);
    limpar_buffer();

    if(op == 1) {
        if(editar('u', vendedor.id) == 0) {
            printf("vendedor atualizado com sucesso.\n");
        } else {
            printf("Erro ao atualizar o vendedor.\n");
        }
    } else {
        printf("Operacao cancelada.\n");
    }

}
void arquivar_vendedor() {
    Usuario vendedor;
    char filtro[TAM];
    int op;
    printf("Selecione o vendedor a ser arquivado:\n");
    consultar('u', NULL);
    printf("Entre com o ID do vendedor a ser arquivado: ");

    do{
        scanf("%d", &vendedor.id);
        limpar_buffer();
        snprintf(filtro, sizeof(filtro), "id = %d AND status = 'ativo'", vendedor.id);
        if(registro_existe('u', filtro, &vendedor) == 0) {
            printf("ID do vendedor nao encontrado. \n gostaria de tentar novamente? (1 para sim, 0 para nao): ");
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

    printf("Deseja realmente arquivar o vendedor %s? (1 para sim, 0 para nao): ", vendedor.nome);
    scanf("%d", &op);
    limpar_buffer();

    if(op == 1) {
        if(excluir('u', vendedor.id)){
            printf("Erro ao arquivar o vendedor.\n");
        } else {
            printf("vendedor arquivado com sucesso.\n");
        }
    } else {
        printf("Operacao cancelada.\n");
    }
}

/* Cadastra uma competicao interna para a equipe do gestor */
void competicao_equipe(Usuario gestor) {
    Competicao competicao;
    int op;
    char data[TAM_DATA + 2];   /* variavel temporaria */

    if (gestor.equipe_id == 0) {
        printf("Voce nao esta vinculado a nenhuma equipe.\n");
        return;
    }

    printf("\n=== CADASTRAR COMPETICAO DA EQUIPE ===\n");

    printf("Nome da competicao: ");
    fgets(competicao.nome, sizeof(competicao.nome), stdin);

    printf("Data de inicio (dd/mm/aaaa): ");
    fgets(data, sizeof(data), stdin);
    sprintf(competicao.data_inicio, "%.4s-%.2s-%.2s\n", data + 6, data + 3, data);

    printf("Data de fim (dd/mm/aaaa): ");
    fgets(data, sizeof(data), stdin);
    sprintf(competicao.data_fim, "%.4s-%.2s-%.2s\n", data + 6, data + 3, data);

    strcpy(competicao.tipo, "interna");
    competicao.equipe_id = gestor.equipe_id;
    competicao.criada_por = gestor.id;

    printf("Deseja cadastrar a competicao? (1 para sim, 0 para nao): ");
    scanf("%d", &op);
    limpar_buffer();
    if (op != 1) {
        printf("Operacao cancelada.\n");
        return;
    }

    if (salvar('o', NULL, NULL, NULL, NULL, &competicao) == 0) {
        printf("Competicao cadastrada com sucesso.\n");
    } else {
        printf("Erro ao cadastrar competicao.\n");
    }
}

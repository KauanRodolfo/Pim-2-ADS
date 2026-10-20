# Rankcom — Product Backlog (proposta para validação)

**Organização:** DMcom Administradora de Consórcios  
**Produto:** Rankcom  
**Objetivo do MVP:** controlar clientes e contratos, validar vendas, calcular comissões e apresentar rankings comerciais.  
**Situação:** proposta inicial; Em validação pela equipe.

> **Legenda:** `*` identifica decisão pendente de validação ou refinamento. A prioridade é uma sugestão para discussão, não uma Sprint já comprometida.

## Product Backlog — visão geral

| ID | História de usuário (resumo) | Prioridade sugerida | Observações |
|---|---|---|---|
| HU01 | Acessar o Rankcom conforme meu perfil | Alta | Admin, Gestor e Funcionário |
| HU02 | Cadastrar e consultar clientes | Alta | Dados mínimos e acesso conforme perfil |
| HU03 | Cadastrar funcionários e associá-los a equipes | Alta | Necessário para autoria, aprovação e rankings |
| HU04 | Registrar contratos vinculados a clientes | Alta | Contrato começa pendente de análise |
| HU05 | Aprovar ou reprovar contratos da equipe | Alta | Reprovação exige justificativa; permite reenvio |
| HU06* | Calcular comissão fixa de contratos aprovados | Alta | Percentual único; 5% **provisório**; base de cálculo pendente |
| HU07* | Acompanhar ranking geral e por equipe | Alta | Total de comissões aprovadas no ciclo; desempate e visibilidade pendentes |
| HU08 | Abrir e encerrar ciclo de apuração | Média | Um ciclo compartilhado pelos quatro rankings |

## Histórias de usuário e critérios de aceitação

### HU01 — Autenticação e acesso

**Como** usuário do Rankcom, **quero** entrar com minhas credenciais, **para** acessar apenas as funções autorizadas para meu perfil.

**Critérios de aceitação:**
- O usuário informa e-mail e senha para autenticação.
- O sistema identifica os perfis Administrador, Gestor e Funcionário.
- Cada perfil acessa somente funcionalidades permitidas.

### HU02 — Cadastro de clientes

**Como** Funcionário, **quero** cadastrar e consultar clientes, **para** vinculá-los aos contratos comerciais.

**Critérios de aceitação:**
- O cadastro permite registrar nome, CPF/CNPJ e contato.
- O sistema valida os campos obrigatórios definidos para o protótipo.
- Um contrato pode ser vinculado a um cliente já cadastrado.

### HU03 — Cadastro de funcionários e equipes

**Como** Administrador, **quero** manter os dados dos funcionários e suas equipes, **para** organizar permissões e classificações comerciais.

**Critérios de aceitação:**
- É possível cadastrar um funcionário com perfil e equipe correspondentes.
- O vínculo de cada vendedor a uma das três equipes pode ser consultado.
- Um gestor acompanha somente os contratos e a classificação de sua equipe, conforme permissões definidas.

### HU04 — Registro de contratos

**Como** Funcionário, **quero** registrar contratos vinculados aos clientes, **para** encaminhar minhas vendas à análise do gestor.

**Critérios de aceitação:**
- O contrato é vinculado a um cliente e a um vendedor.
- Ao ser registrado, recebe o status **Pendente de análise**.
- Contratos pendentes não contabilizam comissão nem pontuação no ranking.

### HU05 — Análise e aprovação de contratos

**Como** Gestor, **quero** aprovar ou reprovar os contratos dos vendedores da minha equipe, **para** validar as vendas consideradas pelo sistema.

**Critérios de aceitação:**
- O gestor consulta os contratos pendentes da sua equipe.
- Pode aprovar o contrato ou reprová-lo com justificativa.
- Um contrato reprovado pode ser corrigido pelo vendedor e reenviado para análise.
- Somente a aprovação torna o contrato elegível ao cálculo e à contabilização de comissão.

### HU06* — Comissão com percentual único

**Como** Funcionário, **quero** que a comissão de um contrato aprovado seja calculada automaticamente, **para** acompanhar meus resultados de forma padronizada.

**Critérios de aceitação:**
- O mesmo percentual fixo é aplicado a todos os vendedores.
- Para os testes iniciais, **5% é apenas uma hipótese provisória**, alterável após decisão do grupo.
- O cálculo e a contabilização ocorrem somente para contratos aprovados.
- **Pendente:** definir a base financeira do cálculo antes de finalizar a implementação e os testes.

### HU07* — Rankings comerciais

**Como** Gestor, **quero** visualizar a classificação geral e a da minha equipe, **para** acompanhar o desempenho comercial no ciclo vigente.

**Critérios de aceitação:**
- O Rankcom apresenta um ranking geral dos 21 vendedores e um ranking para cada uma das três equipes.
- A classificação utiliza o total de comissões válidas acumuladas no ciclo.
- Os rankings destacam os três primeiros colocados e são recalculados quando uma venda aprovada altera os valores.
- **Pendente:** decidir desempates e exatamente quais perfis visualizam cada ranking.

### HU08 — Ciclo de apuração

**Como** Administrador, **quero** abrir e encerrar o ciclo de apuração, **para** delimitar o período considerado nos rankings comerciais.

**Critérios de aceitação:**
- Um único ciclo é compartilhado pelo ranking geral e pelos três rankings das equipes.
- Apenas o Administrador pode abrir ou encerrar o ciclo.
- Os rankings consideram as comissões válidas atribuídas ao ciclo vigente.
- O encerramento consolida os resultados do ciclo; detalhes de histórico serão refinados nos requisitos.

## Relação provisória com requisitos funcionais

- RF01 — Autenticação e permissões (HU01)
- RF02 — Clientes (HU02)
- RF03 — Funcionários e equipes (HU03)
- RF04 — Registro, análise e aprovação de contratos (HU04 + HU05)
- RF05 — Cálculo de comissões (HU06*)
- RF06 — Rankings (HU07*)
- RF07 — Ciclos de apuração (HU08)

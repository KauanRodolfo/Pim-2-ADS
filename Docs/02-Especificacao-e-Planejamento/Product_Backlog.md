 # Rankcom — Product Backlog

**Organização:** DMcom Administradora de Consórcios  
**Produto:** Rankcom  
**Documento relacionado:** [`requisitos.md`](./Requisitos.md)

> **Legenda:** `*` indica detalhe pendente de decisão. **Prioridade** indica importância do item para o produto; não determina sua Sprint.

## 1. Product Backlog — visão geral

| História | Necessidade | RF relacionado | Prioridade | Observação |
|---|---|---|---|---|
| HU01 | Acessar funcionalidades de acordo com o perfil | RF01 | Alta | Administrador, Gestor e Vendedor |
| HU02 | Gerenciar clientes | RF02 | Alta | Cadastro e consulta pelo Vendedor; cliente selecionado no registro do contrato |
| HU03 | Gerenciar vendedores | RF02 | Alta | Cadastro, edição e arquivamento pelo Gestor |
| HU04 | Registrar contratos para análise | RF03 | Alta | Início como pendente |
| HU05 | Aprovar ou reprovar contratos | RF03 | Alta | Reprovação definitiva, sem reenvio |
| HU06 | Calcular comissões padronizadas | RF04* | Alta | 5% fixos; base de cálculo pendente |
| HU07* | Consultar rankings comerciais | RF05* | Alta | Vencedor único; visibilidade pendente |
| HU08 | Abrir e encerrar ciclos de apuração | RF06 | Média | Um ciclo para os quatro rankings |
| HU09 | Gerenciar equipes e gestores | RF02 | Alta | Equipes, promoção de gestores e movimentação de usuários pelo Administrador |

## 2. Histórias de usuário e critérios de aceitação

### HU01 — Autenticação e acesso

**Como** usuário do Rankcom, **quero** entrar com e-mail e senha, **para** acessar somente as funções disponíveis ao meu perfil.

**Critérios de aceitação:**
- Credenciais são solicitadas e verificadas.
- O sistema reconhece os perfis Administrador, Gestor e Vendedor.
- Funções sem autorização não podem ser executadas pelo perfil autenticado.
- Tentativas de acesso são registradas em log, sem armazenar senhas no registro.

### HU02 — Gerenciamento de clientes

**Como** Vendedor, **quero** cadastrar e consultar clientes, **para** selecioná-los ao registrar contratos.

**Critérios de aceitação:**
- Somente o Vendedor pode cadastrar clientes.
- O cadastro contempla nome, CPF/CNPJ e contato.
- Campos obrigatórios são validados.
- O Vendedor pode consultar clientes cadastrados e selecioná-los ao registrar contratos.

### HU03 — Gerenciamento de vendedores

**Como** Gestor, **quero** cadastrar, editar e arquivar vendedores, **para** manter atualizada a equipe comercial.

**Critérios de aceitação:**
- Somente o Gestor pode cadastrar, editar e arquivar vendedores.
- O cadastro identifica o vendedor e os dados necessários para seu acesso ao sistema.
- O Administrador pode mover vendedores entre equipes, conforme a história de gerenciamento de equipes.

### HU04 — Registro de contratos

**Como** Vendedor, **quero** registrar um contrato para um cliente, **para** encaminhar a venda ao gestor.

**Critérios de aceitação:**
- O contrato é associado a um cliente e ao vendedor responsável.
- O registro inicia com status **Pendente de análise**.
- Contratos pendentes não somam comissões válidas nos rankings.

### HU05 — Aprovação e reprovação de contratos

**Como** Gestor, **quero** analisar contratos da minha equipe, **para** validar as vendas que poderão gerar comissão.

**Critérios de aceitação:**
- O gestor consulta os contratos pendentes da sua equipe.
- O gestor pode aprovar ou reprovar um contrato, sem necessidade de justificativa.
- A reprovação encerra definitivamente o processo daquele registro; não há opção de correção ou reenvio.
- Contratos reprovados permanecem identificados no histórico para consulta, sem gerar comissão.
- Somente contratos aprovados ficam aptos à contabilização de comissão.
- Aprovações e reprovações são registradas em log com data e hora.

### HU06 — Cálculo de comissões

**Como** Vendedor, **quero** que contratos aprovados tenham sua comissão calculada automaticamente, **para** acompanhar meu resultado de forma padronizada.

**Critérios de aceitação:**
- O sistema aplica **5% fixos** para todos os vendedores.
- O cálculo só é contabilizado após a aprovação do contrato.
- A comissão calculada pode ser utilizada pelo ranking.
- **Pendente:** determinar a base financeira exata do cálculo; o percentual de **5% está definido**.

### HU07 — Rankings comerciais

**Como** Gestor, **quero** consultar a classificação geral e a de minha equipe, **para** acompanhar os resultados das vendas aprovadas no ciclo.

**Critérios de aceitação:**
- São apresentados um ranking geral dos 21 vendedores e três rankings de equipe.
- A ordem considera o total de comissões válidas no ciclo, do maior para o menor.
- Apenas o **primeiro colocado** é destacado como vencedor em cada ranking (geral e por equipe).
- A classificação é atualizada quando novas aprovações alteram os resultados.
- Em empate no total de comissões válidas, vence quem registrou **mais cedo uma venda válida no ciclo**, conforme a data/hora do registro de contrato posteriormente aprovado.
- Se não houver vendas válidas, não é destacado um vencedor.

### HU08 — Ciclos de apuração

**Como** Administrador, **quero** abrir e encerrar um ciclo de apuração, **para** delimitar as comissões consideradas nos rankings.

**Critérios de aceitação:**
- Apenas o Administrador pode abrir ou encerrar ciclos.
- Ranking geral e rankings por equipe compartilham um único ciclo vigente.
- O encerramento consolida a classificação do período.

### HU09 — Gerenciamento de equipes e gestores

**Como** Administrador, **quero** gerenciar equipes e gestores, **para** organizar a estrutura das equipes comerciais.

**Critérios de aceitação:**
- O Administrador pode criar, editar e arquivar equipes.
- O Administrador pode promover um usuário a Gestor ou rebaixar um Gestor.
- O Administrador pode definir o Gestor responsável por uma equipe.
- O Administrador pode mover usuários entre equipes.

---

## 3. Rastreabilidade resumida

| História | RF relacionado | RN relacionada |
|---|---|---|
| HU01 | RF01, RF07 | — |
| HU02 | RF02 | — |
| HU03 | RF02 | — |
| HU04 | RF03 | — |
| HU05 | RF03, RF07 | RN01 |
| HU06 | RF04* | RN01 |
| HU07* | RF05* | RN01 |
| HU08 | RF06 | — |
| HU09 | RF02 | — |


Os requisitos não funcionais aplicam-se às histórias conforme sua pertinência e estão detalhados em `requisitos.md`.

---

## 4. Planejamento e Kanban

O planejamento semanal e o acompanhamento das atividades são realizados no **Kanban do GitHub** e nos registros das reuniões da equipe.

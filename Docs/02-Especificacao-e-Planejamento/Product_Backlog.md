# Rankcom — Product Backlog e Decomposição em Tarefas

**Organização:** DMcom Administradora de Consórcios  
**Produto:** Rankcom  
**Versão:** 0.4 — decisões de 22/10/2026  
**Situação:** Em validação pela equipe  
**Documento relacionado:** [`requisitos.md`](./requisitos.md)

> **Legenda:** `*` indica detalhe pendente de decisão. **Prioridade** indica importância do item para o produto; não determina sua Sprint. Tarefas aqui são propostas, **não registros de trabalho já concluído**.

## 1. Product Backlog — visão geral

| História | Necessidade | RF relacionado | Prioridade | Observação |
|---|---|---|---|---|
| HU01 | Acessar funcionalidades de acordo com o perfil | RF01 | Alta | Administrador, Gestor e Funcionário |
| HU02 | Cadastrar e consultar clientes | RF02 | Alta | Dados vinculados aos contratos |
| HU03 | Cadastrar funcionários e associar equipes | RF03 | Alta | Equipes e perfis de acesso |
| HU04 | Registrar contratos para análise | RF04 | Alta | Início como pendente |
| HU05 | Aprovar ou reprovar contratos | RF04 | Alta | Reprovação definitiva, sem reenvio |
| HU06 | Calcular comissões padronizadas | RF05* | Alta | 5% fixos; base de cálculo pendente |
| HU07* | Consultar rankings comerciais | RF06* | Alta | Vencedor único; visibilidade pendente |
| HU08 | Abrir e encerrar ciclos de apuração | RF07 | Média | Um ciclo para os quatro rankings |

## 2. Histórias de usuário e critérios de aceitação

### HU01 — Autenticação e acesso

**Como** usuário do Rankcom, **quero** entrar com e-mail e senha, **para** acessar somente as funções disponíveis ao meu perfil.

**Critérios de aceitação:**
- Credenciais são solicitadas e verificadas.
- O sistema reconhece os perfis Administrador, Gestor e Funcionário.
- Funções sem autorização não podem ser executadas pelo perfil autenticado.

### HU02 — Gerenciamento de clientes

**Como** Funcionário, **quero** cadastrar e consultar clientes, **para** associá-los aos contratos.

**Critérios de aceitação:**
- O cadastro contempla nome, CPF/CNPJ e contato.
- Campos obrigatórios são validados.
- É possível consultar clientes cadastrados e selecioná-los ao registrar contratos.

### HU03 — Gerenciamento de funcionários e equipes

**Como** Administrador, **quero** cadastrar funcionários e vinculá-los às equipes comerciais, **para** organizar os perfis e as classificações.

**Critérios de aceitação:**
- O cadastro identifica funcionário, cargo, perfil e equipe, quando aplicável.
- É possível consultar a equipe à qual cada vendedor pertence.
- Os vínculos utilizados nos contratos e rankings são mantidos de maneira consistente.

### HU04 — Registro de contratos

**Como** Funcionário, **quero** registrar um contrato para um cliente, **para** encaminhar a venda ao gestor.

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

### HU06 — Cálculo de comissões

**Como** Funcionário, **quero** que contratos aprovados tenham sua comissão calculada automaticamente, **para** acompanhar meu resultado de forma padronizada.

**Critérios de aceitação:**
- O sistema aplica **5% fixos** para todos os vendedores.
- O cálculo só é contabilizado após a aprovação do contrato.
- A comissão calculada pode ser utilizada pelo ranking.
- **Pendente:** determinar a base financeira exata do cálculo; o percentual de **5% está definido**.

### HU07* — Rankings comerciais

**Como** Gestor, **quero** consultar a classificação geral e a de minha equipe, **para** acompanhar os resultados das vendas aprovadas no ciclo.

**Critérios de aceitação:**
- São apresentados um ranking geral dos 21 vendedores e três rankings de equipe.
- A ordem considera o total de comissões válidas no ciclo, do maior para o menor.
- Apenas o **primeiro colocado** é destacado como vencedor em cada ranking (geral e por equipe).
- A classificação é atualizada quando novas aprovações alteram os resultados.
- Em empate no total de comissões válidas, vence quem registrou **mais cedo uma venda válida no ciclo**, conforme a data/hora do registro de contrato posteriormente aprovado.
- Se não houver vendas válidas, não é destacado um vencedor.
- **Pendente:** níveis de consulta dos rankings por perfil.

### HU08 — Ciclos de apuração

**Como** Administrador, **quero** abrir e encerrar um ciclo de apuração, **para** delimitar as comissões consideradas nos rankings.

**Critérios de aceitação:**
- Apenas o Administrador pode abrir ou encerrar ciclos.
- Ranking geral e rankings por equipe compartilham um único ciclo vigente.
- O encerramento consolida a classificação do período.

---

## 3. Decomposição dos requisitos em tarefas executáveis

A numeração **T01–T26 é proposta** para rastreabilidade. Cada linha representa uma ação verificável e poderá virar um item individual no Kanban depois do planejamento da equipe.

| ID | Requisito | Tarefa | Descrição breve | Prioridade |
|---|---|---|---|---|
| T01 | RF01 / HU01 | Autenticar usuário | Validar e-mail e senha e iniciar sessão. | Alta |
| T02 | RF01 / HU01 | Controlar acesso por perfil | Restringir operações conforme Administrador, Gestor e Funcionário. | Alta |
| T03 | RF02 / HU02 | Cadastrar cliente | Registrar nome, CPF/CNPJ e contato. | Alta |
| T04 | RF02 / HU02 | Consultar cliente | Localizar e visualizar registros de clientes. | Alta |
| T05 | RF02 / HU02 | Validar dados de cliente | Verificar preenchimento e formato dos campos definidos. | Alta |
| T06 | RF03 / HU03 | Cadastrar funcionário | Registrar dados mínimos, cargo e perfil. | Alta |
| T07 | RF03 / HU03 | Associar funcionário à equipe | Vincular vendedores e gestores às equipes pertinentes. | Alta |
| T08 | RF03 / HU03 | Consultar funcionários e equipes | Listar registros e seus vínculos. | Alta |
| T09 | RF04 / HU04 | Registrar contrato | Vincular contrato a cliente e vendedor. | Alta |
| T10 | RF04 / HU04 | Definir status inicial | Registrar contrato como pendente de análise. | Alta |
| T11 | RF04 / HU05 | Consultar contratos pendentes | Exibir ao gestor solicitações da sua equipe. | Alta |
| T12 | RF04 / HU05 | Aprovar ou reprovar contrato | Registrar a decisão de aprovação ou reprovação e atualizar o status do contrato. | Alta |
| T13 | RF04 / HU05 | Encerrar contrato reprovado | Registrar a reprovação definitiva, preservar o histórico e impedir reenvio do mesmo registro. | Alta |
| T14* | RF05 / HU06 | Aplicar comissão de 5% | Calcular 5% sobre a base financeira definida para contrato aprovado. | Alta |
| T15 | RF05 / HU06 | Contabilizar comissão válida | Incorporar somente comissões de contratos aprovados. | Alta |
| T16 | RF06 / HU07 | Gerar ranking geral | Ordenar vendedores pela comissão válida do ciclo. | Alta |
| T17 | RF06 / HU07 | Gerar rankings por equipe | Ordenar vendedores em cada equipe. | Alta |
| T18 | RF06 / HU07 | Identificar vencedor e desempatar | Destacar só o primeiro de cada ranking; desempatar pela primeira venda válida e atualizar após aprovações. | Alta |
| T19 | RF07 / HU08 | Abrir ciclo de apuração | Permitir ao Administrador iniciar ciclo compartilhado. | Média |
| T20 | RF07 / HU08 | Encerrar ciclo de apuração | Encerrar período e consolidar resultados. | Média |
| T21 | RF07 / HU08 | Controlar ciclo vigente | Vincular resultados e consultas ao ciclo correto. | Média |
| T22 | RNF01 | Adequar núcleo do protótipo à linguagem C | Implementar as rotinas prioritárias em C. | Alta |
| T23 | RNF02 | Integrar persistência SQLite | Gravar e recuperar registros necessários entre execuções. | Alta |
| T24 | RNF03 | Registrar logs de acesso e decisões | Verificar acesso por perfil e registrar eventos relevantes com data/hora, usando credenciais fictícias no protótipo. | Alta |
| T25 | RNF04 | Restringir dados pessoais | Impedir consulta ou uso de dados pessoais por perfis não autorizados. | Alta |
| T26 | RNF05 | Validar menus e entradas | Implementar menus CLI, mensagens claras e tratamento de entradas inválidas. | Alta |

> **Observação sobre tarefas RNF:** T22–T26 são transversais e poderão gerar subtarefas durante a implementação. Não representam novas funcionalidades independentes.

---

## 4. Rastreabilidade resumida

| História | RF | Tarefas | RN relacionada |
|---|---|---|---|
| HU01 | RF01 | T01–T02 | — |
| HU02 | RF02 | T03–T05 | — |
| HU03 | RF03 | T06–T08 | — |
| HU04 | RF04 | T09–T10 | — |
| HU05 | RF04 | T11–T13 | RN01 |
| HU06 | RF05* | T14*–T15 | RN01 |
| HU07* | RF06* | T16–T18 | RN01 |
| HU08 | RF07 | T19–T21 | — |

Os RNFs são relacionados às tarefas T22–T26 na tabela anterior e podem afetar várias histórias simultaneamente.

---

## 5. Decisões confirmadas, pendências e limites do MVP

**Decisões confirmadas pelo grupo (22/10/2026):**
- **RN01 / RF05 / HU06:** comissão única e fixa de **5%**, calculada somente após aprovação do contrato.
- **RF04 / HU05 / T13:** reprovação **definitiva**, sem correção ou reenvio do mesmo contrato; histórico mantido.
- **RF06 / HU07 / T18:** um vencedor no geral e um por equipe; em empate, prevalece a venda válida registrada primeiro no ciclo.
- **Premiações:** podem existir futuramente como consequência dos resultados, mas **não há módulo de premiação no MVP**.
- **RNF03 / T24:** autenticação por credenciais fictícias, restrição por perfil e registros de log; segurança de credenciais em produção fora do escopo do protótipo.

**Ainda pendente de validação:**
- **RF05* / T14*:** base monetária sobre a qual serão aplicados os 5%.
- **RF06* / HU07*:** quais perfis podem consultar cada ranking.
- **RNF03:** selecionar os eventos mínimos do log; **não registrar senhas nos logs**.

**Fora do MVP:** cadastro e gestão de premiações, estornos por cancelamento posterior à aprovação, correção/reenvio de contratos rejeitados e implantação corporativa multiusuário. A infraestrutura de rede é uma proposta conceitual, não um ambiente instalado.

---

## 6. Planejamento e Kanban — próxima revisão

O grupo realiza reuniões aos sábados, nas quais revisa entregas, pendências e define o trabalho da semana seguinte.

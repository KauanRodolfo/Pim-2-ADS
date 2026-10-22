# Rankcom — Requisitos do Sistema

**Organização:** DMcom Administradora de Consórcios  
**Produto:** Rankcom  
**Versão:** 0.4 — decisões de 22/10/2026
**Situação:** Em validação pela equipe  
**Documentos relacionados:** [`product_backlog.md`](./product_backlog.md)

> **Legenda:** `*` indica detalhe ainda não definido pelo grupo. Não significa que a funcionalidade esteja excluída.

## 1. Regras de Negócio (RN)

Regras de negócio são políticas da **DMcom**: orientam o processo mesmo que ele não seja informatizado.

### RN01 — Comissão fixa de 5%

A DMcom adota uma **comissão fixa de 5% para todos os vendedores**, independentemente da equipe. O Rankcom calcula a comissão somente após a **aprovação do contrato pelo gestor**. Contratos pendentes ou reprovados não geram comissão válida nem pontuação nos rankings.

**Pendente técnico/financeiro:** definir com a equipe qual **valor do contrato constitui a base de cálculo**; não se presume que seja o valor total do contrato.

**Relação:** RF04, RF05 e RF06; HU05, HU06 e HU07.

---

## 2. Requisitos Funcionais (RF)

Os RFs descrevem capacidades oferecidas pelo sistema. Operações menores (como cadastrar, consultar e validar) são detalhadas nas tarefas do Product Backlog.

### RF01 — Autenticação e controle de acesso

O Rankcom deve autenticar usuários por e-mail e senha e permitir o acesso às funcionalidades conforme os perfis **Administrador**, **Gestor** e **Funcionário**.

**História vinculada:** HU01.

### RF02 — Gerenciamento de clientes

O Rankcom deve permitir que o **Funcionário** cadastre e consulte clientes, contendo nome, CPF/CNPJ e informações de contato, de modo que possam ser vinculados a contratos.

**História vinculada:** HU02.

### RF03 — Gerenciamento de funcionários e equipes

O Rankcom deve permitir que o **Administrador** cadastre e consulte funcionários e associe seus cargos, perfis de acesso e equipes comerciais.

**História vinculada:** HU03.

### RF04 — Registro e análise de contratos

O Rankcom deve permitir que o **Funcionário** cadastre contratos vinculados a clientes, cabendo ao **Gestor** aprová-los ou reprová-los. Contratos reprovados não poderão ser reenviados nem gerar comissões.

**Histórias vinculadas:** HU04 e HU05.  
**Regra relacionada:** RN01.

### RF05* — Cálculo automático de comissões

O Rankcom deve calcular automaticamente a comissão de **5%** sobre a base financeira adotada para o contrato, **exclusivamente depois da aprovação pelo Gestor**. O valor validado compõe a apuração do vendedor no ciclo correspondente.

**Pendente:** identificar precisamente a base de cálculo da comissão; o percentual **já está definido em 5%**.

**História vinculada:** HU06.  
**Regra relacionada:** RN01.

### RF06* — Rankings comerciais

O Rankcom deve apresentar **um ranking geral** dos 21 vendedores e **um ranking para cada uma das três equipes**, considerando a soma das comissões válidas de contratos aprovados no ciclo. O sistema destaca **apenas o vendedor em primeiro lugar de cada ranking**, sem pódio de segundo e terceiro lugares.

Em empate na soma das comissões, vence quem tiver realizado **primeiro uma venda válida no ciclo**, considerando a data e hora de registro da venda vinculada a contrato posteriormente aprovado (não a data da aprovação). As classificações devem refletir novas aprovações durante o ciclo.

**Pendente:** definir as permissões exatas de visualização dos rankings por perfil. Se nenhum participante possuir venda válida, não há vencedor a destacar.

**História vinculada:** HU07.  
**Regra relacionada:** RN01.

### RF07 — Gerenciamento de ciclos de apuração

O Rankcom deve permitir que o **Administrador** abra e encerre **um ciclo único de apuração**, compartilhado pelos rankings geral e por equipe, consolidando a classificação ao término do período.

**História vinculada:** HU08.

---

## 3. Requisitos Não Funcionais (RNF)

Os RNFs descrevem restrições técnicas e características de qualidade do protótipo. Não substituem os RFs nem são tarefas por si só.

### RNF01 — Linguagem de implementação

O protótipo funcional deve ser implementado em **linguagem C**, conforme a disciplina de Programação Estruturada.

### RNF02 — Persistência local de dados

O protótipo deve utilizar **SQLite** para persistir os dados necessários ao funcionamento e recuperá-los em execuções posteriores.

### RNF03 — Controle de acesso e rastreabilidade por logs

O protótipo deve **verificar as credenciais de acesso**, restringir operações conforme o perfil autenticado e **registrar em log eventos relevantes**, como tentativas de acesso e decisões de aprovação ou reprovação de contratos, com identificação do evento e momento do registro.

**Delimitação do protótipo:** não foi assumida a implementação de hash de senhas nesta etapa. Os testes devem utilizar **credenciais fictícias**, sem senhas reais; logs não substituem a proteção das credenciais. Um sistema colocado em produção exigiria armazenamento seguro de senhas e controles adicionais.

### RNF04 — Proteção de dados pessoais

O Rankcom deve restringir a visualização e o tratamento dos dados pessoais de clientes e funcionários aos perfis autorizados, respeitando os princípios aplicáveis da **LGPD**.

### RNF05 — Usabilidade e validação de entradas

O protótipo deve utilizar **interface textual (CLI)** com menus identificados, mensagens compreensíveis e validação das entradas para evitar operações inconsistentes e falhas comuns de utilização.

---

## 4. Rastreabilidade

| Requisito | História(s) | Regra(s) relacionada(s) | Tarefas vinculadas no backlog |
|---|---|---|---|
| RF01 | HU01 | — | T01–T02 |
| RF02 | HU02 | — | T03–T05 |
| RF03 | HU03 | — | T06–T08 |
| RF04 | HU04, HU05 | RN01 | T09–T13 |
| RF05* | HU06 | RN01 | T14–T15 |
| RF06* | HU07* | RN01 | T16–T18 |
| RF07 | HU08 | — | T19–T21 |
| RNF01 | HU01–HU08 | — | T22 |
| RNF02 | HU02–HU08 | — | T23 |
| RNF03 | HU01, HU05 | — | T24 |
| RNF04 | HU01–HU05 | — | T25 |
| RNF05 | HU01–HU08 | — | T26 |

---

## 5. Limites do MVP e pendências

- **Fora do MVP:** módulo de cadastro e controle de premiações; estornos e recálculos de contratos cancelados depois da aprovação; implantação multiusuário em rede. A arquitetura de rede é uma **proposta conceitual**, diferente do protótipo local.
- **A definir:** base financeira da comissão (RF05); permissões de visualização dos rankings (RF06); escopo exato dos eventos registrados em log (RNF03). O percentual de **5%**, o desempate e o vencedor único por ranking **já foram definidos**.
- **Fluxo confirmado:** vendedor registra contrato; gestor aprova ou reprova; a reprovação encerra o fluxo do registro, **sem correção ou reenvio**; **somente contratos aprovados** contam para a comissão e os rankings.
- **Premiações:** cada ranking poderá fundamentar uma premiação distinta no futuro, mas seu cadastro, concessão e acompanhamento **não serão implementados** nesta versão do Rankcom.

**Planejamento:** as tarefas T01–T26 são **propostas**, ainda não atribuídas a Sprints, responsáveis ou status de execução. A equipe fará esse planejamento sem alterar retroativamente os registros reais das reuniões semanais.

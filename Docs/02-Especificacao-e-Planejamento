# Rankcom — Requisitos do Sistema

**Organização:** DMcom Administradora de Consórcios  
**Produto:** Rankcom  
**Versão:** 0.3 — proposta para validação  
**Situação:** Em validação pela equipe  
**Documentos relacionados:** [`product_backlog.md`](./product_backlog.md)

> **Legenda:** `*` indica detalhe ainda não definido pelo grupo. Não significa que a funcionalidade esteja excluída.

## 1. Regras de Negócio (RN)

Regras de negócio são políticas da **DMcom**: orientam o processo mesmo que ele não seja informatizado.

### RN01* — Comissão com percentual único e fixo

A DMcom adota **o mesmo percentual de comissão para todos os vendedores**, independentemente da equipe. Somente contratos **aprovados pelo gestor responsável** podem gerar comissões válidas para contabilização nos rankings comerciais.

**Pendente:** definir a base financeira de incidência e o percentual definitivo. **5% é apenas um parâmetro provisório para simulações**, não uma decisão final do negócio.

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

O Rankcom deve permitir que o **Funcionário** registre contratos vinculados a clientes e os encaminhe à análise do **Gestor da equipe**. O Gestor deve poder aprovar ou reprovar cada registro; a reprovação exige justificativa e permite correção e reenvio pelo Funcionário.

**Histórias vinculadas:** HU04 e HU05.  
**Regra relacionada:** RN01.

### RF05* — Cálculo automático de comissões

O Rankcom deve calcular a comissão dos **contratos aprovados**, utilizando o **percentual único e fixo** adotado pela DMcom.

**Pendente:** percentual definitivo e base financeira do cálculo. Para exemplos e testes preliminares, pode ser utilizado o percentual **provisório de 5%**, sujeito à revisão. Sem a base definida, o cálculo não pode ser considerado especificado integralmente.

**História vinculada:** HU06.  
**Regra relacionada:** RN01.

### RF06* — Rankings comerciais

O Rankcom deve disponibilizar **um ranking geral** dos 21 vendedores e **três rankings por equipe**, classificando participantes pela soma das comissões válidas no ciclo em curso, em ordem decrescente, e destacando os três primeiros colocados. Os resultados devem refletir novas aprovações de contratos.

**Pendente:** critério de desempate e permissões exatas de consulta por perfil.

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

### RNF03* — Segurança de autenticação

O protótipo deve restringir operações conforme o perfil autenticado e **não armazenar senhas em texto puro**, adotando mecanismo de armazenamento seguro de credenciais.

**Pendente técnico:** selecionar e implementar um mecanismo apropriado para senhas em C; hash SHA-256 simples, sem proteção adicional, não é suficiente como estratégia de armazenamento de senhas.

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
| RF05* | HU06* | RN01* | T14–T15 |
| RF06* | HU07* | RN01* | T16–T18 |
| RF07 | HU08 | — | T19–T21 |
| RNF01 | HU01–HU08 | — | T22 |
| RNF02 | HU02–HU08 | — | T23 |
| RNF03* | HU01 | — | T24 |
| RNF04 | HU01–HU05 | — | T25 |
| RNF05 | HU01–HU08 | — | T26 |

> Os RNFs podem incidir sobre diversas histórias e tarefas. Os vínculos indicados são uma referência de planejamento, não uma relação obrigatória de um para um.

---

## 5. Limites do MVP e pendências

- **Fora do MVP:** módulo de cadastro e controle de premiações; estornos e recálculos de contratos cancelados depois da aprovação; implantação multiusuário em rede. A arquitetura de rede é uma **proposta conceitual**, diferente do protótipo local.
- **A definir:** percentual e base de cálculo da comissão (RN01/RF05); desempates e visibilidade dos rankings (RF06); método concreto de proteção de senhas (RNF03).
- **Fluxo confirmado:** vendedor registra contrato; gestor aprova ou reprova com justificativa; o vendedor pode corrigir e reenviar; **somente contratos aprovados** contam para a comissão e os rankings.
- **Premiações:** podem existir como prática administrativa da DMcom, mas seu gerenciamento **não será implementado** nesta versão do Rankcom.

**Planejamento:** as tarefas T01–T26 são **propostas**, ainda não atribuídas a Sprints, responsáveis ou status de execução. A equipe fará esse planejamento sem alterar retroativamente os registros reais das reuniões semanais.

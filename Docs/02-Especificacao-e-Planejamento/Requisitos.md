# Rankcom — Requisitos do Sistema

**Organização:** DMcom Administradora de Consórcios  
**Produto:** Rankcom   
**Documentos relacionados:** [`product_backlog.md`](./product_backlog.md)

> **Legenda:** `*` indica detalhe ainda não definido pelo grupo. Não significa que a funcionalidade esteja excluída.

## 1. Regras de Negócio (RN)

Regras de negócio são políticas da **DMcom**: orientam o processo mesmo que ele não seja informatizado.

### RN01 — Comissão fixa de 5%

A DMcom adota uma **comissão fixa de 5% para todos os vendedores**, independentemente da equipe. O Rankcom calcula a comissão somente após a **aprovação do contrato pelo gestor**. Contratos pendentes ou reprovados não geram comissão válida nem pontuação nos rankings.

**Pendente técnico/financeiro:** definir com a equipe qual **valor do contrato constitui a base de cálculo**; não se presume que seja o valor total do contrato.

**Relação:** RF03, RF04 e RF05; HU05, HU06 e HU07.

---

## 2. Requisitos Funcionais (RF)

Os RFs descrevem as funcionalidades oferecidas pelo sistema e se relacionam às histórias de usuário do Product Backlog.

### RF01 — Autenticação e controle de acesso

O Rankcom deve autenticar usuários por e-mail e senha e permitir o acesso às funcionalidades conforme os perfis **Administrador**, **Gestor** e **Vendedor**.

**História vinculada:** HU01.

### RF02 — Gerenciamento de clientes, vendedores e equipes

O Rankcom deve permitir que o **Vendedor** cadastre e consulte clientes vinculados aos contratos e que o **Administrador** cadastre e consulte vendedores, definindo seus cargos, perfis de acesso e equipes comerciais.

**Histórias vinculadas:** HU02 e HU03.

### RF03 — Registro e análise de contratos

O Rankcom deve permitir que o **Vendedor** cadastre contratos vinculados a clientes, cabendo ao **Gestor** aprová-los ou reprová-los. Contratos reprovados não poderão ser reenviados nem gerar comissões.

**Histórias vinculadas:** HU04 e HU05.  
**Regra relacionada:** RN01.

### RF04* — Cálculo automático de comissões

O Rankcom deve calcular automaticamente a comissão de **5%** sobre a base financeira adotada para o contrato, **exclusivamente depois da aprovação pelo Gestor**. O valor validado compõe a apuração do vendedor no ciclo correspondente.

**Pendente:** identificar precisamente a base de cálculo da comissão; o percentual **já está definido em 5%**.

**História vinculada:** HU06.  
**Regra relacionada:** RN01.

### RF05 — Rankings comerciais

O Rankcom deve apresentar **um ranking geral** dos 21 vendedores e **um ranking para cada uma das três equipes**, considerando a soma das comissões válidas de contratos aprovados no ciclo. O sistema destaca **apenas o vendedor em primeiro lugar de cada ranking**, sem pódio de segundo e terceiro lugares.

Em empate na soma das comissões, vence quem tiver realizado **primeiro uma venda válida no ciclo**, considerando a data e hora de registro da venda vinculada a contrato posteriormente aprovado (não a data da aprovação). As classificações devem refletir novas aprovações durante o ciclo.

**História vinculada:** HU07.  
**Regra relacionada:** RN01.

### RF06 — Gerenciamento de ciclos de apuração

O Rankcom deve permitir que o **Administrador** abra e encerre **um ciclo único de apuração**, compartilhado pelos rankings geral e por equipe, consolidando a classificação ao término do período.

**História vinculada:** HU08.

### RF07 — Controle de acesso e registro de logs

O Rankcom deve verificar as permissões do usuário autenticado e registrar em **log** eventos relevantes, como tentativas de acesso e decisões de aprovação ou reprovação de contratos, com identificação do evento e data/hora.

**Histórias vinculadas:** HU01 e HU05.

**Delimitação do protótipo:** os testes devem utilizar credenciais fictícias. Os logs não substituem a proteção das senhas, e **senhas não devem ser registradas nos logs**. A implementação de proteção adequada de credenciais para uso em produção está fora do escopo atual.

---

## 3. Requisitos Não Funcionais (RNF)

Os RNFs descrevem restrições técnicas e características de qualidade do protótipo. Não substituem os RFs nem são tarefas por si só.

### RNF01 — Linguagem de implementação

O protótipo funcional deve ser implementado em **linguagem C**, conforme a disciplina de Programação Estruturada.

### RNF02 — Persistência local de dados

O protótipo deve utilizar **SQLite** para persistir os dados necessários ao funcionamento e recuperá-los em execuções posteriores.

### RNF03 — Proteção de dados pessoais

O Rankcom deve restringir a visualização e o tratamento dos dados pessoais de clientes e vendedores aos perfis autorizados, respeitando os princípios aplicáveis da **LGPD**.

### RNF04 — Usabilidade e validação de entradas

O protótipo deve utilizar **interface textual (CLI)** com menus identificados, mensagens compreensíveis e validação das entradas para evitar operações inconsistentes e falhas comuns de utilização.

---

## 4. Rastreabilidade

| Requisito | História(s) vinculada(s) | Regra relacionada |
|---|---|---|
| RF01 | HU01 | — |
| RF02 | HU02, HU03 | — |
| RF03 | HU04, HU05 | RN01 |
| RF04* | HU06 | RN01 |
| RF05* | HU07* | RN01 |
| RF06 | HU08 | — |
| RF07 | HU01, HU05 | — |
| RNF01 | HU01–HU08 | — |
| RNF02 | HU02–HU08 | — |
| RNF03 | HU01–HU05 | — |
| RNF04 | HU01–HU08 | — |

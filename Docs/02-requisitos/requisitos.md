# Requisitos do Sistema — Rankcom

**Empresa:** DMcom Administradora de Consórcios  
**Sistema:** Rankcom  
**Situação:** Em validação pela equipe  
**Versão:** Proposta revisada — outubro de 2026

> **Legenda:** `*` indica requisito com algum detalhe pendente de validação pelo grupo. Não significa que a funcionalidade esteja fora do escopo.

---

## 1. Requisitos Funcionais (RF)

Os requisitos funcionais descrevem as operações disponibilizadas pelo Rankcom aos perfis **Administrador**, **Gestor** e **Funcionário**.

### RF01 — Autenticação e Controle de Acesso

O sistema deve permitir a autenticação dos usuários por e-mail e senha, disponibilizando as funcionalidades conforme o perfil de acesso: Administrador, Gestor ou Funcionário.

### RF02 — Gerenciamento de Clientes

O sistema deve permitir que o Funcionário cadastre e consulte clientes, incluindo nome, CPF/CNPJ e dados de contato, possibilitando a vinculação desses registros a contratos de consórcio.

### RF03 — Gerenciamento de Funcionários e Equipes

O sistema deve permitir que o Administrador cadastre e consulte funcionários, associando-os a cargos, perfis de acesso e equipes comerciais.

### RF04 — Registro e Aprovação de Contratos

O sistema deve permitir que o Funcionário registre contratos vinculados a clientes cadastrados e que o Gestor responsável aprove ou reprove esses registros. Em caso de reprovação, deverá ser informada uma justificativa, permitindo a correção e o reenvio do contrato para análise.

### RF05* — Cálculo Automático de Comissões

O sistema deve calcular automaticamente as comissões dos contratos aprovados, aplicando um percentual **único e fixo para todos os vendedores**, conforme a política de comissionamento da DMcom.

**Pendente:** validar a base financeira de cálculo e o percentual definitivo. **5% é apenas um valor provisório para simulações**, sujeito a alteração.

### RF06* — Rankings Comerciais

O sistema deve apresentar um ranking geral dos vendedores e rankings específicos das três equipes comerciais, classificando os participantes em ordem decrescente pelo total de comissões válidas acumuladas no ciclo de apuração e destacando os três primeiros colocados de cada classificação. Os rankings devem ser atualizados após aprovações de contratos que gerem comissões válidas.

**Pendente:** definir o critério de desempate e confirmar a visibilidade dos rankings para cada perfil de acesso.

### RF07 — Gerenciamento de Ciclos de Apuração

O sistema deve permitir que o Administrador abra e encerre um **único ciclo de apuração compartilhado** pelo ranking geral e pelos rankings das três equipes, consolidando os resultados do período.

<br>

---

## 2. Requisitos Não Funcionais (RNF)

Os requisitos não funcionais estabelecem restrições e características técnicas aplicáveis ao protótipo do Rankcom.

### RNF01 — Linguagem de Implementação

O protótipo funcional deverá ser desenvolvido em **linguagem C**, conforme a disciplina de Programação Estruturada.

### RNF02 — Persistência de Dados

O protótipo deverá utilizar **SQLite** para armazenar localmente as informações necessárias ao funcionamento do sistema, permitindo sua recuperação entre execuções.

### RNF03 — Segurança de Autenticação

O sistema deverá restringir funcionalidades conforme o perfil autenticado. As senhas não deverão ser armazenadas em texto puro; deverá ser adotado um mecanismo adequado para armazenamento seguro de credenciais.

### RNF04 — Proteção de Dados Pessoais

O sistema deverá limitar o acesso aos dados pessoais de clientes e funcionários conforme os perfis autorizados, considerando os princípios aplicáveis da **Lei Geral de Proteção de Dados Pessoais (LGPD)**.

### RNF05 — Usabilidade e Validação de Entradas

O protótipo deverá apresentar interface textual de linha de comando (**CLI**), com menus identificados, mensagens compreensíveis e validação das entradas para reduzir erros de utilização.

<br>

---

## 3. Regras de Negócio (RN)

As regras de negócio representam condições e políticas próprias da DMcom que orientam o comportamento do Rankcom.

### RN01* — Comissão com Percentual Único e Fixo

A DMcom adotará um **percentual único e fixo de comissão para todos os vendedores**, independentemente da equipe comercial. Somente contratos **aprovados pelo Gestor** poderão gerar comissões válidas para contabilização nos rankings do Rankcom.

**Pendente:** definir a base financeira e confirmar o percentual definitivo. Para fins de simulação, poderá ser usado **5% como exemplo provisório**, sem caracterizar a política final da empresa.

**Vinculada aos requisitos:** RF04, RF05 e RF06.

> Talvez sejam adicionadas outras regras de negócio só serão incluídas após discussão e aprovação pela equipe.

<br>

---

## 4. Rastreabilidade com o Product Backlog

| Requisito funcional | História(s) de usuário | Regra de negócio relacionada |
|---|---|---|
| RF01 — Autenticação e acesso | HU01 | — |
| RF02 — Clientes | HU02 | — |
| RF03 — Funcionários e equipes | HU03 | — |
| RF04 — Contratos e aprovação | HU04, HU05 | RN01 |
| RF05* — Comissões | HU06* | RN01 |
| RF06* — Rankings | HU07* | RN01 |
| RF07 — Ciclos de apuração | HU08 | — |

## 5. Delimitação e Pendências

O escopo inicial **não inclui** o gerenciamento de premiações dentro do sistema nem o tratamento de cancelamentos posteriores à aprovação de contratos. Essas possibilidades poderão ser avaliadas como evoluções futuras.

**Pontos a validar antes da implementação definitiva:**

- Percentual de comissão e base financeira de incidência (RF05 e RN01).
- Critério de desempate e permissões de consulta aos rankings (RF06).
- Mecanismo concreto de armazenamento seguro de senhas no protótipo em C (RNF03).

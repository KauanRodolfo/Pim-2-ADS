# Sprint 6 — Planejamento, encerramento e entrega final

**Projeto:** Rankcom — DMcom Administradora de Consórcios  
**Período:** 01/11/2026 a 14/11/2026  
**Reunião de planejamento:** 01/11/2026  
**Encerramento (Review e Retrospective):** 14/11/2026  

## 1. Objetivo da Sprint

Promover o **fechamento técnico e documental do Rankcom**, concentrando os esforços da equipe na integração dos módulos em C com o banco SQLite, na conclusão das funcionalidades pendentes das Sprints anteriores, na execução de testes locais e na revisão final do relatório acadêmico do PIM II e do material de apresentação para entrega na UNIP.

A Sprint 6 representa o ciclo final de consolidação do semestre, sem abertura de novos escopos funcionais além dos requisitos já definidos (RF01–RF07 e RNF01–RNF04).

## 2. Diretrizes definidas no planejamento (01/11/2026)

- Integrar todos os módulos de apresentação (`main.c`, `login.c`, `admin.c`, `gestor.c`, `vendedor.c`), regras de negócio (`usuario.c`, `equipe.c`, `cliente.c`, `contrato.c`, `ranking.c`, `competicao.c`, `util.c`) e persistência (`banco.c`) em um único fluxo compilável com `sqlite3.c`.
- Consolidar e validar as regras de negócio fechadas: comissão fixa de **5% sobre o valor do crédito contratado** (exclusivamente após aprovação do gestor), reprovação definitiva (com justificativa opcional) e desempate pelo registro do vendedor que fez a primeira venda válida no ciclo.
- Finalizar a redação, formatação ABNT e revisão cruzada de todas as seções do relatório do PIM II (Introdução, Desenvolvimento das disciplinas, Integração da solução e Considerações Finais).
- Estruturar o roteiro e os slides de apresentação para a banca avaliadora, demonstrando os fluxos de interface textual (CLI) dos três perfis de usuário.
- Delimitar formalmente os itens não implementados no protótipo (recuperação de senha, gerenciamento de premiações, suporte a acessos simultâneos em rede e recursos avançados de acessibilidade) como limitações do protótipo e propostas de evolução futura.

## 3. Acompanhamento das entregas e fechamento das demandas

| Demanda / Issue de referência | Sprint de origem | Situação no encerramento (14/11) |
|---|---|---|
| [#9](https://github.com/KauanRodolfo/Pim-2-ADS/issues/9) — Cadastro e rotinas em C | 2 | **Concluída**; módulos de cadastro, menus por perfil (Admin, Gestor e Vendedor) e fluxo de contratos integrados no executável principal |
| [#11](https://github.com/KauanRodolfo/Pim-2-ADS/issues/11) — Integração SQLite e `structs` | 3 | **Concluída**; persistência local no arquivo `pim2.db` funcional via `criar_db()`, `sqlite3_exec()` e `sqlite3_get_table()` |
| [#14](https://github.com/KauanRodolfo/Pim-2-ADS/issues/14) e [#20](https://github.com/KauanRodolfo/Pim-2-ADS/issues/20) — Modelagem e normalização de banco | 3 / 4 | **Concluída**; modelos conceitual (MER Chen), lógico (3FN/DER) e físico com as seis tabelas relacionais documentados e integrados ao relatório |
| [#21](https://github.com/KauanRodolfo/Pim-2-ADS/issues/21) e [#30](https://github.com/KauanRodolfo/Pim-2-ADS/issues/30) — Documentação completa do PIM II | 3 / 4 | **Concluída**; textos de todas as disciplinas integrados, revisados e formatados para entrega final em novembro/2026 |
| [#22](https://github.com/KauanRodolfo/Pim-2-ADS/issues/22) e [#29](https://github.com/KauanRodolfo/Pim-2-ADS/issues/29) — Diagramas UML e de atividades | 3 / 4 | **Concluída**; diagramas de casos de uso, caso de uso descritivo, UML estendido (versão de 13/11) e atividades incorporados ao documento final |
| [#28](https://github.com/KauanRodolfo/Pim-2-ADS/issues/28) — Recuperação de senha | 4 | **Não implementada (Fora do escopo do protótipo)**; mantida no backlog como melhoria futura, preservando o escopo fechado de RF01 |
| Integração final e revisão geral (Sprint 6) | 6 | **Concluída em 14/11/2026**; compilação única validada, testes funcionais realizados e entrega acadêmica finalizada |
| Roteiro e slides de apresentação (`Rankcom — Apresentação PIM II`) | 6 | **Concluída**; material visual com 23 slides cobrindo contexto da DMcom, requisitos, diagramas UML/DER e demonstração das telas CLI dos 3 perfis |

## 4. Consolidação final do projeto (14/11/2026)

- **Código e Persistência:** O protótipo em C foi unificado em arquitetura de três camadas (Apresentação, Regras de Negócio e Persistência), operando localmente sobre o banco `pim2.db` com seis tabelas (`Usuarios`, `Equipes`, `Clientes`, `Contratos`, `Competicoes` e `LogsAuditoria`).
- **Regras de Negócio Padronizadas:** Fixadas na documentação e no fluxo comercial a comissão de **5% sobre o valor do crédito contratado**, a aprovação/reprovação definitiva pelo gestor (com motivo opcional) e o destaque de **um único vencedor** no ranking geral e em cada um dos três rankings por equipe.
- **Material de Apresentação e Demonstração:** Elaborada a apresentação oficial do projeto (`23 slides`), estruturando o roteiro da banca em quatro eixos (*Empresa, Requisitos, Diagramas e Produto*) e ilustrando a navegação completa pelos menus textuais de Administrador, Gestor e Vendedor.
- **Limitações Documentadas:** Conforme registrado nas Considerações Finais do relatório, ficaram fora da versão entregue a gestão de premiações, a execução concorrente em rede (cliente-servidor), a automatização completa de exclusão lógica em cascata (reservada para evolução futura com Stored Procedures) e recursos avançados de acessibilidade.

## 5. Review e Retrospective da Sprint 6 (14/11/2026)

| Tópico | Registro do encerramento |
|---|---|
| **Entregas efetivamente concluídas** | Integração dos módulos em C com SQLite, finalização dos diagramas UML e modelos de dados (MER/DER/Físico), testes locais das rotinas principais, revisão completa do relatório final do PIM II e criação dos 23 slides de apresentação. |
| **Pendências remanescentes / Escopo futuro** | Funcionalidades classificadas como evolução futura: recuperação de senha (Issue #28), módulo de premiações, implantação da rede corporativa/arquitetura cliente-servidor e validação em ambiente real. |
| **O que funcionou bem** | Divisão modular do código por responsabilidade e perfil de acesso, rastreabilidade entre requisitos e histórias de usuário e concentração de esforços na reta final para unificar documentação, slides e protótipo. |
| **Dificuldades enfrentadas** | Sincronização paralela entre alterações nas regras de negócio da documentação e os arquivos do repositório GitHub, além da validação de consultas SQL mais complexas (ciclo compartilhado e desempate). |
| **Lições aprendidas e melhorias** | Manter uma fonte única de verdade atualizada a cada mudança de regra de negócio e priorizar testes integrados desde as Sprints intermediárias para evitar acúmulo na entrega final. |

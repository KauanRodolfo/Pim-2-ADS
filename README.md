<div align="center">

# Rankcom

### Gestão Comercial, Comissões e Gamificação

**Uma proposta de transformação digital para a DMcom Administradora de Consórcios**

![Status](https://img.shields.io/badge/status-em%20desenvolvimento-orange)
![Linguagem](https://img.shields.io/badge/linguagem-C-blue)
![Banco de dados](https://img.shields.io/badge/banco%20de%20dados-SQLite-07405E)

</div>

## Visão geral

O **Rankcom** é um projeto de sistema de gestão comercial voltado à **DMcom Administradora de Consórcios**, empresa fictícia com **64 colaboradores**. Seu propósito é apoiar a digitalização de processos da área comercial, reunindo o registro e a análise de contratos, a apuração de comissões e o acompanhamento do desempenho das equipes.

A proposta utiliza **gamificação**, por meio de rankings, para tornar os resultados mais visíveis e incentivar o reconhecimento dos vendedores. A gestão de premiações poderá ser incorporada em uma versão futura. O projeto está **em desenvolvimento**: as funcionalidades a seguir representam o escopo previsto, e não necessariamente recursos já finalizados.

## Contexto da organização

A **DMcom** atua na administração e comercialização de cotas de consórcio. Sua estrutura inclui três equipes comerciais, cada uma composta por **um gestor e sete vendedores** — totalizando **21 vendedores**.

No cenário fictício definido para o projeto, o acompanhamento comercial e a consolidação dos resultados ainda dependem de **controles predominantemente manuais**. Isso dificulta a consulta rápida às informações e aumenta o esforço de apuração das comissões e das classificações.

A estrutura organizacional, os processos de negócio e o contexto da empresa estão detalhados na [caracterização da DMcom](Docs/01-Empresa/caracteriza%C3%A7%C3%A3o_da_DMcom.md).

## Problemas e solução proposta

| Desafio identificado | Como o Rankcom pretende contribuir |
|---|---|
| Apuração manual e demorada das comissões | Automatizar o cálculo de **5%** de comissão sobre contratos aprovados, com percentual fixo para todos os vendedores |
| Dificuldade de acompanhar contratos | Centralizar o registro, a análise e a situação de cada contrato |
| Visibilidade limitada do desempenho comercial | Disponibilizar rankings por equipe e um ranking geral, atualizados com base nas comissões válidas |
| Baixo engajamento e reconhecimento pouco visível | Destacar **um vencedor** no ranking geral e **um vencedor por equipe**, com critério de desempate definido |

## Funcionalidades previstas

- **Acesso por perfil:** administrador, gestor e vendedor.
- **Logs de auditoria:** registro de tentativas de acesso e de aprovações ou reprovações de contratos, conforme o escopo do protótipo.
- **Cadastro de clientes:** registro e consulta das informações necessárias à operação comercial.
- **Gerenciamento de vendedores e equipes:** cadastro e associação dos vendedores às equipes comerciais.
- **Registro de contratos:** vinculação de contratos aos clientes e encaminhamento para análise.
- **Análise de contratos:** aprovação ou reprovação definitiva pelo gestor, sem justificativa obrigatória, correção ou reenvio do mesmo registro.
- **Comissões:** cálculo de **5%**, aplicado somente aos contratos aprovados, sobre a base financeira que será definida.
- **Ranking geral:** classificação dos 21 vendedores das três equipes, destacando **apenas o primeiro colocado**.
- **Rankings por equipe:** classificação dos sete vendedores de cada equipe, destacando **apenas o primeiro colocado**.
- **Ciclos de apuração:** abertura e encerramento de um ciclo compartilhado entre os quatro rankings pelo administrador.

> **Limites do escopo inicial:** o gerenciamento de premiações e o tratamento de cancelamentos posteriores à aprovação dos contratos são possibilidades para versões futuras. O percentual de **5% está definido**, mas a base financeira do cálculo ainda será especificada. A autenticação do protótipo será testada com credenciais fictícias; a utilização de logs não equivale à proteção de senhas exigida em produção.

## Como funciona o fluxo comercial

```mermaid
flowchart TD
    A["Vendedor cadastra o cliente"] --> B["Vendedor registra o contrato"]
    B --> C["Contrato em análise"]
    C --> D{"Gestor aprova?"}
    D -- "Não" --> E["Contrato reprovado: processo encerrado, sem reenvio e sem comissão"]
    D -- "Sim" --> G["Contrato aprovado"]
    G --> H["Cálculo e contabilização da comissão fixa de 5%"]
    H --> I["Atualização dos rankings geral e por equipe"]
```

**Funcionamento definido para a proposta:**

- Apenas contratos **aprovados** contribuem para as comissões contabilizadas e para a classificação nos rankings.
- O percentual de comissão é **5% para todos os vendedores**; a base financeira sobre a qual se aplica será especificada.
- O **administrador** controla a abertura e o encerramento do **ciclo compartilhado** dos rankings.
- O ranking geral e os três rankings por equipe possuem **classificações próprias** e destacam **somente um vencedor por ranking**. Em caso de empate no total de comissões válidas, prevalece quem registrou primeiro uma venda válida no ciclo, vinculada a contrato posteriormente aprovado.
- **O sistema não gerencia premiações nesta versão**; esse recurso fica reservado a uma possível evolução futura.

## Tecnologias e ferramentas

| Categoria | Tecnologia ou ferramenta | Finalidade no projeto |
|---|---|---|
| Programação | **C** | Implementação do protótipo e das rotinas principais |
| Persistência | **SQLite** | Armazenamento local dos dados |
| Ambiente de desenvolvimento | **Code::Blocks** | Edição, compilação e execução do protótipo |
| Diagramas | **Draw.io** | Fluxogramas e modelagem visual |
| Modelagem UML | **Astah** | Diagramas de análise e projeto |
| Versionamento e colaboração | **Git e GitHub** | Controle de versões e integração das contribuições |

## Estrutura do repositório

O repositório separa código-fonte, diagramas e documentação. Utilize a tabela abaixo para acessar cada área principal.

| Diretório ou arquivo | Conteúdo | Acesso |
|---|---|---|
| `Código/` | Código-fonte do protótipo, rotinas em C e integração com o banco de dados. | [Ver código-fonte](C%C3%B3digo/) |
| `Diagramas/` | Fluxogramas, modelos de dados e diagramas de análise do sistema. | [Ver diagramas](Diagramas/) |
| `Docs/` | Documentação técnica da empresa, do produto e do desenvolvimento. | [Ver documentos](Docs/) |
| `README.md` | Apresentação geral e ponto de entrada do repositório. | Você está aqui |

## Documentação técnica

A pasta [`Docs/`](Docs/) concentra os arquivos Markdown que registram **o contexto da organização, as decisões do produto e a evolução do desenvolvimento**. O relatório acadêmico do PIM II é mantido separadamente.

### Organização dos documentos

| Área | Arquivo ou pasta de referência | O que documenta | Situação |
|---|---|---|---|
| **01 · Empresa** | [Caracterização da DMcom](Docs/01-Empresa/caracteriza%C3%A7%C3%A3o_da_DMcom.md) | Identificação, missão, estrutura, processos comerciais e cenário anterior à digitalização. | Documento disponível; sujeito à revisão da equipe |
| **02 · Especificação e Planejamento** | [Requisitos](Docs/02-Especificacao-e-Planejamento/Requisitos.md) | Regras de negócio, requisitos funcionais, não funcionais e rastreabilidade. | Em validação pela equipe |
| **02 · Especificação e Planejamento** | [Product Backlog](Docs/02-Especificacao-e-Planejamento/Product_Backlog.md) | Histórias de usuário, critérios de aceitação, prioridades e rastreabilidade. | Em validação pela equipe |
| **Sprints** | [Consultar Sprints](Docs/03-Sprints/) | Planejamento, atividades e entregas das Sprints; acompanhamento da execução pelo Kanban. | Organização e histórico em revisão |
| **Atas** | [Consultar Atas](Docs/04-Atas/) | Reuniões, decisões, responsáveis e pendências. | Conforme registros disponíveis |

> **Navegação:** os acessos direcionam para as pastas e documentos existentes na estrutura da branch `main` utilizada nesta revisão.

### Como consultar a documentação

A **caracterização** apresenta a organização e o problema; os **requisitos** definem o comportamento esperado e as restrições do sistema; o **Product Backlog** registra as necessidades priorizadas e seus critérios de aceitação e prioridades; os documentos de **Sprints** acompanham o planejamento e as entregas; e as **atas** preservam as decisões coletivas.

## Execução do protótipo

**O código principal ainda está em desenvolvimento e integração.** As instruções completas de compilação, configuração do SQLite e execução do sistema serão acrescentadas quando houver um fluxo único de execução validado.

Para obter o repositório localmente:

```bash
git clone https://github.com/KauanRodolfo/Pim-2-ADS.git
cd Pim-2-ADS
```

Os arquivos de implementação disponíveis podem ser consultados na pasta [`Código/`](C%C3%B3digo/). A presença de módulos ou projetos individuais não significa que todas as funcionalidades já estejam integradas.

## Organização do desenvolvimento

A equipe utiliza práticas do **Scrum**, com planejamento semanal e apoio de um **quadro Kanban no GitHub Projects**. As reuniões aos sábados permitem revisar o que foi feito, identificar pendências e decidir as atividades da semana seguinte.

Os registros detalhados das **Sprints** ficam na documentação do projeto, enquanto o **Kanban** apresenta o andamento das atividades. O histórico das Sprints anteriores será relacionado às atividades do Kanban sem modificar retrospectivamente os resultados alcançados.

## Equipe

| Integrante | Responsabilidade |
|---|---|
| Victor de Souza Pascoaleto | Product Owner |
| Kauan Rodolfo Machado Silva | Scrum Master |
| Arthur Assis | Desenvolvedor |
| Elton Henrique da Silva | Desenvolvedor |
| William Ramos Cintra | Desenvolvedor |

## Colaboração e controle de versões

O desenvolvimento é realizado em equipe, com uso de branches que são criadas conforme surgem novas funcionalidades no projeto. O GitHub é utilizado para preservar o histórico de evolução do projeto e apoiar a revisão das contribuições.

---

<div align="center">

**DMcom Administradora de Consórcios · Projeto Rankcom**

*Desenvolvido por estudantes de Análise e Desenvolvimento de Sistemas da Universidade Paulista (UNIP).*

</div>

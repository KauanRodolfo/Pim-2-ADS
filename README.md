# ADMcom — Sistema de Gamificação Comercial e Comissões

Projeto Integrado Multidisciplinar II (PIM II) do curso de **Análise e Desenvolvimento de Sistemas** da **UNIP**.

## Sobre o projeto

O **ADMcom** é uma solução de transformação digital para uma administradora de consórcios fictícia. O sistema automatiza o **cálculo de comissões** dos vendedores e usa **gamificação** (rankings e premiações) para aumentar a motivação e a transparência na equipe comercial.

## A empresa fictícia

Administradora de consórcios de pequeno porte, com cerca de 65 funcionários, que vende e administra **contratos de consórcio** por meio de equipes de vendas. Cada equipe tem um gestor e vários vendedores.

A caracterização completa da empresa (missão, estrutura, processos, público-alvo, problemas e justificativa) está em [`Docs/01-Empresa/caracterização.md`](Docs/01-Empresa/caracteriza%C3%A7%C3%A3o.md).

## Problema e solução

| Problema | Solução |
|---|---|
| Comissões calculadas manualmente, com erros e demora | Cálculo automático no cadastro do contrato |
| Aprovação de contratos sem padrão | Fluxo único: em análise, aprovado ou reprovado com motivo |
| Pouca visibilidade do desempenho | Rankings atualizados a cada contrato aprovado |
| Baixo engajamento da equipe de vendas | Competições com prêmios para o 1º, 2º e 3º lugares |
| Dados de clientes sem controle de acesso | Perfis de acesso, consentimento LGPD e log de auditoria |

## Funcionalidades (MVP)

- Login por perfil: **vendedor**, **gestor** e **admin**
- Cadastro de clientes (com consentimento LGPD)
- Cadastro de contratos de consórcio com comissão calculada
- Aprovação ou reprovação de contratos pelo gestor
- **Ranking geral** (todos os vendedores) e **rankings internos** (por equipe)
- Competições com período, eventos e prêmios
- Gerenciamento de equipes, usuários e tipos de consórcio
- Log de auditoria de exclusões e arquivamentos

## Tecnologias

- Linguagem **C**
- Banco de dados **SQLite**
- IDE **Code::Blocks**
- Modelagem: Draw.io (DER e fluxograma) e Astah (casos de uso e UML)
- Versionamento: Git e GitHub

## Estrutura do repositório

```
.
├── Código_em_C/     # código-fonte em C e módulo de banco de dados
├── Diagramas/       # DER, fluxograma, casos de uso e demais diagramas
├── Docs/            # documentação da empresa e do projeto
├── .gitignore
└── README.md
```

## Documentação

A pasta [`Docs/`](Docs/) reúne a documentação da empresa e do andamento do projeto, como a caracterização da organização, os requisitos, o backlog, as sprints e as atas, à medida que forem sendo adicionados.

## Como executar

> Esta seção será atualizada quando o código principal estiver integrado.

1. Clonar o repositório: `git clone https://github.com/KauanRodolfo/Pim-2-ADS.git`
2. Abrir o arquivo de projeto `.cbp` da pasta desejada no Code::Blocks
3. Compilar e executar (`F9`)

## Metodologia e status

O projeto segue o **Scrum**, com sprints de duas semanas.

| Sprint | Status | Data Conclusão |
|---|---|---|
| Sprint 1 | Concluída | 02/09/2026
| Sprint 2 | Em andamento | ---

## Time de desenvolvimento

| Nome | Papel |
|---|---|
| Victor de Souza Pascoaleto | Product Owner |
| Kauan Rodolfo Machado Silva | Scrum Master |
| Arthur Assis | Desenvolvedor |
| Chayon Sunshine Silva e Priante | Desenvolvedor |
| Elton Henrique da Silva | Desenvolvedor |
| William Ramos Cintra | Desenvolvedor |

## Fluxo de trabalho no Git

- Cada integrante trabalha em uma **branch própria**.
- Mudanças entram na `main` por **merge**, combinado com o grupo; código passa por **Pull Request** com revisão.


# Caracterização da Organização — DMcom Administradora de Consórcios

## 1. Identificação da organização

A **DMcom Administradora de Consórcios** é uma empresa fictícia de pequeno porte, com **64 colaboradores**, criada como cenário organizacional para o desenvolvimento do **Rankcom**, uma solução de apoio à gestão comercial. Atua no setor de serviços financeiros, com foco na administração e comercialização de consórcios, e adota uma cultura de gestão orientada por metas, transparência e reconhecimento de resultados.

## 2. Ramo de atuação

A DMcom atua no **segmento de consórcios**, oferecendo a comercialização e a administração de cotas para a aquisição planejada de bens e serviços. Sua operação envolve o relacionamento com interessados e consorciados, o registro e a análise de contratos e o acompanhamento administrativo e financeiro das operações.

## 3. Missão, visão e valores

**Missão:** facilitar a conquista de bens e serviços por meio de consórcios acessíveis, transparentes e seguros, com atendimento próximo e humano. Unimos tecnologia e uma cultura de reconhecimento para valorizar as pessoas que constroem cada resultado.

**Visão:** ser reconhecida como uma administradora de consórcios ágil, transparente, movida por tecnologia e pelo engajamento de suas equipes.

**Valores:**

- **Transparência com o cliente:** informações claras sobre contratos, condições e prazos.
- **Ética e proteção dos dados:** tratamento responsável e seguro das informações de clientes e colaboradores.
- **Meritocracia:** reconhecimento proporcional ao desempenho, de forma justa e visível.
- **Colaboração:** trabalho conjunto entre as equipes e com os clientes.

## 4. Principais produtos ou serviços

A DMcom administra e comercializa **cotas de consórcio**. A comercialização dessas cotas constitui o principal fluxo de negócio considerado neste projeto. Cada venda é formalizada por meio de um contrato, que passa por análise antes de ser reconhecido como aprovado. **Somente contratos aprovados** geram comissões válidas para os rankings comerciais do Rankcom. A empresa adota um **percentual de comissão único e fixo** para todos os vendedores; sua base de cálculo e o valor definitivo do percentual ainda dependem de validação.

---

## 5. Estrutura organizacional

### 5.1 Organograma

```mermaid
flowchart TD
    D["Diretoria"]
    D --> C["Comercial"]
    D --> AC["Administração<br/>de contratos"]
    D --> AT["Atendimento ao<br/>consorciado"]
    D --> F["Financeiro e<br/>cobrança"]
    D --> RH["Recursos<br/>Humanos"]
    D --> TI["Tecnologia da<br/>Informação"]
    D --> J["Jurídico e<br/>compliance"]
    C --> EA["Equipe A:<br/>1 gestor + 7 vendedores"]
    C --> EB["Equipe B:<br/>1 gestor + 7 vendedores"]
    C --> EC["Equipe C:<br/>1 gestor + 7 vendedores"]
```

### 5.2 Setores e quantidade de pessoas

| Setor | Pessoas | Função principal |
|---|---:|---|
| Diretoria | 3 | Direção geral, estratégia e definição de metas |
| **Comercial** | **24** | 3 gestores e 21 vendedores, divididos em 3 equipes de vendas |
| Administração de contratos | 10 | Conferência documental e organização dos contratos |
| Atendimento ao consorciado | 8 | Suporte e relacionamento pós-venda |
| Financeiro e cobrança | 8 | Cobrança das parcelas e pagamento das comissões |
| Recursos humanos | 4 | Gestão de pessoas, metas e apoio às iniciativas de reconhecimento |
| Tecnologia da informação | 5 | Infraestrutura, segurança e sistemas (inclui o administrador do Rankcom) |
| Jurídico e compliance | 2 | Conformidade regulatória e proteção de dados |
| **Total** | **64** | |

O setor **comercial** é o foco do Rankcom. Está dividido em **três equipes de oito pessoas**, cada uma formada por **um gestor e sete vendedores**. Assim, há **21 vendedores** participantes dos rankings e **três gestores** responsáveis pelo acompanhamento de suas respectivas equipes.

### 5.3 Perfis de usuário do sistema

| Perfil | Quem é | Principais responsabilidades no Rankcom |
|---|---|---|
| **Administrador** | Integrante do setor de TI | Gerencia a plataforma e os usuários; abre e encerra o ciclo de apuração compartilhado pelos rankings geral e internos. Na operação da empresa, é responsável pelas premiações do ranking geral, não gerenciadas pelo protótipo. |
| **Gestor** | Gestor de uma equipe comercial | Analisa, aprova ou reprova os contratos de sua equipe; acompanha os rankings geral e internos. Na operação da empresa, define as premiações de sua equipe, fora do sistema nesta versão. |
| **Funcionário (vendedor)** | Vendedor de uma equipe comercial | Cadastra clientes; registra contratos de consórcio; corrige e reenvia contratos reprovados; acompanha sua classificação nos rankings. |

---

## 6. Principais processos de negócio

### 6.1 Fluxo principal de venda e aprovação

```mermaid
flowchart TD
    A["Vendedor cadastra<br/>o cliente"] --> B["Registra o contrato<br/>de consórcio"]
    B --> C["Contrato em análise"]
    C --> D{"Gestor aprova?"}
    D -- "Sim" --> E["Comissão é contabilizada<br/>no ciclo vigente"]
    E --> F["Ranking geral e ranking<br/>da equipe são atualizados"]
    D -- "Não" --> G["Reprovação com<br/>motivo registrado"]
    G --> H["Vendedor corrige<br/>o contrato"]
    H --> I["Reenvia para análise"]
    I --> C
```

O registro de um contrato, isoladamente, **não gera pontuação no ranking**. A comissão é contabilizada para fins de classificação somente depois da aprovação pelo gestor. Em caso de reprovação, o motivo é registrado, e o vendedor pode corrigir e reenviar o contrato para nova análise.

### 6.2 Ciclo de apuração e classificação

```mermaid
flowchart TD
    A["Administrador abre o<br/>ciclo de apuração"] --> B["Gestores aprovam contratos"]
    B --> C["Comissão com percentual fixo<br/>é contabilizada"]
    C --> D["Ranking geral e rankings<br/>por equipe são atualizados"]
    D --> E["Administrador encerra<br/>o ciclo compartilhado"]
    E --> F["Classificações finais e<br/>três primeiros colocados"]
    F --> G["Resultados preservados<br/>para consulta"]
```

O Rankcom apresenta **quatro classificações**, vinculadas a **um único ciclo de apuração**:

- **Ranking geral:** reúne os 21 vendedores das equipes A, B e C.
- **Ranking interno da equipe A:** considera os sete vendedores da equipe A.
- **Ranking interno da equipe B:** considera os sete vendedores da equipe B.
- **Ranking interno da equipe C:** considera os sete vendedores da equipe C.

Todos os rankings ordenam os vendedores conforme o **total de comissões válidas no ciclo**, em ordem decrescente. A aprovação de um contrato atualiza automaticamente a posição do vendedor nos rankings aplicáveis. Ao encerrar o ciclo compartilhado, o administrador consolida os resultados dos quatro rankings.

**A empresa poderá premiar os três primeiros colocados de cada ranking.** O administrador define as recompensas do ranking geral e cada gestor as de sua equipe. **O Rankcom não cadastra, distribui nem registra premiações nesta versão**: o módulo de gerenciamento de prêmios é uma possibilidade de evolução futura.

### 6.3 Descrição dos processos

1. **Cadastro de clientes:** o vendedor registra os dados do cliente interessado em uma cota de consórcio.
2. **Registro de contrato:** o vendedor formaliza a venda e vincula o contrato ao cliente cadastrado, encaminhando-o para análise.
3. **Análise e aprovação:** o gestor responsável avalia o contrato e decide pela aprovação ou reprovação. Em caso de reprovação, registra o motivo para permitir a correção e o reenvio pelo vendedor.
4. **Cálculo de comissão:** após a aprovação, o sistema aplica um percentual único e fixo a todos os vendedores e contabiliza a comissão válida para fins de apuração comercial. Para simulações, pode-se adotar **5% provisoriamente**, sem estabelecer esse número nem a base de incidência como definitivos.
5. **Atualização dos rankings:** as comissões aprovadas atualizam automaticamente o ranking geral e o ranking interno da equipe correspondente, durante o ciclo aberto.
6. **Encerramento do ciclo:** o administrador encerra o ciclo compartilhado, consolidando a classificação final dos quatro rankings.
7. **Reconhecimento e premiações:** na operação da empresa, o administrador define as premiações gerais e cada gestor as de sua equipe; a gestão dessas recompensas **não faz parte do escopo inicial do Rankcom**.
8. **Registro e preservação do histórico:** contratos, decisões e resultados relevantes são preservados para consulta e rastreabilidade, conforme as funcionalidades previstas. Políticas detalhadas para cancelamentos posteriores à aprovação ficam fora do escopo inicial.

---

## 7. Público-alvo

A atuação da **DMcom** e a utilização do **Rankcom** envolvem públicos distintos:

- **Clientes e consorciados da DMcom:** pessoas interessadas em adquirir bens e serviços de maneira planejada por meio de consórcios. Seus dados e contratos integram o processo comercial, mas eles **não são usuários diretos** do Rankcom nesta proposta.
- **Usuários internos do Rankcom:** vendedores, gestores comerciais e administradores, que utilizam o sistema de acordo com suas responsabilidades e permissões.

A solução é direcionada principalmente às necessidades operacionais e gerenciais do setor comercial.

## 8. Problemas identificados

O acompanhamento comercial baseado em controles predominantemente manuais apresenta três problemas principais:

1. **Falta de visibilidade:** vendedores e gestores não dispõem de uma visão atualizada e integrada do desempenho individual e das equipes.
2. **Lentidão no cálculo e na apuração das comissões:** a consolidação manual dos contratos aprovados exige tempo e pode dificultar a conferência dos resultados.
3. **Baixo engajamento:** a ausência de acompanhamento contínuo e de reconhecimento visível dificulta a percepção dos resultados alcançados pelos vendedores.

## 9. Justificativa da solução proposta

O **Rankcom** é proposto como uma solução de **transformação digital** voltada à gestão comercial da DMcom. O sistema reúne o registro de contratos, o acompanhamento da aprovação, o cálculo automatizado das comissões válidas e a atualização dos rankings, reduzindo a dependência de controles manuais.

Por meio da **gamificação corporativa**, o Rankcom disponibiliza classificações atualizadas por equipe e em nível geral, destacando os três primeiros colocados de cada ranking. A empresa poderá associar premiações a essas classificações, mas o cadastro e o controle dos prêmios ficam para evolução futura do sistema.

Além de ampliar a visibilidade dos resultados para os vendedores, a solução oferece aos gestores informações mais organizadas para acompanhar suas equipes e ao administrador instrumentos para controlar os ciclos de apuração. Dessa forma, pretende contribuir para maior transparência, agilidade operacional e engajamento da área comercial.

## 10. Contexto tecnológico atual

Antes da implantação do Rankcom, a DMcom utiliza **procedimentos predominantemente manuais** para acompanhar as vendas, conferir os contratos aprovados, calcular comissões e consolidar classificações de desempenho. As informações precisam ser reunidas periodicamente pelos setores responsáveis, e os resultados são comunicados às equipes após a apuração.

Esse modelo dificulta o acompanhamento imediato das posições dos vendedores e aumenta o esforço necessário para consolidar dados de diferentes equipes. A ausência de uma ferramenta integrada para o processo comercial torna a apuração mais lenta e limita a visibilidade dos resultados ao longo do ciclo.

Nesse cenário, a implantação do **Rankcom** representa a passagem de controles comerciais manuais para um processo digital integrado: o vendedor registra o contrato, o gestor realiza a análise, e as aprovações alimentam automaticamente o cálculo das comissões e os rankings geral e internos. O administrador acompanha a operação dos ciclos, enquanto a definição de premiações permanece como atividade organizacional externa ao protótipo.

O escopo da transformação digital concentra-se **na gestão de contratos comerciais, comissões e rankings**, não na substituição de todos os sistemas administrativos ou financeiros utilizados pela organização.


## 11. Delimitação do escopo e possibilidades de evolução

A versão inicial do **Rankcom** contempla cadastro de clientes, registro e análise de contratos, comissões com percentual fixo, atualização de rankings geral e por equipe e gestão do ciclo compartilhado. O percentual de **5%** pode ser utilizado apenas como parâmetro **provisório de simulação**. A **base financeira de cálculo** e o percentual definitivo permanecem sujeitos à aprovação do grupo.

A gestão de recompensas — cadastro de prêmios, atribuição e registro de entregas — e o tratamento de cancelamentos posteriores à aprovação **não integram o escopo inicial**, podendo ser considerados evoluções futuras.

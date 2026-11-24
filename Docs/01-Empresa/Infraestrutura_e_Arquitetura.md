# Infraestrutura e Arquitetura do Rankcom

Este documento apresenta a organização da infraestrutura de rede proposta para o ambiente da DMcom e a arquitetura lógica do sistema Rankcom. O objetivo é registrar, de forma complementar à caracterização da empresa, os elementos de infraestrutura e a divisão interna da aplicação.

## 1. Infraestrutura de rede

A infraestrutura de rede proposta para a DMcom adota uma organização física em estrela. Nesse modelo, os dispositivos e pontos de acesso das diferentes áreas são conectados a um equipamento central de comutação, representado pelo switch gerenciável. Essa organização facilita a administração da rede, a identificação de conexões e a manutenção dos pontos.

O ambiente ilustrado contempla três salas destinadas às equipes, uma área de recepção e um espaço reservado à infraestrutura (rack). Nas salas das equipes e na recepção, os computadores e pontos de acesso sem fio se conectam à estrutura central. O switch, por sua vez, estabelece a ligação com o roteador/firewall e com o acesso à internet por fibra.

### 1.1. Componentes representados

- **Computadores:** estações de trabalho utilizadas pelos colaboradores nas salas das equipes e na recepção.
- **Pontos de acesso Wi-Fi (APs):** fornecem conectividade sem fio nas áreas representadas.
- **Switch gerenciável:** concentra as conexões cabeadas e permite administrar a rede.
- **Roteador/firewall:** realiza a interligação com a rede externa e representa a camada de controle e proteção do tráfego.
- **Internet por fibra:** conexão externa utilizada para acesso a serviços e recursos disponíveis na internet.
- **Rack de infraestrutura:** espaço destinado à organização dos equipamentos centrais de rede.

### 1.2. Segmentação lógica da rede

Além da organização física, o projeto prevê a separação lógica dos ambientes por meio de VLANs, permitindo agrupar dispositivos conforme sua finalidade e restringir a comunicação entre segmentos quando necessário.

| Segmento | Finalidade |
|---|---|
| VLAN 10 — Comercial | Dispositivos utilizados pelas equipes comerciais |
| VLAN 20 — Administrativo | Dispositivos da área administrativa |
| VLAN 30 — TI | Dispositivos destinados à equipe de tecnologia |
| Rede de servidores | Segmento reservado aos serviços e recursos internos |
| Rede de visitantes | Acesso separado para visitantes, sem integração direta à rede corporativa |

A utilização de VLANs depende de configuração adequada nos equipamentos de rede, especialmente no switch gerenciável e no roteador/firewall. As regras de comunicação entre os segmentos devem seguir as necessidades operacionais e os requisitos de segurança da organização.

**Observação:** este documento não define faixas de endereçamento IP, máscaras, gateways ou regras detalhadas de firewall. Esses parâmetros devem ser documentados após sua definição no planejamento técnico da infraestrutura.

### 1.3. Diagrama da topologia de rede

A figura apresenta a disposição das salas, das estações de trabalho e dos pontos de acesso, além da conexão dos ambientes ao switch gerenciável, ao roteador/firewall e à internet.

*Figura 1 — Representação da infraestrutura física de rede proposta para a DMcom.*

<img width="1448" height="1013" alt="topologia-rede" src="https://github.com/user-attachments/assets/121daf52-6107-4480-a685-dbacb8e2c9fb" />

## 2. Arquitetura do sistema Rankcom

O Rankcom foi organizado em três camadas lógicas: apresentação, regras de negócio e persistência de dados. Essa divisão busca separar as responsabilidades da aplicação, facilitar a organização do código e tornar mais clara a relação entre a interação com o usuário, o processamento das operações e o armazenamento das informações.

### 2.1. Camada de apresentação

É responsável pela interação entre o sistema e seus usuários. O Rankcom utiliza uma interface textual (CLI), por meio da qual são exibidos menus, opções, mensagens e solicitações de entrada.

Essa camada contempla:

- **Entrada e autenticação:** arquivos `main.c` e `login.c`, responsáveis pelo fluxo inicial e pelo acesso ao sistema.
- **Menus por perfil:** arquivos `admin.c`, `gestor.c` e `vendedor.c`, que organizam as opções disponíveis conforme o perfil de acesso.
- **Interação com o usuário:** leitura de entradas, apresentação de opções e exibição de mensagens.

### 2.2. Camada de regras de negócio

Concentra os processos comerciais e as validações que orientam o funcionamento do Rankcom. Nela estão as rotinas de apoio, as operações relacionadas aos cadastros e contratos e os procedimentos de apuração e classificação.

Conforme a organização apresentada no diagrama, essa camada reúne:

- **Rotinas de apoio:** `util.c` e `util.h`.
- **Operações comerciais:** `usuario.c`, `equipe.c`, `cliente.c` e `contrato.c`.
- **Apuração e classificação:** `ranking.c` e `competicao.c`, relacionados ao cálculo das comissões, aos rankings e aos ciclos.

As regras de negócio incluem, entre outros aspectos, a comissão fixa de 5% e os critérios de formação dos rankings, observadas as condições estabelecidas na documentação funcional do projeto.

### 2.3. Camada de persistência de dados

É responsável pelo acesso e pela gravação das informações utilizadas pelo sistema. O projeto utiliza a biblioteca SQLite para trabalhar com uma base de dados local.

O diagrama apresenta os seguintes elementos:

- **Acesso ao banco:** `banco.c` e `banco.h`, responsáveis pelas rotinas de acesso à base.
- **Biblioteca SQLite:** integração realizada por meio de `sqlite3.c` e `sqlite3.h`, conforme a organização representada no projeto.
- **Base local:** arquivo `pim2.db`, indicado no diagrama como contendo seis tabelas relacionais, incluindo `LogsAuditoria`.

A persistência local permite que os registros utilizados pelo sistema sejam armazenados em uma base relacional, acessada pelas rotinas específicas da aplicação.

### 2.4. Diagrama da arquitetura em camadas

A figura abaixo apresenta a divisão lógica do Rankcom, destacando a responsabilidade de cada camada e os principais módulos associados.

*Figura 2 — Representação da arquitetura lógica do Rankcom em três camadas.*

<img width="1535" height="1024" alt="arquitetura-sistema-camadas" src="https://github.com/user-attachments/assets/ba07775a-10fc-4c4c-b28e-54a0adc5f709" />

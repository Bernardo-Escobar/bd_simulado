# 💾 bd_simulado — Simulador de Banco de Dados com IPC e Threads

Projeto desenvolvido para a disciplina de **Sistemas Operacionais (M1)** da Universidade do Vale do Itajaí (UNIVALI), com foco na aplicação de conceitos como **IPC**, **threads**, **concorrência** e **sincronização** por meio de um simulador de banco de dados em C++.

## 🎯 Objetivo

Simular um sistema de gerenciamento de requisições a um banco de dados usando:

- 🧵 Threads para paralelismo no servidor
- 🔄 Comunicação entre processos (IPC) com *pipes* ou *memória compartilhada*
- 🔐 Sincronização com **mutex** ou **semáforos**
- 📁 Armazenamento de registros simulando operações SQL como **INSERT**, **DELETE** e **SELECT**

## 📁 Estrutura do Projeto

```bash
bd_simulado/
├── banco.h         # Definição da struct Registro e funções de manipulação do "banco"
├── servidor.cpp    # Recebe requisições via IPC e processa com threads
├── cliente.cpp     # Envia requisições para o servidor via IPC
└── README.md       # Este arquivo
```

## ⚙️ Como Compilar e Executar

🔧 Compilação

Servidor:
```bash
g++ servidor.cpp -o servidor -lpthread -lrt
```

Cliente:
```bash
g++ cliente.cpp -o cliente -lrt
```


▶️ Execução
Em um terminal, inicie o servidor:
```bash
./servidor
```

Em outro terminal, execute o cliente:
```bash
./cliente
```

## 🔒 Concorrência e Sincronização
O servidor cria threads para tratar múltiplas requisições em paralelo.

A manipulação do banco (inserção, deleção, leitura) é protegida por:

pthread_mutex_t para garantir exclusão mútua;

ou sem_t para controle mais refinado de acesso.

## 🧪 Resultados Esperados
Inserção e remoção seguras de registros em ambiente concorrente;

Nenhuma condição de corrida ou corrupção de dados;

Processamento paralelo de requisições enviadas pelo cliente.

## 🧠 Conceitos Trabalhados
Comunicação entre processos (IPC) com pipes ou memória compartilhada

Criação e gerenciamento de threads (pthread)

Sincronização com mutex e semáforo

Leitura e escrita concorrente

Simulação de banco de dados leve

## 👨‍💻 Autoria
Projeto desenvolvido por alunos da Universidade do Vale do Itajaí (UNIVALI) — Campus Itajaí
Disciplina: Sistemas Operacionais

## 📜 Licença
Uso acadêmico apenas. Livre para fins educacionais e de aprendizado.

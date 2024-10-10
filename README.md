# Simulação de Controle de Tráfego com FreeRTOS
## Descrição do Projeto
Este projeto simula o controle de tráfego de veículos em 4 cruzamentos utilizando o sistema operacional em tempo real FreeRTOS. Cada cruzamento possui semáforos que controlam o fluxo de veículos nas direções Norte-Sul e Leste-Oeste, com tempos de travessia diferentes para cada cruzamento.

## Funcionalidades
* Controle de Semáforos: Cada cruzamento tem seu ciclo de semáforo que alterna entre verde, amarelo e vermelho para as direções Norte-Sul e Leste-Oeste.
* Simulação de Veículos: Veículos se aproximam de cada cruzamento e aguardam a liberação do semáforo para atravessar. Após a travessia, novos veículos se aproximam em intervalos de tempo aleatórios.
* Ciclos de Semáforo Variáveis: Cada cruzamento tem diferentes durações para os estados de semáforo (verde, amarelo, vermelho) nas duas direções.
## Estrutura do Código
O código é dividido em três partes principais:

* Funções de Controle de Semáforo (TrafficLightTask): Controlam os ciclos de luz verde, amarela e vermelha para cada cruzamento. Os tempos de cada fase são ajustados de acordo com os valores definidos na inicialização.
* Funções de Simulação de Veículos (NorthSouthVehicleTask e EastWestVehicleTask): Simulam veículos que se aproximam dos semáforos, esperam a luz verde e realizam a travessia.
* Inicialização e Scheduler (main): Inicializa os semáforos, define os tempos de cada cruzamento e cria as tarefas do FreeRTOS para gerenciar os semáforos e os veículos.
## Estrutura de Dados
* SemaphoreHandle_t nsSemaphore[4]: Semáforos que controlam o fluxo de veículos na direção Norte-Sul para os 4 cruzamentos.
* SemaphoreHandle_t ewSemaphore[4]: Semáforos que controlam o fluxo de veículos na direção Leste-Oeste.
* TrafficTimes crossroadTimes[4]: Estrutura que define os tempos de verde, amarelo e vermelho para cada cruzamento.
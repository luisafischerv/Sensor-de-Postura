# 📐 Sensor de Postura – Projeto CESAR School

Este repositório contém o desenvolvimento de um *sistema de detecção de postura* utilizando Arduino e o sensor MPU-6050. O objetivo é identificar quando o usuário permanece em má postura e registrar essas ocorrências para análise posterior através de uma interface web.


## 🚀 Objetivo do Projeto

- Monitorar a inclinação corporal em tempo real.  
- Detectar quando a postura foge do padrão considerado ideal.  
- Registrar alertas e disponibilizar visualização simples na web.  
- Servir como base para estudos de computação física e prototipação interativa.


## 🧩 Funcionamento Geral

1. O *MPU-6050* coleta dados de aceleração e rotação.  
2. O *Arduino* processa e envia esses valores continuamente pela porta serial.  
3. O *backend em Python (Flask)* recebe os dados, interpreta os ângulos e determina se houve má postura.  
4. A *interface web* exibe os alertas e registros recebidos pelo servidor.


## 🧠 Sobre o Código

### **Arduino (códigos dentro de /teste-dia-26)**
- Realiza a leitura do MPU-6050 usando a biblioteca apropriada (Wire.h ou similar).  
- Calcula valores de inclinação com base no acelerômetro e giroscópio.  
- Envia continuamente os dados via Serial.println() em um formato legível pelo servidor.  

Esses testes permitem ajustar:
- Sensibilidade  
- Threshold de inclinação  
- Frequência de envio  


### **Backend Python – server.py**
- Cria um servidor Flask que:
  - Lê dados da porta serial.  
  - Interpreta o ângulo corporal recebido.  
  - Define se a postura atual é *adequada* ou *inadequada* com base em limites configurados.  
  - Salva cada ocorrência de má postura em uma estrutura simples de dados.  
- Envia essas informações para a interface web a cada atualização.

O código serve como ponte entre o hardware e o front-end.


### **Interface Web – pasta /templates**
- Paginas HTML exibem:
  - Últimos dados recebidos  
  - Lista de alertas  
  - Estado atual da postura  
- O layout é simples, permitindo futuras melhorias como gráficos ou histórico detalhado.

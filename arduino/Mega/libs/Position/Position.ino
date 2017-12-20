float ax; // Aceleração no eixo x calculada pelo acelerômetro
float ay; // Aceleração no eixo y calculada pelo acelerômetro
float vx = 0; // Valor da velocidade no eixo x (primeira integral) com valor inicial igual a 0
float vy = 0; // Valor da velocidade no eixo y (primeira integral) com valor inicial igual a 0
float x = 0; // Valor da posição no eixo x (segunda integral) com valor inicial igual a 0
float y = 0; // Valor da posição no eixo y (segunda integral) com valor inicial igual a 0
float ax_prev = 0; // Valor prévio da aceleração no eixo x com valor inicial igual a 0
float ay_prev = 0; // Valor prévio da acleeração no eixo y com valor inicial igual a 0
float vx_prev = 0; // Valor prévio da velocidade no eixo x com valor inicial igual a 0
float vy_prev = 0; // Valor prévio da velocidade no eixo y com valor inicial igual a 0

unsigned long currTime; // Tempo atual
unsigned long prevTime = millis();// Tempo da última leitura

void loop() {
    
  currTime = millis();

  vx += (1/2)*(ax + ax_prev)*(currTime - prevTime)/1000; // 1000 adicionado para obter as mesmas unidades
  ax_prev = ax;
    
  vy += (1/2)*(ay + ay_prev)*(currTime - prevTime)/1000;
  ay_prev = ay;
  
  x += (1/2)*(vx + vx_prev)*(currTime - prevTime)/1000;
  vx_prev = vx;
  
  y += (1/2)*(vy + vy_prev)*(currTime - prevTime)/1000;
  vy_prev = vy;

  prevTime = currTime;
  
}

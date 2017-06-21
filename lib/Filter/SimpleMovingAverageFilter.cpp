#include "SimpleMovingAverageFilter.hpp"
#include <stdlib.h>
#include <string.h>

template<unsigned char SamplesCapacity>
SimpleMovingAverageFilter<SamplesCapacity>::SimpleMovingAverageFilter() :
  m_SamplesCount(0),
  m_Position(0),
  m_Sum(0)
{
  memset(m_pSamples, 0, SamplesCapacity * sizeof(float));
  //m_pSamples = (float*)calloc (m_SamplesCapacity, sizeof(float));
}

template<unsigned char SamplesCapacity>
SimpleMovingAverageFilter<SamplesCapacity>::~SimpleMovingAverageFilter()
{
  delete [] m_pSamples;
  //free(m_pSamples);
}

template<unsigned char SamplesCapacity>
void SimpleMovingAverageFilter<SamplesCapacity>::clear()
{
  m_Sum = 0;
  m_SamplesCount = 0;
  m_Position = 0;
  /*
  Da forma que foi implementado isso não é necessário,
  mas é uma boa prática limpar tudo. Manter ou não?
  */
  //memset(m_pSamples[0], 0,  sizeof(float) * m_SamplesCapacity);
}

template<unsigned char SamplesCapacity>
float SimpleMovingAverageFilter<SamplesCapacity>::getInput()
{
  /*
  Incrementa a quantidade de leituras (amostras) que temos agora
  e limita pra que não saia dos limites do array.
  */
  if(++m_SamplesCount > SamplesCapacity)
  {
    m_SamplesCount = SamplesCapacity;
    m_Sum -= m_pSamples[m_Position];
  }


  // Faz a leitura substituindo o valor mais antigo no array de leituras.
  m_pSamples[m_Position] = m_pInputSource->getInput();

  // Adiciona ao somatório
  m_Sum += m_pSamples[m_Position];

  /*
  Incrementa a próxima posição a ser substituida (agora está é a leitura mais antiga).
  Se isso for além dos limites do array, volta pra primeira (ping-pong clamp).
  */
  if(++m_Position >= SamplesCapacity)
  {
    m_Position = 0;
  }

  // Calcula a média dividindo pelo número de leituras
  return m_Sum / m_SamplesCount;
}

#include "AStar.hpp"
#include "../Logger/Logger.hpp"

Vector2<unsigned char> neighbors[] =
{
  Vector2<unsigned char>(-1, -1),
  Vector2<unsigned char>(-1, 0),
  Vector2<unsigned char>(-1, -1),
  Vector2<unsigned char>(0, -1),
  Vector2<unsigned char>(1, -1),
  Vector2<unsigned char>(0, 1),
  Vector2<unsigned char>(1, 1),
  Vector2<unsigned char>(0, 1)
};

void AStar::compute(Matrix<float>* pMap, Matrix<Vector2<unsigned char>>* pCameFrom, Vector2<unsigned char> start, Vector2<unsigned char> goal)
{
  PriorityQueue<Vector2<unsigned char>> frontier;
  frontier.push(start, 0);

  /*
  Se o valor na matriz for diferente de -1, a posição é considerada como visitada.
  Fiz isso pra evitar que fosse necessário alocar mais uma matriz/array.
  */
  pMap->set(start.getY(), start.getX(), 0);
  pCameFrom->set(start.getY(), start.getX(), start);

  while (!frontier.isEmpty())
  {
    /*
    Aqui não precisa verificar se o retorno é valido, pois só será executado se,
    e somente se tiver algum valor para retornar.
    */
    auto current = frontier.pop().getValue();
    if (current == goal)
      break;

    for (unsigned char i = 0; i < 8; i++)
    {
      auto next = neighbors[i] + current;

      Checked<float> newCost = pMap->get(current.getY(), current.getX());
      Checked<float> nextCost = pMap->get(next.getY(), next.getX());

      /*
      Por algum motivo saímos dos limites da matrix.
      */
      if(!newCost.IsValid())
      {
        Log->verbose("Buscando fora do mapa, x=%d, y=%d", current.getX(), current.getY());
        continue;
      }
      else if(!nextCost.IsValid() == -255)
      {
        Log->verbose("Buscando fora do mapa, x=%d, y=%d", next.getX(), next.getY());
        continue;
      }

      if (nextCost > -1 || newCost < nextCost)
      {
        pMap->set(next.getY(), next.getX(), newCost);
        frontier.push(next, newCost + heuristicCost(goal, next));
        pCameFrom->set(next.getY(), next.getX(), current);
      }
    }
  }
}

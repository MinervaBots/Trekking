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

void AStar::compute(Matrix<float> map, Matrix<Vector2<unsigned char>> cameFrom, Vector2<unsigned char> start, Vector2<unsigned char> goal)
{
  PriorityQueue<Vector2<unsigned char>> frontier;
  frontier.push(start, 0);

  /*
  Se o valor na matriz for diferente de -1, a posição é considerada como visitada.
  Fiz isso pra evitar que fosse necessário alocar mais uma matriz/array.
  */
  map.set(start.getY(), start.getX(), 0);
  cameFrom.set(start.getY(), start.getX(), start);

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

      Checked<float> newCost = map.get(current.getY(), current.getX());
      Checked<float> nextCost = map.get(next.getY(), next.getX());

      /*
      Por algum motivo saímos dos limites da matrix.
      */
      if(!newCost.IsValid())
      {
        CurrentLogger->writeLine("Buscando fora do mapa, x=%d, y=%d", current.getX(), current.getY());
      }
      else if(!nextCost.IsValid() == -255)
      {
        CurrentLogger->writeLine("Buscando fora do mapa, x=%d, y=%d", next.getX(), next.getY());
      }

      if (nextCost > -1 || newCost < nextCost)
      {
        map.set(next.getY(), next.getX(), newCost);
        frontier.push(next, newCost + heuristicCost(goal, next));
        cameFrom.set(next.getY(), next.getX(), current);
      }
    }
  }
}

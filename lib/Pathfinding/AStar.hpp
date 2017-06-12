#ifndef AStar_hpp
#define AStar_hpp

#include "../Algebra/Matrix.hpp"
#include "../Algebra/Vector2.hpp"
#include "../Collections/PriorityQueue.hpp"



class AStar
{
public:
  void compute(Matrix<float> map, Matrix<Vector2<unsigned char>> cameFrom, Vector2<unsigned char> start, Vector2<unsigned char> goal);

private:
  static inline float heuristicCost(Vector2<unsigned char> pos1, Vector2<unsigned char> pos2)
  {
    return abs(pos1.getX() - pos2.getX()) + abs(pos1.getY() - pos2.getY());
  }
};

/*
def reconstruct_path(came_from, start, goal):
    current = goal
    path = [current]
    while current != start:
        current = came_from[current]
        path.append(current)
    path.append(start) # optional
    path.reverse() # optional
    return path
*/

#endif //AStar_hpp

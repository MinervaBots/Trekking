#ifndef TrekkingSensoring_hpp
#define TrekkingSensoring_hpp

#include "../Algebra/Vector2.hpp"
#include "../CustomTypes/Tuple.hpp"
#include "../InputSource/InputSource.hpp"
#include "../Position/Position.hpp"

typedef Tuple<bool, float, Vector2<float>> TrekkingOdometry;

/*
Essa classe é uma interface. Classes que implementem essa interface, devem ser
responsáveis por obter todas as informações que a biblioteca Trekking
necessita para operar.

Ela também vai ser responsável por manter uma referência de positionamento,
seja através de um encoder ou acelerômetro.
*/
class TrekkingSensoring : public InputSource<TrekkingOdometry>
{
public:
  /*
  Retorna se a cor branca foi detectada, a linha de referência para 'search'
  e um vetor de direção caso algo seja detectado pelos sensores de obstáculo (ultrassom, IR).
  */
  virtual TrekkingOdometry getInput() = 0;

  virtual Position getPosition() const = 0;
};

#endif //TrekkingSensoring_hpp

#ifndef RANDOM_H
#define RANDOM_H

namespace Utils {
class Random {
 public:
  static float ZeroToOne();
  static float ZeroToN(float n);
  static float RandomRange(float low, float high);
};

} // namespace Utils

#endif /* RANDOM_H */

//на каждой остановке в автобус входит а и выхобит b человек. определить, сколько останется человек в атобусе после всех остановок
//дан массив пар {a, b}; 

#include <utility>
#include <vector>

unsigned int number(const std::vector<std::pair<int, int>>& busStops){
  unsigned int ans = 0;
  for(const auto &[a, b] : busStops){
    ans += a;
    ans -= b;
  }
  return ans;
}

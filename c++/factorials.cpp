//Example: n = 86240 should return "(2**5)(5)(7**2)(11)"


#include <string>
#include <sstream>
#include <map>
#include <cmath>


class PrimeDecomp
{
public:
     static std::string factors(int lst) {
        std::map<int, int> primeFactors;
        
        for (int i = 2; i <= sqrt(lst); ++i) {
            while (lst % i == 0) {
                primeFactors[i]++;
                lst /= i;
            }
        }
        if (lst > 1) {
            primeFactors[lst]++;
        }

        
        std::ostringstream result;
        for (const auto &factor : primeFactors) {
            result << "(" << factor.first;
            if (factor.second > 1) {
                result << "**" << factor.second;
            }
            result << ")";
        }

        return result.str();
    }
      
};

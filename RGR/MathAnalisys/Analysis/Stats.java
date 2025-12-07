import java.util.*;

public class Stats {

    public static StatsResult calcAll(List<Double> data) {
        StatsResult r = new StatsResult();

        if (data == null) {
            r.n = 0;
            return r;
        }

        r.n = data.size();
        if (r.n == 0) {
            return r;
        }

        // Сортируем копию для медианы/квартилей
        List<Double> sorted = new ArrayList<>(data);
        Collections.sort(sorted);

        r.min = sorted.get(0);
        r.max = sorted.get(sorted.size() - 1);
        r.range = r.max - r.min;

        // Среднее
        double sum = 0.0;
        for (double v : data) sum += v;
        r.mean = sum / r.n;

        // Медиана
        if (r.n % 2 == 1) {
            r.median = sorted.get(r.n / 2);
        } else {
            r.median = (sorted.get(r.n / 2 - 1) + sorted.get(r.n / 2)) / 2.0;
        }

        // Квартили
        r.q1 = percentile(sorted, 25.0);
        r.q3 = percentile(sorted, 75.0);

        // Мода (по округлённым значениям)
        Map<Integer, Integer> freq = new HashMap<>();
        for (double v : data) {
            int key = (int) Math.round(v);
            freq.put(key, freq.getOrDefault(key, 0) + 1);
        }
        int modeVal = 0;
        int modeCount = -1;
        for (Map.Entry<Integer, Integer> e : freq.entrySet()) {
            if (e.getValue() > modeCount) {
                modeCount = e.getValue();
                modeVal = e.getKey();
            }
        }
        r.mode = modeVal;

        // Дисперсия и стандартное отклонение (выборочные)
        double sumSq = 0.0;
        for (double v : data) {
            double d = v - r.mean;
            sumSq += d * d;
        }

        if (r.n > 1) {
            r.variance = sumSq / (r.n - 1);
            r.stdDev = Math.sqrt(r.variance);

            // Стандартная ошибка среднего и коэффициент вариации
            r.stdError = r.stdDev / Math.sqrt(r.n);
            r.coeffVar = (r.mean != 0.0) ? r.stdDev / r.mean : Double.NaN;
        } else {
            // При n = 1 выборочная дисперсия не определена
            r.variance = 0.0;
            r.stdDev = 0.0;
            r.stdError = 0.0;
            r.coeffVar = Double.NaN;
        }

        // Асимметрия и эксцесс
        // Суммы центральных моментов 3-го и 4-го порядка
        double sum3 = 0.0; // Σ (x-mean)^3
        double sum4 = 0.0; // Σ (x-mean)^4

        for (double v : data) {
            double d = v - r.mean;
            double d2 = d * d;
            sum3 += d2 * d;
            sum4 += d2 * d2;
        }

        // Нормированные центральные моменты
        double m3 = sum3 / r.n;
        double m4 = sum4 / r.n;

        double s2 = r.variance;
        double s = r.stdDev;

        // По умолчанию (если не хватает данных)
        r.skewness = 0.0;
        r.kurtosis = 0.0;

        // Выборочная асимметрия (с поправкой)
        if (s > 0 && r.n > 2) {
            double n = (double) r.n;
            r.skewness = (Math.sqrt(n * (n - 1)) / (n - 2)) *
                         (m3 / (s * s * s));
        }

        // ✅ Исправленный выборочный ИЗБЫТОЧНЫЙ эксцесс
        // Формула типа Excel KURT:
        // G2 = [n(n+1)/((n-1)(n-2)(n-3))] * [Σ d^4 / s^4]
        //      - [3(n-1)^2/((n-2)(n-3))]
        //
        // ВАЖНО: здесь нужна сумма Σ d^4, а не m4.
        if (s > 0 && r.n > 3) {
            double n = (double) r.n;

            // Защита от редких численных случаев
            if (s2 > 0) {
                double term1 = (n * (n + 1) * sum4) /
                               ((n - 1) * (n - 2) * (n - 3) * s2 * s2);
                double term2 = (3.0 * (n - 1) * (n - 1)) /
                               ((n - 2) * (n - 3));
                r.kurtosis = term1 - term2;
            }
        }

        return r;
    }

    // Процентиль p (0..100) по отсортированному списку
    private static double percentile(List<Double> sorted, double p) {
        if (sorted == null || sorted.isEmpty()) return Double.NaN;
        if (p <= 0) return sorted.get(0);
        if (p >= 100) return sorted.get(sorted.size() - 1);

        // Линейная интерполяция по позиции 0..(n-1)
        double pos = (p / 100.0) * (sorted.size() - 1);
        int i = (int) Math.floor(pos);
        int j = (int) Math.ceil(pos);

        if (i == j) return sorted.get(i);

        double w = pos - i;
        return sorted.get(i) * (1 - w) + sorted.get(j) * w;
    }
}

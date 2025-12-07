package x_y_comparising;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class CorrelationAnalysis {
    
    //корреляция Пирсона м/у Х и У
    public static CorrelationResult pearson(List<StudentRecord> rec){
        int n = rec.size();
        if (n < 3) throw new IllegalArgumentException("Необхолдимо как минимум 3 объекта для анализа");
        double sumX = 0.0;
        double sumY = 0.0;
        double sumX2 = 0.0;
        double sumY2 = 0.0;
        double sumXY = 0.0;

        for(StudentRecord r: rec){
            double x = r.getEducationCode();
            double y = r.getAverageScore();
            sumX += x;
            sumY += y;
            sumX2 += x * x;
            sumY2 += y * y;
            sumXY += x * y;
        }

        double numerator = n * sumXY - sumX * sumY;
        double denomX = n * sumX2 - sumX * sumX;
        double denomY = n * sumY2 - sumY * sumY;

        double denominator = Math.sqrt(denomX * denomY);
        double r = numerator / denominator;

        // t-статистика для проверки значимости r
        double t = r * Math.sqrt(n - 2) / Math.sqrt(1 - r * r);

        return new CorrelationResult(r, n, t);
    }

    //ранговая корреляция Спирмена
    public static double spearman(List<StudentRecord> records) {
        int n = records.size();
        double[] x = new double[n];
        double[] y = new double[n];

        for (int i = 0; i < n; i++) {
            StudentRecord r = records.get(i);
            x[i] = r.getEducationCode();
            y[i] = r.getAverageScore();
        }

        double[] rx = ranks(x);
        double[] ry = ranks(y);

        return pearson(rx, ry);
    }

    private static double pearson(double[] x, double[] y) {
        int n = x.length;
        double sumX = 0.0, sumY = 0.0, sumX2 = 0.0, sumY2 = 0.0, sumXY = 0.0;

        for (int i = 0; i < n; i++) {
            sumX += x[i];
            sumY += y[i];
            sumX2 += x[i] * x[i];
            sumY2 += y[i] * y[i];
            sumXY += x[i] * y[i];
        }

        double numerator = n * sumXY - sumX * sumY;
        double denomX = n * sumX2 - sumX * sumX;
        double denomY = n * sumY2 - sumY * sumY;
        return numerator / Math.sqrt(denomX * denomY);
    }

    // Построение рангов с учётом связей (tie) – средний ранг для одинаковых значений
    private static double[] ranks(double[] values) {
        int n = values.length;

        List<Integer> indices = new ArrayList<>(n);
        for (int i = 0; i < n; i++) {
            indices.add(i);
        }

        indices.sort(Comparator.comparingDouble(i -> values[i]));

        double[] ranks = new double[n];
        int i = 0;
        while (i < n) {
            int j = i;
            // ищем блок одинаковых значений
            while (j + 1 < n && values[indices.get(j + 1)] == values[indices.get(i)]) {
                j++;
            }
            // средний ранг для блока [i..j]
            double rank = (i + 1 + j + 1) / 2.0;
            for (int k = i; k <= j; k++) {
                ranks[indices.get(k)] = rank;
            }
            i = j + 1;
        }

        return ranks;
    }
}

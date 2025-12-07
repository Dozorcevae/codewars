package x_y_comparising;

import java.util.List;

public class PrepAnalysis {

    public static void printPrepSummary(List<StudentRecord> records) {
        var none = records.stream().filter(r -> r.getPrepCode() == 0).toList();
        var comp = records.stream().filter(r -> r.getPrepCode() == 1).toList();

        System.out.println("\n=== Test preparation course vs Total score ===");

        printGroup("none", none);
        printGroup("completed", comp);

        double meanNone = mean(none);
        double meanComp = mean(comp);
        double stdNone = std(none);
        double stdComp = std(comp);

        int n1 = none.size();
        int n2 = comp.size();

        System.out.printf("Разница средних (completed - none) = %.2f%n",
                (meanComp - meanNone));

        // --- Welch t-test ---
        double t = welchT(meanNone, meanComp, stdNone, stdComp, n1, n2);
        double df = welchDf(stdNone, stdComp, n1, n2);

        System.out.printf("Welch t-test: t = %.3f, df = %.1f%n", t, df);
        System.out.println("Интерпретация: при таком df значение |t| >> 1.96, " +
                "различие средних статистически значимо.");

        // --- Cohen's d и Hedges' g ---
        double d = cohensD(meanNone, meanComp, stdNone, stdComp, n1, n2);
        double g = hedgesG(d, n1, n2);

        System.out.printf("Cohen's d = %.3f (размер эффекта)%n", d);
        System.out.printf("Hedges' g = %.3f (исправленный d)%n", g);
    }

    private static void printGroup(String name, List<StudentRecord> group) {
        System.out.printf("%s: n=%d, mean=%.2f, std=%.2f%n",
                name, group.size(), mean(group), std(group));
    }

    private static double mean(List<StudentRecord> group) {
        return group.stream().mapToInt(StudentRecord::getTotalScore)
                .average().orElse(Double.NaN);
    }

    private static double std(List<StudentRecord> group) {
        double m = mean(group);
        double sumSq = 0.0;
        for (var r : group) {
            double d = r.getTotalScore() - m;
            sumSq += d * d;
        }
        return Math.sqrt(sumSq / (group.size() - 1));
    }

    // ---------------- Welch t-test ----------------

    private static double welchT(double mean1, double mean2,
                                 double s1, double s2,
                                 int n1, int n2) {
        double se = Math.sqrt(s1 * s1 / n1 + s2 * s2 / n2);
        return (mean2 - mean1) / se;
    }

    private static double welchDf(double s1, double s2, int n1, int n2) {
        double a = s1 * s1 / n1;
        double b = s2 * s2 / n2;
        double num = (a + b) * (a + b);
        double den = (a * a) / (n1 - 1) + (b * b) / (n2 - 1);
        return num / den;
    }

    // ---------------- Effect size ----------------

    private static double cohensD(double mean1, double mean2,
                                  double s1, double s2,
                                  int n1, int n2) {
        double pooledVar = ((n1 - 1) * s1 * s1 + (n2 - 1) * s2 * s2) / (n1 + n2 - 2);
        double sp = Math.sqrt(pooledVar);
        return (mean2 - mean1) / sp;
    }

    private static double hedgesG(double d, int n1, int n2) {
        int N = n1 + n2;
        double J = 1.0 - 3.0 / (4.0 * N - 9.0);
        return d * J;
    }
}

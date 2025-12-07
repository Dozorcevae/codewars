

public class Correlation {

    public static double pearson(double[] x, double[] y) {
        int n = x.length;
        if (n != y.length || n < 2) return Double.NaN;

        double meanX = 0, meanY = 0;
        for (int i = 0; i < n; i++) {
            meanX += x[i];
            meanY += y[i];
        }
        meanX /= n;
        meanY /= n;

        double sxx = 0, syy = 0, sxy = 0;
        for (int i = 0; i < n; i++) {
            double dx = x[i] - meanX;
            double dy = y[i] - meanY;
            sxx += dx * dx;
            syy += dy * dy;
            sxy += dx * dy;
        }

        double denom = Math.sqrt(sxx * syy);
        if (denom == 0) return Double.NaN;

        return sxy / denom;
    }

    public static double spearman(double[] x, double[] y) {
        int n = x.length;
        if (n != y.length || n < 2) return Double.NaN;

        double[] rx = RankUtils.rank(x);
        double[] ry = RankUtils.rank(y);

        return pearson(rx, ry);
    }

    // t-статистика для проверки значимости корреляции
    public static double tForCorrelation(double r, int n) {
        if (Double.isNaN(r) || n < 3) return Double.NaN;
        double r2 = r * r;
        if (r2 >= 1.0) return Double.POSITIVE_INFINITY;
        return r * Math.sqrt((n - 2.0) / (1.0 - r2));
    }
}


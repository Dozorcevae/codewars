package x_y_comparising;


public class CorrelationResult {
    private final double r;          // коэффициент корреляции Пирсона
    private final int n;             // объём выборки
    private final double tStatistic; // t для проверки значимости

    public CorrelationResult(double r, int n, double tStatistic) {
        this.r = r;
        this.n = n;
        this.tStatistic = tStatistic;
    }

    public double getR() {
        return r;
    }

    public int getN() {
        return n;
    }

    public double getTStatistic() {
        return tStatistic;
    }
}



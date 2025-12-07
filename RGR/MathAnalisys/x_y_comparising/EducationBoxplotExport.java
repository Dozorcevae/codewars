package x_y_comparising;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;
import java.util.stream.Collectors;

public class EducationBoxplotExport {

    // Одна строка итоговой таблицы
    public static class Row {
        public final int code;
        public final String label;
        public final int n;
        public final double min, q1, median, q3, max;
        public final double mean, std;

        public Row(int code, String label, int n,
                   double min, double q1, double median, double q3, double max,
                   double mean, double std) {
            this.code = code;
            this.label = label;
            this.n = n;
            this.min = min;
            this.q1 = q1;
            this.median = median;
            this.q3 = q3;
            this.max = max;
            this.mean = mean;
            this.std = std;
        }
    }

    // Главный метод: построить строки сводки
    public static List<Row> buildSummary(List<StudentRecord> records) {
        Map<Integer, List<Double>> groups = records.stream()
                .collect(Collectors.groupingBy(
                        StudentRecord::getEducationCode,
                        Collectors.mapping(StudentRecord::getAverageScore, Collectors.toList())
                ));

        List<Row> rows = new ArrayList<>();

        for (int code = 1; code <= 6; code++) {
            List<Double> ys = groups.getOrDefault(code, List.of());
            if (ys.isEmpty()) continue;

            double[] arr = ys.stream().mapToDouble(Double::doubleValue).sorted().toArray();

            int n = arr.length;
            double min = arr[0];
            double max = arr[n - 1];

            double median = median(arr);

            // Tukey-style квартиль:
            // нижняя половина и верхняя половина
            double[] lower = Arrays.copyOfRange(arr, 0, n / 2);
            double[] upper = Arrays.copyOfRange(arr, (n + 1) / 2, n);

            double q1 = median(lower);
            double q3 = median(upper);

            double mean = mean(arr);
            double std = std(arr, mean);

            rows.add(new Row(code, labelFor(code), n, min, q1, median, q3, max, mean, std));
        }

        return rows;
    }

    // Экспорт сводной таблицы
    public static void exportSummaryCsv(List<StudentRecord> records, Path out) throws IOException {
        List<Row> rows = buildSummary(records);

        StringBuilder sb = new StringBuilder();
        sb.append("code,label,n,min,q1,median,q3,max,mean,std\n");

        for (Row r : rows) {
            sb.append(r.code).append(",")
              .append(escape(r.label)).append(",")
              .append(r.n).append(",")
              .append(fmt(r.min)).append(",")
              .append(fmt(r.q1)).append(",")
              .append(fmt(r.median)).append(",")
              .append(fmt(r.q3)).append(",")
              .append(fmt(r.max)).append(",")
              .append(fmt(r.mean)).append(",")
              .append(fmt(r.std)).append("\n");
        }

        Files.writeString(out, sb.toString(), StandardCharsets.UTF_8);
    }

    // Экспорт "широкой" таблицы сырых значений Y:
    // 6 столбцов, каждый — значения Average_Y по группе
    public static void exportWideValuesCsv(List<StudentRecord> records, Path out) throws IOException {
        Map<Integer, List<Double>> groups = records.stream()
                .collect(Collectors.groupingBy(
                        StudentRecord::getEducationCode,
                        Collectors.mapping(StudentRecord::getAverageScore, Collectors.toList())
                ));

        // Упорядочим
        for (int code = 1; code <= 6; code++) {
            groups.computeIfAbsent(code, k -> new ArrayList<>());
            groups.get(code).sort(Double::compareTo);
        }

        int maxLen = groups.values().stream().mapToInt(List::size).max().orElse(0);

        StringBuilder sb = new StringBuilder();

        // header
        sb.append("some high school,high school,some college,associate's degree,bachelor's degree,master's degree\n");

        for (int i = 0; i < maxLen; i++) {
            for (int code = 1; code <= 6; code++) {
                List<Double> list = groups.get(code);
                if (i < list.size()) sb.append(fmt(list.get(i)));
                if (code < 6) sb.append(",");
            }
            sb.append("\n");
        }

        Files.writeString(out, sb.toString(), StandardCharsets.UTF_8);
    }

    // --------- helpers ---------

    private static String labelFor(int code) {
        return switch (code) {
            case 1 -> "some high school";
            case 2 -> "high school";
            case 3 -> "some college";
            case 4 -> "associate's degree";
            case 5 -> "bachelor's degree";
            case 6 -> "master's degree";
            default -> "unknown";
        };
    }

    private static String escape(String s) {
        // простая защита CSV
        if (s.contains(",") || s.contains("\"")) {
            return "\"" + s.replace("\"", "\"\"") + "\"";
        }
        return s;
    }

    private static String fmt(double v) {
        // точка как разделитель — Numbers обычно понимает
        return String.format(Locale.US, "%.4f", v);
    }

    private static double mean(double[] arr) {
        double sum = 0;
        for (double v : arr) sum += v;
        return sum / arr.length;
    }

    private static double std(double[] arr, double mean) {
        if (arr.length < 2) return 0;
        double ss = 0;
        for (double v : arr) {
            double d = v - mean;
            ss += d * d;
        }
        return Math.sqrt(ss / (arr.length - 1));
    }

    private static double median(double[] arr) {
        if (arr.length == 0) return Double.NaN;
        int n = arr.length;
        if (n % 2 == 1) return arr[n / 2];
        return (arr[n / 2 - 1] + arr[n / 2]) / 2.0;
    }
}

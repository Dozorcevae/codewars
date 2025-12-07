package x_y_comparising;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class GroupAnalysis {

    public static Map<Integer, GroupSummary> byParentalEducation(List<StudentRecord> records) {
        Map<Integer, List<Double>> groups = new HashMap<>();
        Map<Integer, String> labels = new HashMap<>();

        for (StudentRecord r : records) {
            int code = r.getEducationCode();
            groups.computeIfAbsent(code, k -> new ArrayList<>())
                  .add(r.getAverageScore());

            labels.putIfAbsent(code, r.getParentalEducationText());
        }

        Map<Integer, GroupSummary> result = new TreeMap<>();

        for (Map.Entry<Integer, List<Double>> entry : groups.entrySet()) {
            int code = entry.getKey();
            List<Double> ys = entry.getValue();
            int n = ys.size();

            double sum = 0.0;
            for (double v : ys) sum += v;
            double mean = sum / n;

            double sumSq = 0.0;
            for (double v : ys) {
                double d = v - mean;
                sumSq += d * d;
            }
            double std = Math.sqrt(sumSq / (n - 1));

            String label = labels.get(code);
            result.put(code, new GroupSummary(code, label, n, mean, std));
        }

        return result;
    }
}


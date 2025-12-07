import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.*;

public class RgrStudentsMain {

    public static void main(String[] args) {
        String csvPath = "StudentsPerformance.csv";

        List<StudentRecord> records;
        try {
            records = readCsv(csvPath);
        } catch (IOException e) {
            System.err.println("Ошибка чтения файла: " + e.getMessage());
            return;
        }
        
        System.out.println("Прочитано записей: " + records.size());

        printYHistogram(records);

        // Собираем X и Y
        List<Double> yValues = new ArrayList<>();
        List<Integer> xValues = new ArrayList<>();
        for (StudentRecord r : records) {
            yValues.add(r.avgScore);          // Y – средний балл
            xValues.add(r.parentEduCode);     // X – образование родителей (код)
        }
        saveYValuesToCsv(yValues);
        saveXYToCsv(records);
        // Описательная статистика для Y
        StatsResult statsY = Stats.calcAll(yValues);

        System.out.println("=== Описательная статистика для Y (средний балл) ===");
        System.out.printf("n = %d%n", statsY.n);
        System.out.printf("min = %.2f%n", statsY.min);
        System.out.printf("max = %.2f%n", statsY.max);
        System.out.printf("Размах = %.2f%n", statsY.range);
        System.out.printf("Среднее = %.2f%n", statsY.mean);
        System.out.printf("Мода = %.2f%n", statsY.mode);
        System.out.printf("Медиана = %.2f%n", statsY.median);
        System.out.printf("Дисперсия (выборочная) = %.4f%n", statsY.variance);
        System.out.printf("Ст. отклонение = %.4f%n", statsY.stdDev);
        System.out.printf("Стандартная ошибка mean = %.4f%n", statsY.stdError);
        System.out.printf("Коэфф. вариации = %.2f%%%n", statsY.coeffVar * 100.0);
        System.out.printf("Асимметрия = %.4f%n", statsY.skewness);
        System.out.printf("Эксцесс (избыточный) = %.4f%n", statsY.kurtosis);
        System.out.printf("Q1 = %.2f, медиана = %.2f, Q3 = %.2f%n",
                statsY.q1, statsY.median, statsY.q3);

    }

    private static void saveXYToCsv(java.util.List<StudentRecord> records) {
    try (java.io.PrintWriter out = new java.io.PrintWriter("XY_values.csv", java.nio.charset.StandardCharsets.UTF_8)) {
        out.println("parentEdu,parentEduCode,Y");
        for (StudentRecord r : records) {
            out.printf(java.util.Locale.US,
                    "\"%s\",%d,%.4f%n",
                    r.parentEduRaw, r.parentEduCode, r.avgScore);
        }
        System.out.println("Файл XY_values.csv сохранён");
    } catch (java.io.IOException e) {
        System.err.println("Не удалось записать XY_values.csv: " + e.getMessage());
    }
    }

    private static void saveYValuesToCsv(List<Double> yValues) {
    try (PrintWriter out = new PrintWriter("Y_values.csv", StandardCharsets.UTF_8)) {
        out.println("Y");
        for (double y : yValues) {
            out.printf(Locale.US, "%.4f%n", y); // точка в качестве разделителя
        }
        System.out.println("Файл Y_values.csv сохранён");
    } 
    catch (IOException e) {
        System.err.println("Не удалось записать Y_values.csv: " + e.getMessage());
        }
    }
    private static List<StudentRecord> readCsv(String path) throws IOException {
        List<StudentRecord> list = new ArrayList<>();

        try (BufferedReader br = Files.newBufferedReader(Paths.get(path))) {
            String line = br.readLine(); // пропускаем заголовок

            while ((line = br.readLine()) != null) {
                if (line.trim().isEmpty()) continue;
                StudentRecord rec = StudentRecord.fromCsvLine(line);
                if (rec != null) {
                    list.add(rec);
                }
            }
        }

        return list;
    }
    // Печать гистограммы для Y по интервалам [10;20), [20;30), ..., [90;100]
private static void printYHistogram(java.util.List<StudentRecord> records) {
  
    double[] edges = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    int m = edges.length - 1;       // число интервалов = 9
    int[] counts = new int[m];      // частоты

    for (StudentRecord r : records) {
        double y = r.avgScore;

        // если вдруг что-то < 10, отнесём к первому интервалу
        if (y < edges[0]) {
            counts[0]++;
            continue;
        }

        for (int i = 0; i < m; i++) {
            double left = edges[i];
            double right = edges[i + 1];

            boolean inInterval;
            if (i < m - 1) {
                // все, кроме последнего: [left; right)
                inInterval = (y >= left && y < right);
            } else {
                // последний интервал: [left; right] включительно
                inInterval = (y >= left && y <= right);
            }

            if (inInterval) {
                counts[i]++;
                break;
            }
        }
    }

    int n = records.size();

    System.out.println("=== Гистограмма среднего балла Y по интервалам ===");
    System.out.println("Интервал\tЧастота\tОтносительная частота");
    for (int i = 0; i < m; i++) {
        double left = edges[i];
        double right = edges[i + 1];

        String bracketRight = (i == m - 1) ? "]" : ")"; // последний интервал включительно

        double rel = (double) counts[i] / n;

        System.out.printf(Locale.US,
                "[%.0f; %.0f%s\t%d\t%.4f%n",
                left, right, bracketRight, counts[i], rel);
    }

    System.out.println();
    }



}

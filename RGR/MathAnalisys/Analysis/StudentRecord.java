import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

/**
 * Представление строки из StudentsPerformance.csv.
 * Хранит исходные поля и несколько производных значений,
 * которые используются остальной программой.
 */
public class StudentRecord {
    public final String gender;
    public final String raceEthnicity;
    public final String parentEduRaw;
    public final String lunch;
    public final String testPreparation;
    public final int mathScore;
    public final int readingScore;
    public final int writingScore;
    public final double avgScore;
    public final int parentEduCode;

    private static final Map<String, Integer> PARENT_EDU_CODES = new HashMap<>();

    static {
        PARENT_EDU_CODES.put("some high school", 1);
        PARENT_EDU_CODES.put("high school", 2);
        PARENT_EDU_CODES.put("some college", 3);
        PARENT_EDU_CODES.put("associate's degree", 4);
        PARENT_EDU_CODES.put("bachelor's degree", 5);
        PARENT_EDU_CODES.put("master's degree", 6);
    }

    private StudentRecord(String gender,
                          String raceEthnicity,
                          String parentEduRaw,
                          String lunch,
                          String testPreparation,
                          int mathScore,
                          int readingScore,
                          int writingScore,
                          double avgScore,
                          int parentEduCode) {
        this.gender = gender;
        this.raceEthnicity = raceEthnicity;
        this.parentEduRaw = parentEduRaw;
        this.lunch = lunch;
        this.testPreparation = testPreparation;
        this.mathScore = mathScore;
        this.readingScore = readingScore;
        this.writingScore = writingScore;
        this.avgScore = avgScore;
        this.parentEduCode = parentEduCode;
    }

    public static StudentRecord fromCsvLine(String line) {
        if (line == null || line.isBlank()) {
            return null;
        }

        List<String> tokens = splitCsv(line);
        if (tokens.size() < 8) {
            System.err.println("Строка CSV содержит меньше 8 столбцов: " + line);
            return null;
        }

        try {
            String gender = tokens.get(0);
            String race = tokens.get(1);
            String parentEdu = tokens.get(2);
            String lunch = tokens.get(3);
            String testPrep = tokens.get(4);

            int math = parseScore(tokens.get(5));
            int reading = parseScore(tokens.get(6));
            int writing = parseScore(tokens.get(7));

            double avg = (math + reading + writing) / 3.0;
            int parentCode = mapParentEducation(parentEdu);

            return new StudentRecord(
                    gender,
                    race,
                    parentEdu,
                    lunch,
                    testPrep,
                    math,
                    reading,
                    writing,
                    avg,
                    parentCode
            );
        } catch (NumberFormatException e) {
            System.err.println("Не удалось распарсить числа в строке: " + line);
            return null;
        }
    }

    private static int parseScore(String token) {
        String trimmed = token == null ? "" : token.trim();
        if (trimmed.isEmpty()) {
            throw new NumberFormatException("Пустое значение балла");
        }
        return Integer.parseInt(trimmed);
    }

    private static int mapParentEducation(String parentEduRaw) {
        if (parentEduRaw == null) {
            return 0;
        }
        String key = parentEduRaw.trim().toLowerCase(Locale.ROOT);
        return PARENT_EDU_CODES.getOrDefault(key, 0);
    }

    private static List<String> splitCsv(String line) {
        List<String> tokens = new ArrayList<>();
        StringBuilder current = new StringBuilder();
        boolean inQuotes = false;

        for (int i = 0; i < line.length(); i++) {
            char ch = line.charAt(i);
            if (ch == '"') {
                // Двойные кавычки внутри поля экранируются двумя символами '"'
                if (inQuotes && i + 1 < line.length() && line.charAt(i + 1) == '"') {
                    current.append('"');
                    i++; // пропускаем второй символ
                } else {
                    inQuotes = !inQuotes;
                }
            } else if (ch == ',' && !inQuotes) {
                tokens.add(current.toString());
                current.setLength(0);
            } else {
                current.append(ch);
            }
        }

        tokens.add(current.toString());

        for (int i = 0; i < tokens.size(); i++) {
            tokens.set(i, tokens.get(i).trim());
        }

        return tokens;
    }
}

package x_y_comparising;

import java.io.BufferedReader;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

public class DataLowder {

    public static List<StudentRecord> load(Path csvPath) throws IOException {
        List<StudentRecord> result = new ArrayList<>();

        try (BufferedReader br = Files.newBufferedReader(csvPath, StandardCharsets.UTF_8)) {
            String header = br.readLine(); // заголовок

            String line;
            while ((line = br.readLine()) != null) {
                List<String> fields = parseCsvLine(line);

                if (fields.size() != 8) {
                    throw new IllegalStateException(
                            "Unexpected column count: " + fields.size() + " in line: " + line);
                }

                String gender = normalize(fields.get(0));
                String race = normalize(fields.get(1));
                String parentalEdu = normalize(fields.get(2));
                String lunch = normalize(fields.get(3));
                String testPrep = normalize(fields.get(4));

                int math = Integer.parseInt(normalize(fields.get(5)));
                int reading = Integer.parseInt(normalize(fields.get(6)));
                int writing = Integer.parseInt(normalize(fields.get(7)));

                result.add(new StudentRecord(
                        gender, race, parentalEdu, lunch, testPrep,
                        math, reading, writing
                ));
            }
        }

        return result;
    }

    // Парсер CSV с учётом кавычек
    private static List<String> parseCsvLine(String line) {
        List<String> out = new ArrayList<>();
        StringBuilder sb = new StringBuilder();
        boolean inQuotes = false;

        for (int i = 0; i < line.length(); i++) {
            char c = line.charAt(i);

            if (c == '"') {
                // экранированная кавычка внутри quoted-поля: ""
                if (inQuotes && i + 1 < line.length() && line.charAt(i + 1) == '"') {
                    sb.append('"');
                    i++;
                } else {
                    inQuotes = !inQuotes;
                }
            } else if (c == ',' && !inQuotes) {
                out.add(sb.toString());
                sb.setLength(0);
            } else {
                sb.append(c);
            }
        }
        out.add(sb.toString());

        return out;
    }

    // Нормализация поля: убираем кавычки и пробелы
    private static String normalize(String s) {
        if (s == null) return "";
        String t = s.trim();

        // на всякий случай убираем обрамление
        if (t.length() >= 2 && t.startsWith("\"") && t.endsWith("\"")) {
            t = t.substring(1, t.length() - 1);
        }

        // и любые оставшиеся "
        t = t.replace("\"", "").trim();
        return t;
    }
}

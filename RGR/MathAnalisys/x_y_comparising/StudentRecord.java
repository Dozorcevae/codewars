package x_y_comparising;

public class StudentRecord {

    private final String gender;
    private final String group;
    private final String parentalEducationText;
    private final String lunch;
    private final String testPreparation;
    private final int mathScore;
    private final int readingScore;
    private final int writingScore;

    private final int educationCode;   // 1..6
    private final double averageScore; // 0..100

    private final int prepCode;   // 0/1
    private final int totalScore; // 0..300

    public StudentRecord(String gender,
                         String group,
                         String parentalEducationText,
                         String lunch,
                         String testPreparation,
                         int mathScore,
                         int readingScore,
                         int writingScore) {
        this.gender = gender;
        this.group = group;
        this.parentalEducationText = parentalEducationText;
        this.lunch = lunch;
        this.testPreparation = testPreparation;
        this.mathScore = mathScore;
        this.readingScore = readingScore;
        this.writingScore = writingScore;

        this.educationCode = encodeEducation(parentalEducationText);
        this.averageScore = (mathScore + readingScore + writingScore) / 3.0;
        this.totalScore = mathScore + readingScore + writingScore;
        this.prepCode = encodePrep(testPreparation);
    }

    private static int encodePrep(String text) {
        return switch (text) {
            case "none" -> 0;
            case "completed" -> 1;
            default -> throw new IllegalArgumentException("Unknown test prep: " + text);
        };
    }

    private static int encodeEducation(String text) {
        return switch (text) {
            case "some high school" -> 1;
            case "high school" -> 2;
            case "some college" -> 3;
            case "associate's degree" -> 4;
            case "bachelor's degree" -> 5;
            case "master's degree" -> 6;
            default -> throw new IllegalArgumentException("Unknown parental education: " + text);
        };
    }

    public int getPrepCode() { return prepCode; }
    public int getTotalScore() { return totalScore; }

    public String getParentalEducationText() { return parentalEducationText; }

    public int getEducationCode() { return educationCode; }
    public double getAverageScore() { return averageScore; }
}

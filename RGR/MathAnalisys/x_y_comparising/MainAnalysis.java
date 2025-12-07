package x_y_comparising;

import java.io.IOException;
import java.nio.file.Path;
import java.util.Map;


public class MainAnalysis {
    public static void main(String[] args) throws IOException {
        Path csvPath = Path.of("Students.csv");

        var records = DataLowder.load(csvPath);
        System.out.println("прочитано записей: " + records.size());

        EducationBoxplotExport.exportSummaryCsv(records, Path.of("education_boxplot_summary.csv"));
        EducationBoxplotExport.exportWideValuesCsv(records, Path.of("education_y_wide.csv"));

        //корреляция пирсона
        
        PrepAnalysis.printPrepSummary(records);

        var corr = CorrelationAnalysis.pearson(records);

        System.out.printf("Корреляция Пирсона: r=%.4f%n", corr.getR());
        System.out.printf("t-статистика для проверки значимости r t = %.3f (n = %d)%n", 
            corr.getTStatistic(), corr.getN());
        double rSpearman = CorrelationAnalysis.spearman(records);
        System.out.printf("Ранговая корреляция Спирмена rs = %.4f%n", rSpearman);

        // --- Средние Y по уровням образования родителей ---
        Map<Integer, GroupSummary> groups = GroupAnalysis.byParentalEducation(records);
        System.out.println("\nСредний балл Y по уровням образования родителей:");
        System.out.println("Код\tОбразование\t\t n\tMean(Y)\tStd(Y)");
        for (GroupSummary g : groups.values()) {
            System.out.printf("%d\t%-20s\t %3d\t%7.2f\t%7.2f%n",
                    g.getCode(),
                    g.getLabel(),
                    g.getCnt(),
                    g.getMean(),
                    g.getStd());
        }
    }
}


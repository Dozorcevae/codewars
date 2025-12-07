package x_y_comparising;

public class GroupSummary {
    private final int code;     //1...6
    private final String label; //"титульник" образования, чтобы понимать что происходит
    private final int cnt;      // число учеников в группе
    private final double mean;  //средний Y
    private final double std;   //среднее отклонение

    public GroupSummary(
        int code,
        String label,
        int cnt, 
        double mean, 
        double std) 
    {
        this.code = code;
        this.label = label;
        this.cnt = cnt;
        this.mean = mean;
        this.std = std;
    }

    public int     getCode()  {return code;}
    public String  getLabel() {return label;}
    public int     getCnt()   {return cnt;}   
    public double  getMean()  {return mean;}
    public double  getStd()   {return std;}
}

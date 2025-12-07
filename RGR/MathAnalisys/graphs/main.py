import pandas as pd
import matplotlib.pyplot as plt

# 1) читаем данные
df = pd.read_csv("Students.csv")

# 2) считаем нужные переменные
df["Average_Y"] = (df["math score"] + df["reading score"] + df["writing score"]) / 3.0
df["Total_score"] = df["math score"] + df["reading score"] + df["writing score"]

# 3) порядок категорий образования родителей
edu_order = [
    "some high school",
    "high school",
    "some college",
    "associate's degree",
    "bachelor's degree",
    "master's degree"
]

# ---------- Рисунок 1: boxplot Average_Y по образованию родителей ----------
data_avg = [df.loc[df["parental level of education"] == cat, "Average_Y"].dropna().values
            for cat in edu_order]

plt.figure()
plt.boxplot(data_avg, labels=edu_order, showfliers=True)
plt.xticks(rotation=25, ha="right")
plt.ylabel("Average exam score (Y)")
plt.title("Average Y by parental education")
plt.tight_layout()
plt.savefig("fig_5_1_boxplot_education_avg.png", dpi=200)
plt.close()

# ---------- Рисунок 2: средние Average_Y по X с ошибками ±SD ----------
means = [df.loc[df["parental level of education"] == cat, "Average_Y"].mean() for cat in edu_order]
stds  = [df.loc[df["parental level of education"] == cat, "Average_Y"].std(ddof=1) for cat in edu_order]

plt.figure()
plt.bar(edu_order, means, yerr=stds)
plt.xticks(rotation=25, ha="right")
plt.ylabel("Mean Average_Y")
plt.title("Mean Y by parental education (±SD)")
plt.tight_layout()
plt.savefig("fig_5_2_bar_education_avg_sd.png", dpi=200)
plt.close()

# ---------- Рисунок 3: scatter X–Y (код образования vs Average_Y) ----------
edu_code = {name: i+1 for i, name in enumerate(edu_order)}
df["Education_code"] = df["parental level of education"].map(edu_code)

plt.figure()
plt.scatter(df["Education_code"], df["Average_Y"], s=10)
plt.xlabel("Parental education code (1..6)")
plt.ylabel("Average exam score (Y)")
plt.title("Scatter X–Y (parental education vs Average_Y)")
plt.tight_layout()
plt.savefig("fig_5_3_scatter_xy.png", dpi=200)
plt.close()

# ---------- Рисунок 4: boxplot Total_score по test preparation ----------
prep_order = ["none", "completed"]
data_total = [df.loc[df["test preparation course"] == cat, "Total_score"].dropna().values
              for cat in prep_order]

plt.figure()
plt.boxplot(data_total, labels=prep_order, showfliers=True)
plt.ylabel("Total score (math+reading+writing)")
plt.title("Total score by test preparation course")
plt.tight_layout()
plt.savefig("fig_5_4_boxplot_prep_total.png", dpi=200)
plt.close()

# ---------- Рисунок 5 (опционально): средние Total_score с ±SD ----------
means_t = [df.loc[df["test preparation course"] == cat, "Total_score"].mean() for cat in prep_order]
stds_t  = [df.loc[df["test preparation course"] == cat, "Total_score"].std(ddof=1) for cat in prep_order]

plt.figure()
plt.bar(prep_order, means_t, yerr=stds_t)
plt.ylabel("Mean Total_score")
plt.title("Mean Total score by test prep (±SD)")
plt.tight_layout()
plt.savefig("fig_5_5_bar_prep_total_sd.png", dpi=200)
plt.close()

print("Готово! PNG-файлы сохранены рядом со скриптом.")

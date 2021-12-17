import numpy as np
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score

def report_performance(y_actual, y_predicted):
    false_negative = np.array([1 if (y_predicted[i] == 0 and y_actual[i] == 1) else 0 for i in range(len(y_predicted))]).sum()
    false_positive = np.array([1 if (y_predicted[i] == 1 and y_actual[i] == 0) else 0 for i in range(len(y_predicted))]).sum()
    true_positive = np.array([1 if (y_predicted[i] == 1 and y_actual[i] == 1) else 0 for i in range(len(y_predicted))]).sum()
    true_negative = np.array([1 if (y_predicted[i] == 0 and y_actual[i] == 0) else 0 for i in range(len(y_predicted))]).sum()
    print(f"train accuracy: {accuracy_score(y_actual, y_predicted)*100}%")
    print(f"precision: {precision_score(y_actual, y_predicted)*100}%")
    print(f"recall: {recall_score(y_actual, y_predicted)*100}%")
    print(f"f1 score: {f1_score(y_actual, y_predicted)*100}%")
    print(f"false discovery: {false_positive*100/(false_positive+ true_positive)}%")
    print(f"specificity: {true_negative*100/(true_negative+false_positive)}%")

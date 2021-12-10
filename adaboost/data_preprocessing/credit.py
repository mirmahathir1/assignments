import pandas as pd
from sklearn.preprocessing import StandardScaler
from sklearn.feature_selection import mutual_info_classif
import random
random.seed(42)
def load(column_count = 25):
    dataframe = pd.read_csv('data/credit/creditcard.csv')
    dataframe.head(5)
    #%%
    # for col in dataframe:
    #     for i in range(len(dataframe[col])):
    #         try:
    #             float(dataframe[col][i])
    #         except ValueError as e:
    #             print(f"found missing value {dataframe[col][i]} in {col} column, {i} row")
    #             exit(1)
    #%% md
    ## dropping column Time
    #%%
    dataframe = dataframe.drop('Time',axis = 1)
    #%% md
    ## scale Amount column
    #%%
    amount_column = dataframe['Amount'].to_numpy().reshape((dataframe.shape[0],1))
    scaler = StandardScaler()
    amount_column_numpy = scaler.fit_transform(amount_column)
    dataframe['Amount'] = amount_column_numpy
    #%%
    rows_with_class_1 = dataframe[dataframe['Class']==1]
    rows_with_class_0 = dataframe[dataframe['Class']==0]
    #%% md
    ## Fix Data class distribution
    #%%
    print(f"row count with class 1: {rows_with_class_1.shape[0]}")
    print(f"row count with class 0: {rows_with_class_0.shape[0]}")
    factor = rows_with_class_0.shape[0] // rows_with_class_1.shape[0]
    print(f"0 classes are {factor} times more than 1 classes")
    #%%
    random_indices = random.sample(range(len(rows_with_class_0)),20000)
    #%%
    undersamples_for_0 = rows_with_class_0.iloc[random_indices]
    #%%
    dataframe = rows_with_class_1.append(undersamples_for_0)
    #%%
    dataframe.groupby(['Class']).size().reset_index(name='counts')
    #%%
    print(f"Dataframe columns: {dataframe.columns}")
    #%%
    target_column = "Class"
    X_dataframe = dataframe.drop(target_column, inplace=False, axis=1)
    Y_dataframe = dataframe[target_column]
    #%%
    information_gains = []
    for column in X_dataframe.columns:
        single_column = X_dataframe[column].to_numpy()
        single_column = single_column.reshape((single_column.shape[0], 1))
        information_gain = mutual_info_classif(single_column, Y_dataframe, random_state=42)[0]
        information_gains.append({
            "column": column,
            "information": information_gain
        })

    sorted_attributes = sorted(information_gains, key=lambda d: d['information'], reverse=True)

    column_count = min(column_count, len(X_dataframe.columns))

    keeping_attributes = []
    for info in sorted_attributes[:column_count]:
        keeping_attributes.append(info["column"])
    #%%
    print(f"length of keeping attributes: {len(keeping_attributes)}")
    #%%
    X_dataframe = X_dataframe[keeping_attributes]
    #%%
    X_dataframe['ones'] = 1
    #%%
    X_numpy = X_dataframe.to_numpy()
    Y_numpy = Y_dataframe.to_numpy().reshape((Y_dataframe.shape[0],1))
    print(f"X_numpy shape: {X_numpy.shape}")
    print(f"Y_numpy shape: {Y_numpy.shape}")
    return X_numpy, Y_numpy
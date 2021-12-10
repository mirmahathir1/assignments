#%%
import pandas as pd
from io import StringIO
from sklearn.feature_selection import mutual_info_classif
from sklearn.preprocessing import StandardScaler
#%%
def load(column_count = 50):
    #%%
    data_file = open('data/adult/adult.data', mode='r')
    whole_csv = data_file.read()
    # the dataset consists of abundant useless whitespaces
    whole_csv = whole_csv.replace(' ','')
    data_file.close()

    #%%
    column_names = ["age","workclass","fnlwgt","education","education-num","marital-status","occupation","relationship","race","sex","capital-gain","capital-loss","hours-per-week","native-country","salary"]
    dataframe = pd.read_csv(StringIO(whole_csv), names=column_names)
    dataframe.head(5)
    #%% md
    ## workclass analysis
    #%%
    dataframe.groupby(['workclass']).size().reset_index(name='counts')
    #%% md
    ## workclass processing
    #%%
    dataframe['workclass'] = dataframe['workclass'].replace('?', 'UNK')
    dataframe.groupby(['workclass']).size().reset_index(name='counts')
    #%% md
    ## fnlwgt analysis
    #%%
    dataframe['fnlwgt'].describe()
    #%%
    for i in range(len(dataframe['fnlwgt'])):
        try:
            float(dataframe['fnlwgt'][i])
        except ValueError as e:
            print(f"found missing value {dataframe['fnlwgt'][i]} in fnlwgt column, {i} row")
    #%% md
    ## education analysis
    #%%
    dataframe.groupby(['education']).size().reset_index(name='counts')
    #%% md
    ## education-num analysis
    #%%
    dataframe['education-num'].describe()
    #%%
    for i in range(len(dataframe['education-num'])):
        try:
            float(dataframe['education-num'][i])
        except ValueError as e:
            print(f"found missing value {dataframe['education-num'][i]} in education-num column, {i} row")
    #%% md
    ## marital-status analysis
    #%%
    dataframe.groupby(['marital-status']).size().reset_index(name='counts')
    #%% md
    ## occupation analysis
    #%%
    dataframe.groupby(['occupation']).size().reset_index(name='counts')
    #%% md
    ## occupation processing
    #%%
    dataframe['occupation'] = dataframe['occupation'].replace('?', 'UNK')
    dataframe.groupby(['occupation']).size().reset_index(name='counts')
    #%% md
    ## relationship analysis
    #%%
    dataframe.groupby(['relationship']).size().reset_index(name='counts')
    #%%
    ## race analysis
    dataframe.groupby(['race']).size().reset_index(name='counts')
    #%% md
    ## sex analysis
    #%%
    dataframe.groupby(['sex']).size().reset_index(name='counts')
    #%% md
    ## capital-gain analysis
    #%%
    dataframe['capital-gain'].describe()
    #%%
    col = 'capital-gain'
    for i in range(len(dataframe[col])):
        try:
            float(dataframe[col][i])
        except ValueError as e:
            print(f"found missing value {dataframe[col][i]} in {col} column, {i} row")

    #%% md
    ## capital-loss analysis
    #%%
    dataframe['capital-loss'].describe()
    #%%
    col = 'capital-loss'
    for i in range(len(dataframe[col])):
        try:
            float(dataframe[col][i])
        except ValueError as e:
            print(f"found missing value {dataframe[col][i]} in {col} column, {i} row")
    #%% md
    ## hours-per-week analysis
    #%%
    dataframe['hours-per-week'].describe()
    #%%
    col = 'hours-per-week'
    for i in range(len(dataframe[col])):
        try:
            float(dataframe[col][i])
        except ValueError as e:
            print(f"found missing value {dataframe[col][i]} in {col} column, {i} row")

    #%% md
    ## native-country analysis
    #%%
    dataframe.groupby(['native-country']).size().reset_index(name='counts')
    #%%
    dataframe['native-country'] = dataframe['native-country'].replace('?', 'UNK')
    dataframe.groupby(['native-country']).size().reset_index(name='counts')
    #%% md
    ## salary analysis
    #%%
    dataframe.groupby(['salary']).size().reset_index(name='counts')
    #%% md
    # Statistics
    #%%
    print(f"Number of rows: {dataframe.shape[0]}")
    print(f"Dataframe original column count: {len(dataframe.columns)}")
    print(f"Dataframe column list: {dataframe.columns}")
    print("Column data types: ")
    for col in dataframe.columns:
        print(f"{col} column dtype: {dataframe[col].dtype}")
    #%%
    for col in dataframe.columns:
        if dataframe[col].dtype == 'int64':
            continue
        one_hot_encoded = pd.get_dummies(dataframe[col], prefix=col, prefix_sep='_', drop_first=True, dtype=int)
        dataframe = dataframe.join(one_hot_encoded)
        dataframe = dataframe.drop(col,axis = 1)
    #%%
    print(f"current column count: {len(dataframe.columns)}")
    print(f"current column names: {dataframe.columns}")
    #%%
    target_column = "salary_>50K"
    X_dataframe = dataframe.drop(target_column, inplace=False, axis=1)
    Y_dataframe = dataframe[target_column]
    #%%
    # checking whether all data is numeric
    for col in X_dataframe:
        for i in range(len(X_dataframe[col])):
            try:
                float(X_dataframe[col][i])
            except ValueError as e:
                print(X_dataframe[col][i] == ' ')
                print(f"found missing value {X_dataframe[col][i]} in {col} column, {i} row")
                exit(1)
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
    keeping_attributes = []
    for info in sorted_attributes[:column_count]:
        keeping_attributes.append(info["column"])
    #%%
    print(f"length of keeping attributes: {len(keeping_attributes)}")
    # keeping_attributes
    #%%
    X_dataframe = X_dataframe[keeping_attributes]
    #%%
    len(X_dataframe.columns)
    #%%
    # add bias column
    X_dataframe['ones'] = 1
    #%%
    # conversion to numpy array and scaling
    X_numpy = X_dataframe.to_numpy()
    Y_numpy = Y_dataframe.to_numpy().reshape((Y_dataframe.shape[0],1))
    scaler = StandardScaler()
    X_numpy = scaler.fit_transform(X_numpy)
    #%%
    print(f"X_numpy shape: {X_numpy.shape}")
    print(f"Y_numpy shape: {Y_numpy.shape}")
    return X_numpy, Y_numpy
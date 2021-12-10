# %%
import argparse
from sklearn.model_selection import train_test_split
from numpy.random import choice
import numpy as np
from sklearn.preprocessing import LabelEncoder
from sklearn.preprocessing import StandardScaler
import pandas as pd
from io import StringIO
from sklearn.feature_selection import mutual_info_classif
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score
import random

parser = argparse.ArgumentParser(description='Argument parser')
parser.add_argument('--rounds', type=int, help='Number of rounds for adaboost')
parser.add_argument('--dataset', type=str, help='Number of rounds for adaboost')
parser.add_argument('--epochs', type=int, help='Number of epochs for training')
parser.add_argument('--feature_count', type=int, help='Number of features to train on')
parser.add_argument('--learning_rate', type=float, help='Learning rate of algorithm')

args = parser.parse_args()
if not args.rounds:
    print("Number of rounds for adaboost required")
    exit(0)

if not args.dataset:
    print("Name of dataset required")
    exit(0)

if not args.epochs:
    print("Number of epochs is required")
    exit(0)

if not args.feature_count:
    print("Number of feature count is required")
    exit(0)

if not args.learning_rate:
    print("Learning rate is required")
    exit(0)

datasets_available = ['adult', 'credit', 'telco']

if args.dataset not in datasets_available:
    print("Dataset not recognized")

print(f"Number of rounds for adaboost: {args.rounds}")
print(f"Name of dataset: {args.dataset}")

np.random.seed(seed=42)
random.seed(42)


def adult_data_load(column_count=50):
    # %%
    data_file = open('data/adult/adult.data', mode='r')
    whole_csv = data_file.read()
    # the dataset consists of abundant useless whitespaces
    whole_csv = whole_csv.replace(' ', '')
    data_file.close()

    # %%
    column_names = ["age", "workclass", "fnlwgt", "education", "education-num", "marital-status", "occupation",
                    "relationship", "race", "sex", "capital-gain", "capital-loss", "hours-per-week", "native-country",
                    "salary"]
    dataframe = pd.read_csv(StringIO(whole_csv), names=column_names)
    dataframe.head(5)
    # %% md
    ## workclass analysis
    # %%
    dataframe.groupby(['workclass']).size().reset_index(name='counts')
    # %% md
    ## workclass processing
    # %%
    dataframe['workclass'] = dataframe['workclass'].replace('?', 'UNK')
    dataframe.groupby(['workclass']).size().reset_index(name='counts')
    # %% md
    ## fnlwgt analysis
    # %%
    dataframe['fnlwgt'].describe()
    # %%
    for i in range(len(dataframe['fnlwgt'])):
        try:
            float(dataframe['fnlwgt'][i])
        except ValueError as e:
            print(f"found missing value {dataframe['fnlwgt'][i]} in fnlwgt column, {i} row")
    # %% md
    ## education analysis
    # %%
    dataframe.groupby(['education']).size().reset_index(name='counts')
    # %% md
    ## education-num analysis
    # %%
    dataframe['education-num'].describe()
    # %%
    for i in range(len(dataframe['education-num'])):
        try:
            float(dataframe['education-num'][i])
        except ValueError as e:
            print(f"found missing value {dataframe['education-num'][i]} in education-num column, {i} row")
    # %% md
    ## marital-status analysis
    # %%
    dataframe.groupby(['marital-status']).size().reset_index(name='counts')
    # %% md
    ## occupation analysis
    # %%
    dataframe.groupby(['occupation']).size().reset_index(name='counts')
    # %% md
    ## occupation processing
    # %%
    dataframe['occupation'] = dataframe['occupation'].replace('?', 'UNK')
    dataframe.groupby(['occupation']).size().reset_index(name='counts')
    # %% md
    ## relationship analysis
    # %%
    dataframe.groupby(['relationship']).size().reset_index(name='counts')
    # %%
    ## race analysis
    dataframe.groupby(['race']).size().reset_index(name='counts')
    # %% md
    ## sex analysis
    # %%
    dataframe.groupby(['sex']).size().reset_index(name='counts')
    # %% md
    ## capital-gain analysis
    # %%
    dataframe['capital-gain'].describe()
    # %%
    col = 'capital-gain'
    for i in range(len(dataframe[col])):
        try:
            float(dataframe[col][i])
        except ValueError as e:
            print(f"found missing value {dataframe[col][i]} in {col} column, {i} row")

    # %% md
    ## capital-loss analysis
    # %%
    dataframe['capital-loss'].describe()
    # %%
    col = 'capital-loss'
    for i in range(len(dataframe[col])):
        try:
            float(dataframe[col][i])
        except ValueError as e:
            print(f"found missing value {dataframe[col][i]} in {col} column, {i} row")
    # %% md
    ## hours-per-week analysis
    # %%
    dataframe['hours-per-week'].describe()
    # %%
    col = 'hours-per-week'
    for i in range(len(dataframe[col])):
        try:
            float(dataframe[col][i])
        except ValueError as e:
            print(f"found missing value {dataframe[col][i]} in {col} column, {i} row")

    # %% md
    ## native-country analysis
    # %%
    dataframe.groupby(['native-country']).size().reset_index(name='counts')
    # %%
    dataframe['native-country'] = dataframe['native-country'].replace('?', 'UNK')
    dataframe.groupby(['native-country']).size().reset_index(name='counts')
    # %% md
    ## salary analysis
    # %%
    dataframe.groupby(['salary']).size().reset_index(name='counts')
    # %% md
    # Statistics
    # %%
    print(f"Number of rows: {dataframe.shape[0]}")
    print(f"Dataframe original column count: {len(dataframe.columns)}")
    print(f"Dataframe column list: {dataframe.columns}")
    print("Column data types: ")
    for col in dataframe.columns:
        print(f"{col} column dtype: {dataframe[col].dtype}")
    # %%
    for col in dataframe.columns:
        if dataframe[col].dtype == 'int64':
            continue
        one_hot_encoded = pd.get_dummies(dataframe[col], prefix=col, prefix_sep='_', drop_first=True, dtype=int)
        dataframe = dataframe.join(one_hot_encoded)
        dataframe = dataframe.drop(col, axis=1)
    # %%
    print(f"current column count: {len(dataframe.columns)}")
    print(f"current column names: {dataframe.columns}")
    # %%
    target_column = "salary_>50K"
    X_dataframe = dataframe.drop(target_column, inplace=False, axis=1)
    Y_dataframe = dataframe[target_column]
    # %%
    # checking whether all data is numeric
    for col in X_dataframe:
        for i in range(len(X_dataframe[col])):
            try:
                float(X_dataframe[col][i])
            except ValueError as e:
                print(X_dataframe[col][i] == ' ')
                print(f"found missing value {X_dataframe[col][i]} in {col} column, {i} row")
                exit(1)
    # %%
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

    column_count = min(column_count, len(X_dataframe.columns))

    for info in sorted_attributes[:column_count]:
        keeping_attributes.append(info["column"])
    # %%
    print(f"length of keeping attributes: {len(keeping_attributes)}")
    # keeping_attributes
    # %%
    X_dataframe = X_dataframe[keeping_attributes]
    # %%
    len(X_dataframe.columns)
    # %%
    # add bias column
    X_dataframe['ones'] = 1
    # %%
    # conversion to numpy array and scaling
    X_numpy = X_dataframe.to_numpy()
    Y_numpy = Y_dataframe.to_numpy().reshape((Y_dataframe.shape[0], 1))
    scaler = StandardScaler()
    X_numpy = scaler.fit_transform(X_numpy)
    # %%
    print(f"X_numpy shape: {X_numpy.shape}")
    print(f"Y_numpy shape: {Y_numpy.shape}")
    return X_numpy, Y_numpy


def credit_data_load(column_count=25):
    dataframe = pd.read_csv('data/credit/creditcard.csv')
    dataframe.head(5)
    # %%
    # for col in dataframe:
    #     for i in range(len(dataframe[col])):
    #         try:
    #             float(dataframe[col][i])
    #         except ValueError as e:
    #             print(f"found missing value {dataframe[col][i]} in {col} column, {i} row")
    #             exit(1)
    # %% md
    ## dropping column Time
    # %%
    dataframe = dataframe.drop('Time', axis=1)
    # %% md
    ## scale Amount column
    # %%
    amount_column = dataframe['Amount'].to_numpy().reshape((dataframe.shape[0], 1))
    scaler = StandardScaler()
    amount_column_numpy = scaler.fit_transform(amount_column)
    dataframe['Amount'] = amount_column_numpy
    # %%
    rows_with_class_1 = dataframe[dataframe['Class'] == 1]
    rows_with_class_0 = dataframe[dataframe['Class'] == 0]
    # %% md
    ## Fix Data class distribution
    # %%
    print(f"row count with class 1: {rows_with_class_1.shape[0]}")
    print(f"row count with class 0: {rows_with_class_0.shape[0]}")
    factor = rows_with_class_0.shape[0] // rows_with_class_1.shape[0]
    print(f"0 classes are {factor} times more than 1 classes")
    # %%
    random_indices = random.sample(range(len(rows_with_class_0)), 20000)
    # %%
    undersamples_for_0 = rows_with_class_0.iloc[random_indices]
    # %%
    dataframe = rows_with_class_1.append(undersamples_for_0)
    # %%
    dataframe.groupby(['Class']).size().reset_index(name='counts')
    # %%
    print(f"Dataframe columns: {dataframe.columns}")
    # %%
    target_column = "Class"
    X_dataframe = dataframe.drop(target_column, inplace=False, axis=1)
    Y_dataframe = dataframe[target_column]
    # %%
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
    # %%
    print(f"length of keeping attributes: {len(keeping_attributes)}")
    # %%
    X_dataframe = X_dataframe[keeping_attributes]
    # %%
    X_dataframe['ones'] = 1
    # %%
    X_numpy = X_dataframe.to_numpy()
    Y_numpy = Y_dataframe.to_numpy().reshape((Y_dataframe.shape[0], 1))
    print(f"X_numpy shape: {X_numpy.shape}")
    print(f"Y_numpy shape: {Y_numpy.shape}")
    return X_numpy, Y_numpy

def telco_data_load(column_count=10):
    print("Load telco dataset")
    dataframe = pd.read_csv('data/telco-customer-churn/WA_Fn-UseC_-Telco-Customer-Churn.csv')

    dataframe.drop('customerID', axis=1, inplace=True)

    # process Total Charges
    total_charges = []
    for i in dataframe['TotalCharges']:
        if not i == ' ':
            total_charges.append(float(i))
    mean_of_total_charge = np.mean(total_charges)
    dataframe['TotalCharges'] = dataframe['TotalCharges'].replace(' ', mean_of_total_charge)

    # Process Gender
    gender_encoder = LabelEncoder()
    gender_encoder.fit(dataframe['gender'])
    dataframe["gender_encoded"] = gender_encoder.transform(dataframe['gender'])

    dataframe.drop('gender', inplace=True, axis=1)

    # Process Partner
    Partner_encoder = LabelEncoder()
    Partner_encoder.fit(dataframe['Partner'])
    dataframe["Partner_encoded"] = Partner_encoder.transform(dataframe['Partner'])
    dataframe.drop('Partner', inplace=True, axis=1)

    # Process Dependents
    Dependents_encoder = LabelEncoder()
    Dependents_encoder.fit(dataframe['Dependents'])
    dataframe["Dependents_encoded"] = Dependents_encoder.transform(dataframe['Dependents'])
    dataframe.drop(['Dependents'], inplace=True, axis=1)

    # Process PhoneService
    PhoneService_encoder = LabelEncoder()
    PhoneService_encoder.fit(dataframe['PhoneService'])
    dataframe["PhoneService_encoded"] = PhoneService_encoder.transform(dataframe['PhoneService'])
    dataframe.drop(['PhoneService'], inplace=True, axis=1)

    # Process MultipleLines
    dataframe['MultipleLines_No'] = dataframe['MultipleLines'].map({'No': 1, 'No phone service': 0, 'Yes': 0}).astype(
        int)
    dataframe['MultipleLines_No_phone_service'] = dataframe['MultipleLines'].map(
        {'No': 0, 'No phone service': 1, 'Yes': 0}).astype(int)
    dataframe.drop(['MultipleLines'], inplace=True, axis=1)

    # Process InternetService
    dataframe['InternetService_DSL'] = dataframe['InternetService'].map({'DSL': 1, 'Fiber optic': 0, 'No': 0}).astype(
        int)
    dataframe['InternetService_Fiber_optic'] = dataframe['InternetService'].map(
        {'DSL': 0, 'Fiber optic': 1, 'No': 0}).astype(int)
    dataframe.drop(['InternetService'], inplace=True, axis=1)

    # Process OnlineSecurity
    dataframe['OnlineSecurity_No'] = dataframe['OnlineSecurity'].map(
        {'No': 1, 'Yes': 0, 'No internet service': 0}).astype(int)
    dataframe['OnlineSecurity_Yes'] = dataframe['OnlineSecurity'].map(
        {'No': 0, 'Yes': 1, 'No internet service': 0}).astype(int)
    dataframe.drop(['OnlineSecurity'], inplace=True, axis=1)

    # Process OnlineBackup
    dataframe['OnlineBackup_No'] = dataframe['OnlineBackup'].map({'No': 1, 'Yes': 0, 'No internet service': 0}).astype(
        int)
    dataframe['OnlineBackup_Yes'] = dataframe['OnlineBackup'].map({'No': 0, 'Yes': 1, 'No internet service': 0}).astype(
        int)
    dataframe.drop('OnlineBackup', inplace=True, axis=1)

    # Process DeviceProtection
    dataframe['DeviceProtection_No'] = dataframe['DeviceProtection'].map(
        {'No': 1, 'Yes': 0, 'No internet service': 0}).astype(int)
    dataframe['DeviceProtection_Yes'] = dataframe['DeviceProtection'].map(
        {'No': 0, 'Yes': 1, 'No internet service': 0}).astype(int)
    dataframe.drop('DeviceProtection', inplace=True, axis=1)

    # Process TechSupport
    dataframe['TechSupport_No'] = dataframe['TechSupport'].map({'No': 1, 'Yes': 0, 'No internet service': 0}).astype(
        int)
    dataframe['TechSupport_Yes'] = dataframe['TechSupport'].map({'No': 0, 'Yes': 1, 'No internet service': 0}).astype(
        int)
    dataframe.drop('TechSupport', inplace=True, axis=1)

    # Process StreamingTV
    dataframe['StreamingTV_No'] = dataframe['StreamingTV'].map({'No': 1, 'Yes': 0, 'No internet service': 0}).astype(
        int)
    dataframe['StreamingTV_Yes'] = dataframe['StreamingTV'].map({'No': 0, 'Yes': 1, 'No internet service': 0}).astype(
        int)
    dataframe.drop('StreamingTV', inplace=True, axis=1)

    # Process StreamingMovies
    dataframe['StreamingMovies_No'] = dataframe['StreamingMovies'].map(
        {'No': 1, 'Yes': 0, 'No internet service': 0}).astype(int)
    dataframe['StreamingMovies_Yes'] = dataframe['StreamingMovies'].map(
        {'No': 0, 'Yes': 1, 'No internet service': 0}).astype(int)
    dataframe.drop('StreamingMovies', inplace=True, axis=1)

    # Process Contract
    dataframe['Contract_One_year'] = dataframe['Contract'].map(
        {'One year': 1, 'Two year': 0, 'Month-to-month': 0}).astype(int)
    dataframe['Contract_Two_year'] = dataframe['Contract'].map(
        {'One year': 0, 'Two year': 1, 'Month-to-month': 0}).astype(int)
    dataframe.drop('Contract', inplace=True, axis=1)

    # Process PaperlessBilling
    PaperlessBilling_encoder = LabelEncoder()
    PaperlessBilling_encoder.fit(dataframe['PaperlessBilling'])
    dataframe["PaperlessBilling_encoded"] = PaperlessBilling_encoder.transform(dataframe['PaperlessBilling'])
    dataframe.drop('PaperlessBilling', inplace=True, axis=1)

    # Process PaymentMethod
    dataframe['PaymentMethod_Bank_transfer'] = dataframe['PaymentMethod'].map(
        {'Bank transfer (automatic)': 1, 'Credit card (automatic)': 0, 'Electronic check': 0,
         'Mailed check': 0}).astype(int)
    dataframe['PaymentMethod_Credit_card'] = dataframe['PaymentMethod'].map(
        {'Bank transfer (automatic)': 0, 'Credit card (automatic)': 1, 'Electronic check': 0,
         'Mailed check': 0}).astype(int)
    dataframe['PaymentMethod_Electronic_check'] = dataframe['PaymentMethod'].map(
        {'Bank transfer (automatic)': 0, 'Credit card (automatic)': 0, 'Electronic check': 1,
         'Mailed check': 0}).astype(int)
    dataframe.drop('PaymentMethod', inplace=True, axis=1)

    # Process Churn
    Churn_encoder = LabelEncoder()
    Churn_encoder.fit(dataframe['Churn'])
    dataframe["Churn_encoded"] = Churn_encoder.transform(dataframe['Churn'])
    dataframe.drop('Churn', inplace=True, axis=1)

    X_dataframe = dataframe.drop("Churn_encoded", inplace=False, axis=1)
    Y_dataframe = dataframe["Churn_encoded"]

    for col in X_dataframe:
        for i in range(len(X_dataframe[col])):
            try:
                float(X_dataframe[col][i])
            except ValueError as e:
                print(X_dataframe[col][i] == ' ')
                print(f"found missing value {X_dataframe[col][i]} in {col} column, {i} row")
                exit(1)
    # %%
    information_gains = []
    for column in X_dataframe.columns:
        single_column = X_dataframe[column].to_numpy()
        single_column = single_column.reshape((single_column.shape[0], 1))
        information_gain = mutual_info_classif(single_column, Y_dataframe, random_state=42)[0]
        # print(f"information gain of {column}: {information_gain}")
        information_gains.append({
            "column": column,
            "information": information_gain
        })

    sorted_attributes = sorted(information_gains, key=lambda d: d['information'], reverse=True)
    # %%
    keeping_attributes = []

    column_count = min(column_count, len(X_dataframe.columns))

    for info in sorted_attributes[:column_count]:
        keeping_attributes.append(info["column"])
    X_dataframe = X_dataframe[keeping_attributes]

    # %%
    # add bias column
    X_dataframe['ones'] = 1

    X_numpy = X_dataframe.to_numpy()
    Y_numpy = Y_dataframe.to_numpy()
    scaler = StandardScaler()
    X_numpy = scaler.fit_transform(X_numpy)

    return (X_numpy, Y_numpy)


def report_performance(y_actual, y_predicted):
    false_negative = np.array(
        [1 if (y_predicted[i] == 0 and y_actual[i] == 1) else 0 for i in range(len(y_predicted))]).sum()
    false_positive = np.array(
        [1 if (y_predicted[i] == 1 and y_actual[i] == 0) else 0 for i in range(len(y_predicted))]).sum()
    true_positive = np.array(
        [1 if (y_predicted[i] == 1 and y_actual[i] == 1) else 0 for i in range(len(y_predicted))]).sum()
    true_negative = np.array(
        [1 if (y_predicted[i] == 0 and y_actual[i] == 0) else 0 for i in range(len(y_predicted))]).sum()
    print(f"train accuracy: {accuracy_score(y_actual, y_predicted) * 100}%")
    print(f"precision: {precision_score(y_actual, y_predicted) * 100}%")
    print(f"recall: {recall_score(y_actual, y_predicted) * 100}%")
    print(f"f1 score: {f1_score(y_actual, y_predicted) * 100}%")
    print(f"false discovery: {false_positive * 100 / (false_positive + true_positive)}%")
    print(f"specificity: {true_negative * 100 / (true_negative + false_positive)}%")


class LogisticRegression:
    def __init__(self):
        self.weights = []

    def loss(self, y, y_hat):
        loss = np.sum(np.square(np.subtract(y, y_hat))) / y.shape[0]
        return loss

    def scale_y(self, y):
        return np.array([1 if yi > 0 else -1 for yi in y]).reshape((y.shape[0], 1))

    def reverse_scale_y(self, y):
        return np.array([1 if single_prediction > 0 else 0 for single_prediction in y]).reshape((y.shape[0], 1))

    def fit(self, X, y, number_of_epochs, learning_rate):
        m, n = X.shape
        self.weights = np.random.uniform(low=0.0, high=1.0, size=(n, 1))
        y_original_scaled = self.scale_y(y)
        losses = []

        for epoch in range(number_of_epochs):
            y_hat = np.tanh(np.dot(X, self.weights))
            gradient = np.matmul(X.T, np.multiply(y_original_scaled - y_hat, 1 - y_hat ** 2))
            self.weights += learning_rate * gradient
            loss = self.loss(y_original_scaled, np.tanh(np.dot(X, self.weights)))
            # if loss < 0.5:
            #     print(f"early terminate at epoch {epoch}")
            #     break
            if epoch % 100 == 0:
                print(f"Epoch {epoch}. Loss= {loss}")
            losses.append(loss)

        return self.weights, losses

    def predict(self, X):
        predictions = np.tanh(np.dot(X, self.weights))
        return self.reverse_scale_y(predictions)


def main():
    data_loader = telco_data_load
    if args.dataset == 'telco':
        data_loader = telco_data_load
    elif args.dataset == 'credit':
        data_loader = credit_data_load
    elif args.dataset == 'adult':
        data_loader = adult_data_load
    else:
        print("dataset not found. Exiting program")
        exit(0)

    (X, Y) = data_loader(column_count=args.feature_count)
    X_train, X_test, y_train, y_test = train_test_split(X, Y, test_size=0.20, random_state=42)

    # %%
    K_ensembles = args.rounds  # number of hypotheses
    N_dataset_size = X_train.shape[0]
    W_data_weights = np.full(N_dataset_size, 1 / N_dataset_size)
    Z_model_weights = np.zeros((K_ensembles,))
    models = []

    for k in range(K_ensembles):
        sample_indices = choice(range(N_dataset_size), N_dataset_size,
                                p=W_data_weights)
        X_sample_train = X_train[sample_indices]
        Y_sample_train = y_train[sample_indices]

        logistic_regression = LogisticRegression()
        logistic_regression.fit(X_sample_train,
                                Y_sample_train,
                                number_of_epochs=args.epochs,
                                learning_rate=args.learning_rate)

        print(f"Train accuracy of model {k}: {accuracy_score(y_train, logistic_regression.predict(X_train))}")

        print(f"Test accuracy of model {k}: {accuracy_score(y_test, logistic_regression.predict(X_test))}")
        models.append(logistic_regression)
        y_hypothesis = logistic_regression.predict(X_train)
        error = 0
        for j in range(N_dataset_size):
            if not y_hypothesis[j] == y_train[j]:
                error += W_data_weights[j]
        print(f"error: {error}")
        if error > 0.5:
            print("skipping ensemble")
            continue

        for j in range(N_dataset_size):
            if y_hypothesis[j] == y_train[j]:
                W_data_weights[j] += ((W_data_weights[j] * error) / (1 - error))

        W_data_weights = W_data_weights / np.sum(W_data_weights)
        Z_model_weights[k] = np.log((1 - error) / error)

    # %%
    weighted_predictions_test = np.zeros((K_ensembles, X_test.shape[0]))
    for i in range(K_ensembles):
        Y_pred_test = models[i].predict(X_test)
        Y_pred_test = np.where(Y_pred_test == 0, -1, Y_pred_test) * Z_model_weights[i]
        weighted_predictions_test[i] = Y_pred_test.reshape((Y_pred_test.shape[0],))

    prediction_sums = np.sum(weighted_predictions_test, axis=0)
    predictions = np.where(prediction_sums > 0, 1, prediction_sums)
    Y_pred_test = np.where(prediction_sums <= 0, 0, predictions)

    print("_"*100)
    print("testing stats: ")
    report_performance(y_test, Y_pred_test)
    # %%
    weighted_predictions_train = np.zeros((K_ensembles, X_train.shape[0]))
    for i in range(K_ensembles):
        Y_pred_train = models[i].predict(X_train)
        Y_pred_train = np.where(Y_pred_train == 0, -1, Y_pred_train) * Z_model_weights[i]
        weighted_predictions_train[i] = Y_pred_train.reshape((Y_pred_train.shape[0],))

    prediction_sums = np.sum(weighted_predictions_train, axis=0)
    predictions = np.where(prediction_sums > 0, 1, prediction_sums)
    Y_pred_train = np.where(prediction_sums <= 0, 0, predictions)

    print("_" * 100)
    print("training accuracy: ")
    report_performance(y_train, Y_pred_train)

if __name__ == "__main__":
    main()
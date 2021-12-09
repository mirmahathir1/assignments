import numpy as np
import pandas as pd
from sklearn.preprocessing import LabelEncoder
from sklearn.preprocessing import StandardScaler
from sklearn.feature_selection import mutual_info_classif

def load(column_count):
    if not column_count:
        print("Please specify column count")
        exit(1)

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
        information_gain = mutual_info_classif(single_column, Y_dataframe)[0]
        # print(f"information gain of {column}: {information_gain}")
        information_gains.append({
            "column": column,
            "information": information_gain
        })

    sorted_attributes = sorted(information_gains, key=lambda d: d['information'], reverse=True)
    # %%
    keeping_attributes = []

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
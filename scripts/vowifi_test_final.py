import os
import pandas as pd

path = os.getcwd()
device_names = os.listdir(path)

vowifi_data = []

for device_name in device_names:  # device이름 순서대로 하나씩 진행
    device_path = os.path.join(path, device_name)
    experiment_information = os.listdir(device_path)  # result.init list

    for experiment_file in experiment_information:  # result.init 순서대로 하나씩 진행
        file_path = os.path.join(device_path, experiment_file)

        with open(file_path, "r") as file:
            lines = file.readlines()  # result.init의 각 줄을 읽음,list형태

        experiment_name = experiment_file.split(".")[1]
        experiment_id = experiment_file.split(".")[2]

        functional_oracle = None  # 없을 경우
        liveness_oracle = None  # 없을 경우

        for line in lines:
            if "Functional Oracle" in line:
                functional_oracle = line.split("Functional Oracle: ")[-1].strip()
            elif "Liveness Oracle" in line:
                liveness_oracle = line.split("Liveness Oracle: ")[-1].strip()

        vowifi_data.append(
            {  # append는 list에 가능
                "Device Name": device_name,
                "Experiment Name": experiment_name,
                "Experiment ID": experiment_id,
                "Functional Oracle": functional_oracle,
                "Liveness Oracle": liveness_oracle,
            }
        )

df = pd.DataFrame(vowifi_data)

print(df)

while True:
    selected_device = input("Select a device name (type 'exit' to quit): ")
    if selected_device == "exit":
        break

    if selected_device not in df["Device Name"].unique():
        print("Invalid device name. Please try again.")
        continue

    selected_df = df[df["Device Name"] == selected_device]  # device name의 해당하는 전체 정보

    oracle_type = input(
        "Choose oracle type - 'Functional' for Functional Oracle, 'Liveness' for Liveness Oracle (type 'exit' to quit): "
    )
    if oracle_type == "exit":
        break

    if oracle_type not in ["Functional", "Liveness"]:  # 잘 못 입력했을 때
        print("Invalid oracle type. Please try again.")
        continue

    oracle_value = input(
        "Choose oracle value - 'negative' or 'positive' (type 'exit' to quit): "
    )
    if oracle_value == "exit":
        break

    if oracle_value not in ["negative", "positive"]:  # 잘 못 입력했을 때
        print("Invalid oracle value. Please try again.")
        continue

    selected_oracle_df = selected_df[
        selected_df[f"{oracle_type} Oracle"] == oracle_value
    ]  # oracle value에 해당하는 df만 추림

    if selected_oracle_df.empty:  # oracle value가 존재하지 않을 때
        print(
            f"No experiments found for {selected_device} with {oracle_type} Oracle value {oracle_value}."
        )
    else:  # 고른 experiment name 과 id 보여줌
        print("Experiments with selected criteria:")
        print(selected_oracle_df[["Experiment Name", "Experiment ID"]])

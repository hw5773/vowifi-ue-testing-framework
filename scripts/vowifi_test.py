import re


def extract_information(log_text):
    log_text = re.findall(
        f"vowifi-log/{device_name}/{experiment_info}/([^/]+)/", log_text
    )
    global log_text

    for i in range(len(log_text)):
        # 1. 휴대폰 device 이름 추출
        device_name_matches = re.findall(r"vowifi-log/([^/]+)/", log_text[i])
        device_names = device_name_matches if device_name_matches else None

        # 2. 각 device에 대한 experiment information 추출
        all_experiment_information = []
        for device_name in device_names:
            experiment_information_matches = re.findall(
                f"vowifi-log/{device_name}/([^/]+)/", log_text[i]
            )
            experiment_information = (
                experiment_information_matches
                if experiment_information_matches
                else None
            )

            # 3. 각 experiment_information에 대한 Functional Oracle과 Liveness Oracle 추출
            functional_oracles = []
            liveness_oracles = []
            for experiment_info in experiment_information:
                oracle_match = re.search(
                    rf"vowifi-log/{device_name}/{experiment_info}/Functional Oracle: (\w+)\s+Liveness Oracle: (\w+)",
                    log_text[i],
                )
                functional_oracle = oracle_match.group(1) if oracle_match else None
                liveness_oracle = oracle_match.group(2) if oracle_match else None

                functional_oracles.append(functional_oracle)
                liveness_oracles.append(liveness_oracle)

            all_experiment_information.append(
                {
                    "device_name": device_name,
                    "experiments": experiment_information,
                    "functional_oracles": functional_oracles,
                    "liveness_oracles": liveness_oracles,
                }
            )

    return all_experiment_information


result = extract_information(log_text)
print(result)

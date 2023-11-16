import re
import pandas as pd


def extract_information(log_text):
    # 1. 휴대폰 device 이름 추출
    device_name_matches = re.findall(r"vowifi-log/([^/]+)/", log_text)
    device_names = device_name_matches if device_name_matches else None
    # 2. 각 device에 대한 experiment information 추출
    all_experiment_information = []

    for device_name in device_names:
        experiment_information_matches = re.findall(
            f"vowifi-log/{device_name}/([^/]+)/", log_text
        )
        experiment_information = (
            experiment_information_matches if experiment_information_matches else None
        )

        # 3. 각 experiment_information에 대한 Functional Oracle과 Liveness Oracle 추출
        functional_oracles = []
        liveness_oracles = []

        # 추가: experiment_information이 None이 아닌 경우에만 반복문 수행
        if experiment_information:
            for experiment_info in experiment_information:
                oracle_match = re.search(
                    rf"vowifi-log/{device_name}/{experiment_info}/Functional Oracle: (\w+)\s+Liveness Oracle: (\w+)",
                    log_text,
                )
                functional_oracle = oracle_match.group(1) if oracle_match else None
                liveness_oracle = oracle_match.group(2) if oracle_match else None

                functional_oracles.append(functional_oracle)
                liveness_oracles.append(liveness_oracle)

        all_experiment_information.append(
            {
                "device_name": device_name,
                "experiment_information": experiment_information,
                "functional_oracles": functional_oracles,
                "liveness_oracles": liveness_oracles,
            }
        )

    return all_experiment_information


path = ""
log_text = pd.read_csv(path)
# 테스트용 로그 텍스트?이렇게 하면 안될텐데
log_text = """
vowifi-log/HTC_U11/result.init-dh-group--1.122.1695207740608
Testcase: {"testcase":[{"receiver":"ue","sender":"epdg","name":"enable_vowifi","id":"3-1"},{"sub":[{"op":"update","name":"message_size","type":"uint16","value":4000}],"receiver":"epdg","sender":"epdg","name":"ike_sa_init_response"}]}
Message:
  Query: enable_vowifi
  Reply: ike_sa_init_request
  Query: ike_sa_init_response
  Reply: timeout
Result:
  Functional Oracle: negative
  Liveness Oracle: negative
"""

result = extract_information(log_text)
print(result)

import os
import sys
import json
import argparse

def extract_info(line):
    parts = line.split('//')[-1].split('/')
    file_part = parts[-1]
    
    if 'ike' not in file_part or 'response-' not in file_part:
        raise ValueError(f"Invalid format in line: {line}")

    json_filename = 'ike' + file_part.split('ike')[1].split('response')[0] + 'response'
    testcase_id = file_part.split('result.')[1].split('.')[0]

    return json_filename, testcase_id

def extract_testcase_from_json(testcase_path, testcase_id):
    testcases_collected = []

    if os.path.exists(testcase_path):
        with open(testcase_path, 'r') as testcase_file:
            data = json.load(testcase_file)
            if "testcases" in data:
                for testcase_entry in data["testcases"]:
                    if "testcase" in testcase_entry:
                        for testcase in testcase_entry["testcase"]:
                            if testcase.get("id") == testcase_id:
                                testcases_collected.append(testcase_entry)  # Append entire testcase_entry
            else:
                print(f'JSON file {testcase_path} does not contain a "testcases" key.')
    else:
        print(f'Test case file {testcase_path} not found.')
    
    return testcases_collected

def main(result_file_path, testcase_dir):
    testcases_collected = []
    ue_model = None

    with open(result_file_path, 'r') as file:
        lines = file.readlines()

    # Ignore the first and last lines
    lines = lines[1:-1]

    for line in lines:
        line = line.strip()
        if line:
            if not ue_model:
                ue_model = line.split('//')[-1].split('/')[0]

            try:
                json_filename, testcase_id = extract_info(line)
                testcase_path = os.path.join(testcase_dir, f'{json_filename}.json')

                # Skip lines containing "Function Oracle : liveness"
                if "Liveness Oracle" in line:
                    continue

                testcases = extract_testcase_from_json(testcase_path, testcase_id)
                if testcases:
                    testcases_collected.extend(testcases)
                    print(f'Extracted root JSON object from {json_filename}.json where testcase id is "{testcase_id}"')
                else:
                    print(f'Test case with id "{testcase_id}" not found in {testcase_path}.')
            except ValueError as e:
                print(e)

    if testcases_collected:
        output_data = {
            "testcases": testcases_collected
        }

        output_filename = f'{ue_model}.json'
        with open(output_filename, 'w') as output_file:
            json.dump(output_data, output_file, indent=2)
        print(f'Created {output_filename} with all extracted test cases.')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Process result file and test case directory to generate UE-specific test cases.')
    parser.add_argument('-r', '--results', required=True, help='Path to the results.txt file')
    parser.add_argument('-d', '--directory', required=True, help='Path to the test case directory')

    args = parser.parse_args()

    result_file_path = args.results
    testcase_dir = args.directory

    print(f'Starting extraction from {result_file_path} and {testcase_dir}')
    main(result_file_path, testcase_dir)


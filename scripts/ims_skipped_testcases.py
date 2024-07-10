import os
import json
import argparse

def extract_testcase_from_json(testcase_path, testcase_id):
    testcases_collected = []

    if os.path.exists(testcase_path):
        with open(testcase_path, 'r') as testcase_file:
            try:
                data = json.load(testcase_file)
                if "testcases" in data:
                    for testcase_entry in data["testcases"]:
                        if "testcase" in testcase_entry:
                            for testcase in testcase_entry["testcase"]:
                                if testcase.get("id") == testcase_id:
                                    testcases_collected.append(testcase_entry)  # Append entire testcase_entry
                else:
                    print(f'JSON file {testcase_path} does not contain a "testcases" key.')
            except json.JSONDecodeError as e:
                print(f'Error decoding JSON file {testcase_path}: {str(e)}')
    else:
        print(f'Test case file {testcase_path} not found.')
    
    return testcases_collected

def process_result_file(result_directory, testcase_dir):
    result_file_path = None
    testcases_collected = {}
    output_directory = os.path.join(result_directory, 'failed-testcases')
    
    # Create the output directory if it doesn't exist
    if not os.path.exists(output_directory):
        os.makedirs(output_directory)

    print(f'Searching for result.txt file in directory: {result_directory}')

    # Find result.txt file in the result_directory
    for filename in os.listdir(result_directory):
        if filename.startswith('result.txt.'):
            result_file_path = os.path.join(result_directory, filename)
            print(f'Found result.txt file: {result_file_path}')
            break
    
    if not result_file_path:
        print(f'Error: No result.txt file found in {result_directory}')
        return
    
    print(f'Processing {result_file_path}...')

    current_ue_model = None

    with open(result_file_path, 'r') as file:
        lines = file.readlines()

    for line in lines:
        line = line.strip()
        if line.startswith("====="):
            if current_ue_model:
                if current_ue_model in testcases_collected:
                    ue_model_output_file = os.path.join(output_directory, f'{current_ue_model}.json')
                    output_data = {"testcases": testcases_collected[current_ue_model]}
                    with open(ue_model_output_file, 'w') as output_file:
                        json.dump(output_data, output_file, indent=2)
                        print(f'Created {ue_model_output_file} with all extracted test cases.')
                else:
                    print(f'No test cases collected for {current_ue_model}.')
            current_ue_model = line.strip("=").strip()
            print(f'Processing UE model: {current_ue_model}')
            testcases_collected[current_ue_model] = []
        elif line:
            try:
                testcase_id = line.split('result.')[1].split('.')[0]
                testcase_path = os.path.join(testcase_dir, '401_unauthorized.json')
                print(f'Processing testcase id: {testcase_id} in UE model: {current_ue_model}')
                print(f'Test case file path: {testcase_path}')
                testcases = extract_testcase_from_json(testcase_path, testcase_id)
                if testcases:
                    testcases_collected[current_ue_model].extend(testcases)
                    print(f'Extracted {len(testcases)} test cases for {testcase_id} in {current_ue_model}')
                else:
                    print(f'Test case with id "{testcase_id}" not found in {testcase_path}.')
            except IndexError:
                print(f'Error extracting testcase id from line: {line}')

    # Process the last UE model
    if current_ue_model:
        if current_ue_model in testcases_collected:
            ue_model_output_file = os.path.join(output_directory, f'{current_ue_model}.json')
            output_data = {"testcases": testcases_collected[current_ue_model]}
            with open(ue_model_output_file, 'w') as output_file:
                json.dump(output_data, output_file, indent=2)
                print(f'Created {ue_model_output_file} with all extracted test cases.')
        else:
            print(f'No test cases collected for {current_ue_model}.')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Process result file and test case directory to generate UE-specific test cases.')
    parser.add_argument('-d', '--directory', required=True, help='Path to the directory where result.txt file resides')
    parser.add_argument('-t', '--testcase-dir', default='~/vowifi-core-network/testcases', help='Path to the test case directory (default: ~/vowifi-core-network/testcases)')

    args = parser.parse_args()

    result_directory = args.directory
    testcase_dir = os.path.expanduser(args.testcase_dir)  # Expand ~ to full path

    print(f'Starting extraction from {result_directory} and {testcase_dir}')

    process_result_file(result_directory, testcase_dir)


import json
import argparse

def count_test_cases(json_file_path):
    with open(json_file_path, 'r') as file:
        data = json.load(file)
        
    # Count the number of top-level "testcase" groups
    test_case_count = len(data.get("testcases", []))
        
    return test_case_count

def main():
    parser = argparse.ArgumentParser(description="Count the number of test cases in a JSON file.")
    parser.add_argument('json_file_path', type=str, help='Path to the JSON file.')
    
    args = parser.parse_args()
    
    test_case_count = count_test_cases(args.json_file_path)
    print(f'The number of test cases in the JSON file is: {test_case_count}')

if __name__ == "__main__":
    main()


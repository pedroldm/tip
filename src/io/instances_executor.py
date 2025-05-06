from collections import defaultdict
import json
import os
import random
import subprocess

import concurrent.futures
import tqdm

class InstancesExecutor:
    params = {
        "p": 400, # [200, 300, 400, 500]
        "pe": 0.15, # [0.05, 0.10, 0.15]
        "pm": 0.10, # [0.10, 0.20, 0.30]
        "rhoe": 0.5, # [0.50, 0.55, 0.60, 0.65]
        "K": 1, # [1]
        "MAXT": 16, # [-]
        "X_INTVL": 1, # [-]
        "X_NUMBER": 0, # []
        "MAX_GENS": 700, # [5000]
        "MAX_GENS_WITHOUT_IMPROVEMENT": 100,
        "irace": "false",
        "lsNonEliteAplicationPercentage": 0.05,
        "lsEliteApplicationPercentage": 0.5,
        "lsCoveragePercentage": 0.75,
        "timeLimit": 120,
        "useVND": "true",
        "PM_INCREASE_FACTOR": 0.10
    }

    def __init__(self, params: dict) :
        self.params = params
    
    def save_results(self, results, dir):
        with open(dir, 'w') as f:
            json.dump(results, f, indent=4)
            
    def read_result(self, dir):
        with open(dir, 'r') as f:
            data = json.load(f)
        return data
    
    def scan_files_in_subdirs(self, directory):
        return [os.path.join(root, file) for root, _, files in os.walk(directory) for file in files]
    
    def run(self):
        args = ['/home/pedro/Pessoal/MSc/tip/main_prd']
        for p, v in self.params.items():
            args.append(f"--{p}={v}")
            
        result = subprocess.run(args, capture_output=True, text=True)
        return json.loads(result.stdout)
    
    def run_all(self, runs: int = 1, instances_subset = []):
        instances = self.scan_files_in_subdirs("/home/pedro/Pessoal/MSc/tip/instances")
        if instances_subset:
            instances = [i for i in instances if os.path.basename(os.path.dirname(i)) in instances_subset]

        results = defaultdict(list)

        with tqdm.tqdm(total=len(instances) * runs, desc="Running instances...") as pbar:
            for instance in instances:
                instance_name = os.path.basename(instance)
                for _ in range(runs):
                    self.params['filepath'] = instance
                    self.params['rngSeed'] = random.randint(0, 1000000000)
                    result = self.run()
                    results[instance_name].append(result)
                    pbar.update(1)

        return results
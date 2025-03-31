from collections import defaultdict
import json
import os
import random
import subprocess

import concurrent.futures
import tqdm

class InstancesExecutor:
    params = {
        "p": 500, # [200, 300, 400, 500]
        "pe": 0.15, # [0.05, 0.10, 0.15]
        "pm": 0.20, # [0.10, 0.20, 0.30]
        "rhoe": 0.50, # [0.50, 0.55, 0.60, 0.65]
        "K": 1, # [1]
        "MAXT": 1, # [-]
        "X_INTVL": 1, # [-]
        "X_NUMBER": 2, # []
        "MAX_GENS": 7000, # [5000]
        "irace": "false",
        "rngSeed": 2147483647
    }
    
    def save_results(self, results, dir):
        with open(dir, 'w') as f:
            json.dump(results, f, indent=4)
            
    def read_result(self, dir):
        with open(dir, 'r') as f:
            data = json.load(f)
        return data
    
    def scan_files_in_subdirs(self, directory):
        return [os.path.join(root, file) for root, _, files in os.walk(directory) for file in files]
    
    def run(self, params: dict):
        args = ['/home/pedro/tip/main_prd']
        for p, v in params.items():
            args.append(f"--{p}={v}")
            
        result = subprocess.run(args, capture_output=True, text=True)
        return json.loads(result.stdout)
    
    def run_all(self, runs: int = 1, instances_subset = []):
        def process_instance(instance, runs):
            instance_results = []
            for i in range(runs):
                self.params['filepath'] = instance
                self.params['rngSeed'] = random.randint(0, 1000000000)
                result = self.run(self.params)
                instance_results.append(result)
            return os.path.basename(instance), instance_results
        
        instances = self.scan_files_in_subdirs("/home/pedro/tip/instances")
        if(instances_subset):
            instances = [i for i in instances if os.path.basename(os.path.dirname(i)) in instances_subset]
            
        results = defaultdict(list)

        with concurrent.futures.ThreadPoolExecutor(max_workers=4) as executor:
            futures = [executor.submit(process_instance, instance, runs) for instance in instances]
            
            for future in tqdm.tqdm(concurrent.futures.as_completed(futures), total=len(futures), desc="Running instances..."):
                instance_name, instance_results = future.result()
                results[instance_name].extend(instance_results)
        
        return results
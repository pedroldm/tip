from instances_executor import InstancesExecutor

ie = InstancesExecutor()
results = ie.run_all(runs=5, instances_subset=["sko"])
ie.save_results(results, "/home/pedro/tip/output/sko_2.json")
import csv
from collections import defaultdict

def calculate_averages(filename):
    data = defaultdict(lambda: {'waiting': [], 'turnaround': []})
    
    with open(filename, newline='') as file:
        reader = csv.reader(file)
        for queue_id, _, waiting, turnaround in reader:
            data[queue_id]['waiting'].append(int(waiting))
            data[queue_id]['turnaround'].append(int(turnaround))
    
    for queue_id, times in data.items():
        avg_waiting = sum(times['waiting']) / len(times['waiting'])
        avg_turnaround = sum(times['turnaround']) / len(times['turnaround'])
        
        print(f"Queue: {queue_id} | Avg Waiting Time: {avg_waiting:.2f} | Avg Turnaround Time: {avg_turnaround:.2f}")

calculate_averages("log.csv")

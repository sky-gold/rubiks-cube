import subprocess
import time
import os

def format_time(avg_time):
    total_seconds = avg_time
    hours = int(total_seconds // 3600)
    remainder = total_seconds % 3600
    minutes = int(remainder // 60)
    seconds = remainder % 60
    
    parts = []
    if hours > 0:
        parts.append(f"{hours}h")
    if minutes > 0:
        parts.append(f"{minutes}m")
    if seconds >= 0.1 or (hours == 0 and minutes == 0):
        if seconds.is_integer():
            parts.append(f"{int(seconds)}s")
        else:
            parts.append(f"{seconds:.1f}s")
    
    return ' '.join(parts) or "0s"

threads_counts = [i for i in range(2, 9)]
filenames = ["static/14.txt", "static/15.txt", "static/16.txt"]
pdb_size = "high"

for filename in filenames:
    print()
    print(f"--- filename = {filename}")

    if not os.path.isfile(filename):
            print(f"Файл {filename} не найден, пропускаем.")
            continue

    for threads_count in threads_counts:
        
        start_time = time.time()
        
        with open(filename, 'r') as f:
            subprocess.run(
                [
                    "./build/src/rubiks_cube_solver",
                    "--threads_count", str(threads_count),
                    "--pdb_size", pdb_size
                ],
                stdin=f,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL
            )
        
        total_time = time.time() - start_time
        
        with open(filename, 'r') as f:
            lines_count = sum(1 for line in f if line.strip())
        
        avg_time = total_time / max(1, lines_count)
        
        print(f"threads_count={str(threads_count).ljust(4)} time={format_time(avg_time).rjust(8)}")

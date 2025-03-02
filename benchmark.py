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

filenames = ["static/random5.txt"]
for i in range(1, 19):
    filenames.append(f"static/{i}.txt")

header = (
    "| Файл с позициями        | Количество позиций | Общее время (секунды) | Среднее время на позицию |"
)
separator = "|-------------------------|--------------------|-----------------------|--------------------------|"


print(header)
print(separator)


for filename in filenames:
    if not os.path.isfile(filename):
        print(f"Файл {filename} не найден, пропускаем.")
        continue

    start_time = time.time()
    
    with open(filename, 'r') as f:
        subprocess.run(
            ['./build/src/rubiks_cube_solver'],
            stdin=f,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL
        )
    
    total_time = time.time() - start_time
    
    with open(filename, 'r') as f:
        lines_count = sum(1 for line in f if line.strip())
    
    avg_time = total_time / max(1, lines_count)
    print(
        f"| {filename.ljust(23)} "
        f"| {str(lines_count).center(18)} "
        f"| {total_time:>21.1f} "
        f"| {format_time(avg_time).rjust(24)} |"
    )

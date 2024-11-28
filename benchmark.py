import subprocess
import re
import time

def run_command(file_number):
    command = f"/bin/time -v build/src/rubiks_cube_solver < static/{file_number}.txt"
    timeout_min = 15
    try:
        result = subprocess.run(command, shell=True, capture_output=True, text=True, timeout= timeout_min * 60)
        output = result.stderr

        user_time_match = re.search(r"User time \(seconds\): (\d+\.\d+)", output)
        max_rss_match = re.search(r"Maximum resident set size \(kbytes\): (\d+)", output)

        if user_time_match and max_rss_match:
            user_time = float(user_time_match.group(1))
            max_rss = int(max_rss_match.group(1))
            return user_time, max_rss
        else:
            return None, None
    except subprocess.TimeoutExpired:
        return None, None

distance_width = 23
time_width = 17
memory_width = 19

print(f"| Расстояние от решения {' ' * (distance_width - 23)} | Время (секунды) {' ' * (time_width - 17)} | Память (килобайты){' ' * (memory_width - 22)} |")
print("|------------------------|------------------|--------------------|")

for i in range(1, 6):
    user_time, max_rss = run_command(i)
    if user_time is not None and max_rss is not None:
        print(f"| {i} {' ' * (distance_width - len(str(i)) - 2)} | {user_time} {' ' * (time_width - len(str(user_time)) - 2)} | {max_rss} {' ' * (memory_width - len(str(max_rss)) - 2)} |")
    else:
        print(f"| {i} {' ' * (distance_width - len(str(i)) - 2)} | >15 мин {' ' * (time_width - 7)} | ??? {' ' * (memory_width - 4)} |")
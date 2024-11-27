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

print("| Расстояние от решения | Время (секунды) | Память (килобайты) |")
print("|-----------------------|-----------------|--------------------|")

for i in range(1, 6):
    user_time, max_rss = run_command(i)
    if user_time is not None and max_rss is not None:
        print(f"| {i} | {user_time} | {max_rss} |")
    else:
        print(f"| {i} | >15 мин | ??? |")
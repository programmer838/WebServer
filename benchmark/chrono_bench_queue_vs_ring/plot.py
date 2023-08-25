import matplotlib.pyplot as plt
import numpy as np

# Replace these values with your data

ring_buffer_times = [
    250, 251, 271, 271, 151, 160, 140, 111, 150, 261, 270,
    151, 141, 150, 260, 150, 271, 151, 260, 160, 261, 150,
    150, 261, 160, 151, 150, 150, 150, 150
]

std_queue_times = [
    10960, 6201, 6071, 11141, 7073, 6242, 6132, 4428, 5931, 6713,
    10510, 11491, 8926, 6802, 6442, 9358, 6011, 10329, 10219, 6172,
    16861, 6291, 9038, 11812, 11331, 10400, 9067, 11582, 10670
]

plt.plot(ring_buffer_times, label='ring_buffer', marker='o', markersize=3, linestyle='-')
plt.plot(std_queue_times, label='std::queue', marker='o', markersize=3, linestyle='-')
plt.xlabel('Benchmark Run')
plt.ylabel('Time (ns)') # Adjust the unit as needed
plt.title('Comparison of ring_buffer and std::queue using std::chrono')

# Set custom ticks on the x axis, for example, every 5 runs
plt.xticks(np.arange(0, len(ring_buffer_times), step=5)) # You can adjust the step as needed

# Set custom ticks on the y axis, for example, every 2000 ns
plt.yticks(np.arange(min(min(ring_buffer_times), min(std_queue_times)), max(max(ring_buffer_times), max(std_queue_times)), step=2000)) # You can adjust the step as needed

plt.legend()
plt.show()

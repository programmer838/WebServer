import matplotlib.pyplot as plt
import pandas as pd
import os

# Read the data into a pandas DataFrame, stripping any whitespace from headers
df = pd.read_csv('times.txt', skipinitialspace=True)

# Strip whitespace from DataFrame column names
df.columns = df.columns.str.strip()

# Separate the DataFrame into atomic and mutex DataFrames based on the 'Type' column
atomic_df = df[df['Type'] == 'atomic_threadpool_perf']
mutex_df = df[df['Type'] == 'mutex_threadpool_perf']

# Calculate the rolling mean over 10 runs for a smoother line for CPU cycles
window_size = 10
atomic_rolling_cycles = atomic_df['CPU Cycles'].rolling(window_size).mean()
mutex_rolling_cycles = mutex_df['CPU Cycles'].rolling(window_size).mean()

# Plot the rolling mean CPU cycles
plt.figure(figsize=(14, 7))
plt.plot(atomic_rolling_cycles, label='Atomic Rolling Mean CPU Cycles', color='tab:red')
plt.plot(mutex_rolling_cycles, label='Mutex Rolling Mean CPU Cycles', color='tab:blue')
plt.title('Rolling Mean CPU Cycles: Atomic vs Mutex Thread Pool')
plt.xlabel('Run (Rolling Window)')
plt.ylabel('Rolling Mean CPU Cycles')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# Calculate the rolling mean over 10 runs for IPC
atomic_rolling_ipc = atomic_df['IPC'].rolling(window_size).mean()
mutex_rolling_ipc = mutex_df['IPC'].rolling(window_size).mean()

# Plot the rolling mean IPC
plt.figure(figsize=(14, 7))
plt.plot(atomic_rolling_ipc, label='Atomic Rolling Mean IPC', color='tab:red')
plt.plot(mutex_rolling_ipc, label='Mutex Rolling Mean IPC', color='tab:blue')
plt.title('Rolling Mean IPC: Atomic vs Mutex Thread Pool')
plt.xlabel('Run (Rolling Window)')
plt.ylabel('Rolling Mean IPC')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# Calculate the rolling mean over 10 runs for a smoother line for Context Switches
atomic_rolling_context_switches = atomic_df['Context Switches'].rolling(window_size).mean()
mutex_rolling_context_switches = mutex_df['Context Switches'].rolling(window_size).mean()

# Plot the rolling mean Context Switches
plt.figure(figsize=(14, 7))
plt.plot(atomic_rolling_context_switches, label='Atomic Rolling Mean Context Switches', color='tab:red')
plt.plot(mutex_rolling_context_switches, label='Mutex Rolling Mean Context Switches', color='tab:blue')
plt.title('Rolling Mean Context Switches: Atomic vs Mutex Thread Pool')
plt.xlabel('Run (Rolling Window)')
plt.ylabel('Rolling Mean Context Switches')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# Clean up
os.remove("times.txt") 
os.remove("atomic_threadpool_perf")
os.remove("mutex_threadpool_perf")

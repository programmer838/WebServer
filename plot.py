import matplotlib.pyplot as plt
import numpy as np

# Replace these values with your data
lambda_times = [
    411106, 714418, 450220, 480707, 603930, 600252, 397390, 450951,
    680914, 598039, 496547, 571227, 499973, 515674, 598299, 671387,
    481619, 632343, 566900, 505484, 492208, 435081, 628125, 490265,
    679472, 552242, 393863, 817573, 637362, 610181, 387732, 568011,
    511375, 425704, 365249, 476970, 456271, 573742, 683390, 621081,
    453767, 537664, 462233, 499122, 588010, 542494, 690543, 608989,
    525382, 464476
]

bind_times = [
    713837, 446322, 492419, 528447, 425964, 569574, 575366, 523618,
    614049, 407809, 534759, 496768, 427206, 574784, 580405, 481649,
    549607, 580786, 377291, 491046, 377162, 514070, 593510, 478303,
    690413, 626412, 385437, 565377, 553825, 412499, 792916, 519961,
    517267, 584463, 430623, 565948, 468144, 557662, 603779, 449088, 
    507568, 411948, 552603, 632173, 450701, 551200, 533477, 578611, 
    374076, 710100
]

plt.plot(lambda_times, label='lambda', marker='o', markersize=3, linestyle='-')
plt.plot(bind_times, label='bind', marker='o', markersize=3, linestyle='-')
plt.xlabel('Benchmark Run')
plt.ylabel('Time (ns)')  # Adjust the unit as needed
plt.title('Comparison of ring_buffer and std::queue using std::chrono')

# Set custom ticks on the x axis, for example, every 5 runs
plt.xticks(np.arange(0, len(lambda_times), step=5))  # You can adjust the step as needed

# Set custom ticks on the y axis, for example, every 2000 ns
plt.yticks(
    np.arange(
        min(min(lambda_times), min(bind_times)), 
        max(max(lambda_times), max(bind_times)), 
        step=2000
    )
)  # You can adjust the step as needed

plt.legend()
plt.show()


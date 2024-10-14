#!python

import matplotlib.pyplot as plt
import csv

# Initialize an empty list to store the data
data = []

# Read data from the CSV file (assuming all values are in one row)
with open('./performance-metrics/last-run-fps.csv', 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        if row:
            data = [float(value) for value in row]

# Create time points for x-axis (100ms intervals)
time = [i * 0.1 for i in range(len(data))]  # 100ms intervals

# Plot the data
plt.plot(time, data)
plt.title('Complete frametime over runtime')
plt.xlabel('Elapsed time (Seconds')
plt.ylabel('Frametime (Milliseconds)')

# Display the plot
plt.show()


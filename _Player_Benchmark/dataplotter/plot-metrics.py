#!python
import matplotlib.pyplot as plt
import csv

def read_data(file_path):
    # Initialize a dictionary to store each column's data
    data = {}

    # Read data from the CSV file
    with open(file_path, 'r') as file:
        reader = csv.DictReader(file)  # Use DictReader to access columns by name
        headers = reader.fieldnames  # Get the column names from the first row

        # Skip the first record
        next(reader)

        for row in reader:
            for header in headers:
                if header not in data:
                    data[header] = []  # Initialize list for each header
                data[header].append(float(row[header]))  # Append data as float

    return data

def plot_data(data):
    # Create time points for x-axis (100ms intervals)
    time = [i * 0.1 for i in range(len(data[next(iter(data))]))]  # Use the first key to get length

    plt.figure(figsize=(16, 10))

    # Plot each metric
    for key in data.keys():
        plt.plot(time, data[key], label=key)

    # Add titles and labels
    plt.title('Performance Metrics Over Time')
    plt.xlabel('Time (seconds)')
    plt.ylabel('Time (ms)')
    plt.legend()  # Add a legend to differentiate the metrics


    # Display the plot
    plt.show()

def main():
    file_path = './performance-metrics/last-run.csv'
    data = read_data(file_path)  # Read the CSV data
    plot_data(data)  # Plot the data

if __name__ == "__main__":
    main()

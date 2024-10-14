#!python
import matplotlib.pyplot as plt
import csv
import os

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

def plot_data(data, output_file):
    # Create time points for x-axis (100ms intervals)
    time = [i * 0.1 for i in range(len(data[next(iter(data))]))]  # Use the first key to get length

    # Create a larger figure
    plt.figure(figsize=(16, 10))  # Set figure size (width, height)

    # Plot each metric
    for key in data.keys():
        plt.plot(time, data[key], label=key)

    # Add titles and labels
    plt.title('Performance Metrics Over Time')
    plt.xlabel('Time (seconds)')
    plt.ylabel('Time (ms)')
    
    # Adjust legend location and size
    plt.legend(loc='upper left', bbox_to_anchor=(1, 1), fontsize='small', title="Metrics")

    # Save the plot to file
    plt.tight_layout()  # Adjust layout to make room for the legend
    plt.savefig(output_file)
    plt.close()  # Close the figure after saving

def main():
    input_dir = './performance-metrics/'
    output_dir = './performance-plots/'

    # Create output directory if it doesn't exist
    os.makedirs(output_dir, exist_ok=True)

    # Iterate over all CSV files in the input directory
    for file_name in os.listdir(input_dir):
        if file_name.endswith('.csv'):
            file_path = os.path.join(input_dir, file_name)
            output_file = os.path.join(output_dir, file_name.replace('.csv', '.png'))
            
            # Read the CSV data
            data = read_data(file_path)
            
            # Plot and save to file
            plot_data(data, output_file)

if __name__ == "__main__":
    main()


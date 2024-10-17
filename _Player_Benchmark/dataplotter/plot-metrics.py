#!python
import matplotlib.pyplot as plt
import csv
import os
import argparse
import numpy as np

"""
SYNOPSIS:
./plot-metrics.py ./performance-metrics/2024-10-17/20-55-44
./plot-metrics.py ./performance-metrics/2024-10-17/20-55-44 --compare-to ./performance-metrics/2024-10-17/20-40-22
"""

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

def interpolate_data(data, target_length):
    """
    Interpolate data to match the target length.
    """
    current_length = len(data)
    if current_length == target_length:
        return data
    x_old = np.linspace(0, 1, current_length)
    x_new = np.linspace(0, 1, target_length)
    return np.interp(x_new, x_old, data)

def plot_data(data1, output_file, title, data2=None):
    # Create time points for x-axis (100ms intervals)
    time1 = [i * 0.1 for i in range(len(data1[next(iter(data1))]))]  # Use the first key to get length

    # Create a larger figure
    plt.figure(figsize=(16, 10))  # Set figure size (width, height)

    # Plot each metric from the first dataset
    for key in data1.keys():
        plt.plot(time1, data1[key], label=key)

    # If second dataset is provided, plot it with the same colors but opacity 0.5
    if data2:
        time2 = [i * 0.1 for i in range(len(data2[next(iter(data2))]))]  # Second dataset time points
        for key in data2.keys():
            if key in data1:  # Ensure we have the same metrics in both datasets
                # Interpolate second dataset to match the time length of the first
                interpolated_data2 = interpolate_data(data2[key], len(time1))
                plt.plot(time1, interpolated_data2, label=f"compared run: {key}", alpha=0.25)

    # Add titles and labels
    plt.title(title)
    plt.xlabel('Time (seconds)')
    plt.ylabel('Time (ms) (lower is better)')
    
    # Adjust legend location and size
    plt.legend(loc='upper left', bbox_to_anchor=(1, 1), fontsize='small', title="Metrics")

    # Save the plot to file
    plt.tight_layout()  # Adjust layout to make room for the legend
    plt.savefig(output_file)
    plt.close()  # Close the figure after saving

def main():
    # Set up argument parser
    parser = argparse.ArgumentParser(description="Process CSV files and generate comparison plots.")
    parser.add_argument('input_dir1', type=str, help="Directory containing first set of CSV files.")
    parser.add_argument('--compare-to', type=str, help="Directory containing second set of CSV files (optional).")

    # Parse arguments
    args = parser.parse_args()

    # Define output directory
    output_dir = './performance-plots/'

    # Create output directory if it doesn't exist
    os.makedirs(output_dir, exist_ok=True)

    # Iterate over all CSV files in the first input directory
    for file_name in os.listdir(args.input_dir1):
        if file_name.endswith('.csv'):
            file_path1 = os.path.join(args.input_dir1, file_name)
            output_file = os.path.join(output_dir, file_name.replace('.csv', '.png'))

            # Read the CSV data from the first directory
            data1 = read_data(file_path1)

            # If the second directory is provided, try to find the same file there
            data2 = None
            if args.compare_to:
                file_path2 = os.path.join(args.compare_to, file_name)
                if os.path.exists(file_path2):
                    data2 = read_data(file_path2)

            # Plot and save to file (with comparison if data2 exists)
            if args.compare_to:
                plotTitle = f'Compare run\n"{file_path1}"\nwith\n"{file_path2}" (faded out)'
            else:
                plotTitle = f'Inspect run\n{file_path1}"'
            plot_data(data1, output_file, plotTitle, data2)

if __name__ == "__main__":
    main()


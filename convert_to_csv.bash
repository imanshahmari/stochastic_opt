#!/bin/bash

# Check if conda is installed
command -v conda >/dev/null 2>&1 || { echo >&2 "conda is required but not installed. Aborting."; exit 1; }

# Input file path
input_file="$1"

# Check if input file path is provided
if [ -z "$input_file" ]; then
  echo "Usage: $0 <input_file.xlsx> <output_file.csv>"
  exit 1
fi

# Check if input file exists
if [ ! -f "$input_file" ]; then
  echo "Input file '$input_file' does not exist. Aborting."
  exit 1
fi

# Output file name
output_file="$2"

# Check if output file name is provided
if [ -z "$output_file" ]; then
  echo "Usage: $0 <input_file.xlsx> <output_file.csv>"
  exit 1
fi

# Convert Excel to CSV using pandas
python <<END
import pandas as pd

input_file = "$input_file"
output_file = "$output_file"

df = pd.read_excel(input_file)
df.to_csv(output_file, index=False)
END

# Deactivate and remove the conda virtual environment
conda deactivate

echo "Excel file '$input_file' has been converted to CSV file '$output_file'."



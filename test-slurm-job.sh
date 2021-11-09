#!/bin/bash

# Name for job
#SBATCH --job-name=darts_render

# Request the default queue for this job
#SBATCH --partition=standard

# Request 1 node for this job
#SBATCH --nodes=1

# Specify number of tasks per node for this job
#SBATCH --ntasks-per-node=1

# Number of tasks (i.e. cores) per CPU to allocate
#SBATCH --cpus-per-task=8

# Request 60 minutes of wall time
#SBATCH --time=00:60:00

# Specify compute resource account
#SBATCH --account cs_comp

# Submit a job array
#SBATCH --array=0-32

# Name the standard and error log files for each element of the job array
# Where %A is the job ID and %a is the array index
#SBATCH --output=results/stdout_%A-%a

# You can use the environment variable SLURM_ARRAY_TASK_ID to identify
# each element of the job array

./build/darts scenes/assignment5/jensen_box_mats.json -o results/jensen_box_mats_$SLURM_ARRAY_TASK_ID.exr
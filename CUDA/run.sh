#!/bin/sh
#SBATCH -J gpu_05
#SBATCH --time=00:05:00
#SBATCH -p dual_v100_node
#SBATCH -o result_%j.out
##SBATCH -e result_%j.err
#SBATCH --gres=gpu:1
srun ./a



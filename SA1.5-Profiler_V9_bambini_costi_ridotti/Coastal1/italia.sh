#!/bin/bash
#SBATCH --time=5:00:00
#SBATCH --job-name=test
#SBATCH --output=prova.out
#SBATCH --account=def-cordeauj
#SBATCH --mem=24576M
#SBATCH --cpus-per-task=1
#SBATCH --mail-user=<196906@studenti.unimore.it>
#SBATCH --mail-type=FAIL,SUCCESS
cd /home/tedeschi/Costal
./costal

#!/bin/sh
#SBATCH --partition=general-compute
#SBATCH --time=12:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
##SBATCH --constraint=IB
##SBATCH --mem=1024
# Memory per node specification is in MB. It is optional. 
# The default limit is 3000MB per core.
#SBATCH --job-name="DustSim_BatchTest"
#SBATCH --output="dustsimout_batchtest1.txt"
#SBATCH --mail-user=kgvansly@buffalo.edu
#SBATCH --mail-type=ALL
##SBATCH --requeue
#Specifies that the job will be requeued after a node failure.
#The default is that the job will not be requeued.

echo "SLURM_JOBID="$SLURM_JOBID
echo "SLURM_JOB_NODELIST"=$SLURM_JOB_NODELIST
echo "SLURM_NNODES"=$SLURM_NNODES
echo "SLURMTMPDIR="$SLURMTMPDIR

echo "working directory = "$SLURM_SUBMIT_DIR

NPROCS=`srun --nodes=${SLURM_NNODES} bash -c 'hostname' |wc -l`
echo NPROCS=$SLURM_NPROCS

module load intel-mpi
module load intel
module list
ulimit -s unlimited

#The PMI library is necessary for srun
export I_MPI_PMI_LIBRARY=/usr/lib64/libpmi.so

make dust_simulation

echo "Launch MPI merging dust simulation with srun"
echo "Doing dust simulation with pore size 15 and min / max grain size 1 / 15"
echo "Varying fiber diameter from 1 to 45"
for ((i = 1; i < 45; i++))
do
	echo "Fiber diameter = $i"
	srun -n $SLURM_NPROCS ./dust_simulation 1000 1000 20 20 1000 1 15 1000 1 0 0 $i 15 1
done

echo "All Done!"

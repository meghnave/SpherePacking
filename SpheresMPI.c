#include<stdio.h>
#include<mpi.h>
int main(int argc,char *argv[])
{
	int tid,nth;
	char *cpu_name;
	double start,end,time;
	start=MPI_Wtime();
	MPI_Init(&argc,&argv);	//launch mpi process on each node
	MPI_Comm_rank(MPI_COMM_WORLD,&tid); //request a th id
	MPI_Comm_size(MPI_COMM_WORLD,&nth); //get total no of threads launched
	cpu_name=(char*)calloc(80,sizeof(char)); //get machine of this host
	gethostname(cpu_name,80);
	printf("\nHello from process %d on machine %s\n",tid,cpu_name);
	end=MPI_Wtime();
	time=end-start;
	printf("\nTime taken %f:\n",time); 
	MPI_Finalize();
	return 0;

}

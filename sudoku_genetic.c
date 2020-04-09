#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <string.h>
#include <math.h>

int Nd=9;
int flagggg=1;
struct Candidate
{
	int values[9][9];
	float fitness;
};
struct Population
{
	struct Candidate candidates[1000];
};
struct Fixedd
{
	int values[9][9];
};
struct Sudoku{
	int given[9][9];
};
struct temp
{
	int row1[10];
	int row2[10];
};
struct timespec timer_start(){
	struct timespec start_time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
	return start_time;
}
long timer_end(struct timespec start_time){
	struct timespec end_time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
	long diffInNanos = (end_time.tv_sec - start_time.tv_sec) * (long)1e9 + (end_time.tv_nsec - start_time.tv_nsec);
	return diffInNanos;
}
typedef struct Population Population;
typedef struct Candidate Candidate;
typedef struct Fixedd Fixedd;
typedef struct Sudoku Sudoku;
typedef struct temp temp;

Candidate Solution;
float sampleNormal(){

	float u=((float)rand()/(RAND_MAX))*2 - 1;
	float v=((float)rand()/(RAND_MAX))*2 - 1;
	float r= u*u+v*v;
	if(r==0 || r>1)
		return sampleNormal();
	float c=sqrt(-2*log(r)/r);
	return u*c;
}
int is_row_duplicate(Fixedd given, int row, int value){
	for(int column=0;column<Nd; column++){
		if(given.values[row][column]==value){
			return 1;
		}
	}
	return 0;
}
int is_column_duplicate(Fixedd given, int column, int value){
	for(int row=0;row<Nd; row++){
		if(given.values[row][column]==value){
			return 1;
		}
	}
	return 0;
}
int is_block_duplicate(Fixedd given, int row, int column, int value){
	int i = 3 * ((int)row / 3);
    int j = 3 * ((int)column / 3);

        if ((given.values[i][j] == value)
            || (given.values[i][j + 1] == value)
            || (given.values[i][j + 2] == value)
            || (given.values[i + 1][j] == value)
            || (given.values[i + 1][j + 1] == value)
            || (given.values[i + 1][j + 2] == value)
            || (given.values[i + 2][j] == value)
            || (given.values[i + 2][j + 1] == value)
            || (given.values[i + 2][j + 2] == value))
            return 1;
        else
            return 0;
}
int make_index(int v){
        if(v <= 2)
            return 0;
        else if(v <= 5)
            return 3;
        else
            return 6;
}
int no_duplicates(Fixedd given){

	for(int row=0;row<Nd;row++){
		for(int column=0;column<Nd;column++){
			if(given.values[row][column]!=0){

				int c1=0,c2=0,c3=0;
				for(int i=0;i<Nd;i++){
					if(given.values[row][i]==given.values[row][column])
						c1+=1;
				}
				for(int i=0;i<Nd;i++){
					if(given.values[i][column]==given.values[row][column])
						c2+=1;
				}
				int block_values[10];
				int c=0;
				for(int y=make_index(row);y<make_index(row)+3;y++){
					for(int x=make_index(column);x<make_index(column)+3;x++){
						block_values[c++]=given.values[y][x];
					}
				}
				for(int i=0;i<Nd;i++){
					if(block_values[i]==given.values[row][column])
						c3+=1;
				}
				if(c1>1 || c2>1 || c3>1)
					return 0;
			}
		}
	}
	return 1;
}
Candidate compete(Population *population){
	Candidate c1=population->candidates[(rand()%1000)];
	Candidate c2=population->candidates[(rand()%1000)];
	float f1=c1.fitness;
	float f2=c2.fitness;
	Candidate fittest,weakest;
	if(f1>f2){
		fittest=c1;
		weakest=c2;
	}
	else{
		fittest=c2;
		weakest=c1;
	}
	float selection_rate=0.80;
	float r=((float)rand())/RAND_MAX;
	if(r<selection_rate)
		return fittest;
	else
		return weakest;
}
int find_unused(int* parent_row, int* remaining, int rc){
        for(int i=0; i<Nd; i++){
        	for(int j=0;j<rc;j++)
            	if (parent_row[i]==remaining[j])
                	return i;
        }
}
int find_value(int* parent_row, int value){
        for(int i=0; i<Nd; i++){
            if (parent_row[i]==value)
                return i;
        }
}
void printbro(int child_row1[9], int child_row2[9]){
	printf("child_row1\n");
                    for(int i=0;i<9;i++)
                    	printf("%d ",child_row1[i] );
                    printf("\nchild_row2\n");
                    for(int i=0;i<9;i++)
                    	printf("%d ",child_row2[i] );
    printf("\n");
}
void print_remaining(int remaining[9],int rc){
	printf("remaining\n");
	for(int i=0;i<rc;i++)
		printf("%d \n",remaining[i] );
	printf("\n");
}
int index_remaining(int* remaining, int value, int n){
	for(int i=0;i<n;i++){
		if(remaining[i]==value)
			return i;
	}
}
temp crossover_rows(int row1[9], int row2[9]){
	int child_row1[9]={0};
	int child_row2[9]={0};
	int remaining[9]={1,2,3,4,5,6,7,8,9};
	int rc=9;
	int cycle=0;
	int f1=0,f2=0;
	int c1=0,c2=0;
	for(int i=0;i<9;i++){
			if(child_row1[i]!=0)
				c1+=1;
			if(child_row2[i]!=0)
				c2+=1;
		}
		if(c1<9)
			f1=0;
		else
			f1=1;
		if(c2<9)
			f2=0;
		else
			f2=1;
	while(f1==0 && f2==0){
		if(cycle%2==0){
			int index=find_unused(row1, remaining,rc);
			int x=index_remaining(remaining,row1[index],rc);
			child_row1[index] = row1[index];
			int start=row1[index];
			for(int i=x;i<rc-1;i++)
				remaining[i]=remaining[i+1];
			rc-=1;
			child_row1[index] = row1[index];
            child_row2[index] = row2[index];
            int next = row2[index];
            while (next != start){
                    index = find_value(row1, next);
                    x=index_remaining(remaining,row1[index],rc);
                    child_row1[index] = row1[index];
                    for(int i=x;i<rc-1;i++)
						remaining[i]=remaining[i+1];
					rc-=1;
					child_row2[index] = row2[index];
                    next = row2[index];
             }
             cycle += 1;
        }
		else{
                int index = find_unused(row1, remaining, rc);
                int x=index_remaining(remaining,row1[index],rc);
                int start = row1[index];
                for(int i=x;i<rc-1;i++)
					remaining[i]=remaining[i+1];
				rc-=1;
				child_row1[index] = row2[index];
                child_row2[index] = row1[index];
                int next = row2[index];
                int cpo=0;
                while (next != start){
                    index = find_value(row1, next);
                    x=index_remaining(remaining,row1[index],rc);
                    child_row1[index] = row2[index];
                    for(int i=x;i<rc-1;i++)
						remaining[i]=remaining[i+1];
					rc-=1;
                    child_row2[index] = row1[index];
                    next = row2[index];
                }
                cycle += 1;
        }
        c1=0;c2=0;
		for(int i=0;i<9;i++){
			if(child_row1[i]!=0)
				c1+=1;
			if(child_row2[i]!=0)
				c2+=1;
		}
		if(c1<9)
			f1=0;
		else
			f1=1;
		if(c2<9)
			f2=0;
		else
			f2=1;
	}
	struct temp t1;
	memcpy(t1.row1,child_row1,sizeof(int)*9);
	memcpy(t1.row2,child_row2,sizeof(int)*9);
	return t1;
}

Candidate* crossover(Candidate parent1, Candidate parent2, float crossover_rate){
	static Candidate children[2];
	memcpy(children[0].values,parent1.values,sizeof(int)*Nd*Nd);
	children[0].fitness=parent1.fitness;
	memcpy(children[1].values,parent2.values,sizeof(int)*Nd*Nd);
	children[1].fitness=parent2.fitness;
	float r=((float)rand())/RAND_MAX;
	struct temp t2;
	int crossover_point1,crossover_point2;

	if (r < crossover_rate){
            crossover_point1 = rand()%9;
            crossover_point2 = rand()%9 + 1;
            while (crossover_point1 == crossover_point2){
                crossover_point1 = rand()%9;
                crossover_point2 = rand()%9 + 1;
            }
            if (crossover_point1 > crossover_point2){
                int temp = crossover_point1;
                crossover_point1 = crossover_point2;
                crossover_point2 = temp;
            }
            for(int i=crossover_point1;i<crossover_point2;i++){
                t2 = crossover_rows(children[0].values[i], children[1].values[i]);
                memcpy(children[0].values[i],t2.row1,sizeof(int)*9);
                memcpy(children[1].values[i],t2.row2,sizeof(int)*9); 
            }
    }
    return children;
}
void can_init(Candidate *candidate){
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			candidate->values[i][j]=0;
		}
	}
	candidate->fitness=-1;
}
void pop_init(Population *population){
	for(int i=0;i<1000;i++){
		can_init(&population->candidates[i]);
	}
}
void sort(Population *population){
	for(int i=0;i<1000;i++){
		for(int j=0;j<i;j++){
			if(population->candidates[j].fitness>population->candidates[i].fitness){
				Candidate temp;
				float temp1;
				memcpy(temp.values,population->candidates[j].values,sizeof(int)*Nd*Nd);
				temp1=population->candidates[j].fitness;
				memcpy(population->candidates[j].values,population->candidates[i].values,sizeof(int)*Nd*Nd);
				population->candidates[j].fitness=population->candidates[i].fitness;
				memcpy(population->candidates[i].values,temp.values,sizeof(int)*Nd*Nd);
				population->candidates[i].fitness=temp1;
			}
		}
	}
}
void fix_init(Fixedd *given, Candidate candidate){
	memcpy(given->values,candidate.values,sizeof(int)*Nd*Nd);
}
void can_update_fitness(Candidate *candidate){
	int column_count[9]={0};
	int block_count[9]={0};
	float column_sum=0;
	float block_sum=0;

	for(int j=0;j<Nd;j++){
		for(int i=0;i<Nd;i++){
			/*if(flagggg==0)
				printf("%d\n", candidate->values[i][j] );*/
			column_count[candidate->values[i][j]-1]+=1;
		}
		for(int k=0;k<Nd;k++){
			if(column_count[k]==1){
				column_sum+=(float)(1/81.0);
			}
		}
		for(int i=0;i<Nd;i++){
			column_count[i]=0;
		}
	}
	for(int i=0;i<Nd;i+=3){
		for(int j=0;j<Nd;j+=3){
			block_count[candidate->values[i][j] - 1] += 1;
            block_count[candidate->values[i][j + 1] - 1] += 1;
            block_count[candidate->values[i][j + 2] - 1] += 1;

            block_count[candidate->values[i + 1][j] - 1] += 1;
            block_count[candidate->values[i + 1][j + 1] - 1] += 1;
            block_count[candidate->values[i + 1][j + 2] - 1] += 1;

            block_count[candidate->values[i + 2][j] - 1] += 1;
            block_count[candidate->values[i + 2][j + 1] - 1] += 1;
            block_count[candidate->values[i + 2][j + 2] - 1] += 1;

            for(int k=0;k<Nd;k++){
            	if(block_count[k]==1){
            		block_sum+=(1/81.0);
            	}
            }
            for(int k=0;k<Nd;k++){
				block_count[k]=0;
			}
		}
	}
	if((int)column_sum == 1 && (int)block_sum == 1)
            candidate->fitness = 1.0;
    else
            candidate->fitness = column_sum * block_sum;
}
void update_fitness(Population *population){
	for(int i=0;i<1000;i++){
		can_update_fitness(&population->candidates[i]);
	}
}
int mutate(Candidate *candidate, float mutation_rate, Fixedd given){

	float r=((float)rand())/RAND_MAX;
	
	int success=0;

	if(r<mutation_rate){
		while(!success){
			int row1=(rand() % (9));
			int row2=row1;

			int from_column=(rand() % (9));
			int to_column=(rand() % (9));

			while(from_column == to_column){
				from_column=(rand() % (9));
				to_column=(rand() % (9));	//check
			}	

			if(given.values[row1][from_column]==0 && given.values[row1][to_column]==0){
				if(!is_column_duplicate(given, to_column, candidate->values[row1][from_column]) && !is_column_duplicate(given, from_column, candidate->values[row2][to_column]) && !is_block_duplicate(given,row2, to_column, candidate->values[row1][from_column]) && !is_block_duplicate(given, row1, from_column, candidate->values[row2][to_column])){
					int temp=candidate->values[row2][to_column];
					candidate->values[row2][to_column]=candidate->values[row1][from_column];
					candidate->values[row1][from_column]=temp;
					success=1;
				}
			}
		}
	}

	return success;

}
int pop_seed(Population *population, int Nc, Fixedd given){
	pop_init(population);
	int cp=0;
	int cnt[81]={0};
	int helper[9][9][9];
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			for(int k=0;k<9;k++)
				helper[i][j][k]=0;
		}
	}
	for(int row=0;row<Nd;row++){
		for(int column=0;column<Nd;column++){
			int c=0;
			for(int value=1;value<10;value++){
				if ((given.values[row][column] == 0) && !(is_column_duplicate(given,column, value) || is_block_duplicate(given,row, column, value) || is_row_duplicate(given,row, value))){
					cnt[(row*9)+column]=++c;
					helper[row][column][cnt[(row*9)+column]-1]=value;
				}
				else if(given.values[row][column]!=0){
					cnt[(row*9)+column]=++c;
					helper[row][column][cnt[(row*9)+column]-1]=given.values[row][column];
					break;
				}
			}
		}
	}
	
	srand(time(0));
	
	for(int p=0;p<Nc;p++){
		Candidate g;
		can_init(&g);
		for(int i=0;i<Nd;i++){
			int row[9]={0};
			for(int j=0;j<Nd;j++){
				if(given.values[i][j]!=0){
					row[j]=given.values[i][j];
				}
				else if(given.values[i][j]==0){
					row[j]=helper[i][j][(rand() % (cnt[(i*9)+j]))];
				}
			}
			//printf("\n");
			int ii=0;
			int flag=1;
			for(int k=0;k<9;k++){
				for(int v=k+1;v<9;v++){
					if(row[k]==row[v]){
						flag=0;
						break;
					}
				}
			}
			while(flag==0){
				ii+=1;
				if(ii > 500000){
					return 0;
				}
				for(int j=0;j<Nd;j++){
					if(given.values[i][j]==0){
						row[j]=helper[i][j][(rand() % (cnt[(i*9)+j]))];
					}
				}
				flag=1;
				for(int k=0;k<9;k++){
					for(int v=k+1;v<9;v++){
						if(row[k]==row[v]){
							flag=0;
							break;
						}
					}
				}
			}
			memcpy(g.values[i], row, sizeof(int) * Nd);
		}
		for(int a=0;a<Nd;a++){
			for(int b=0;b<Nd;b++){
				population->candidates[cp].values[a][b]=g.values[a][b];
			}
		}
		cp++;
	}
	for(int x=0;x<Nc;x++){
		for(int i=0;i<9;i++){
			for(int j=0;j<9;j++){
				if( population->candidates[x].values[i][j]>=10);
					//printf("hi\n");
			}
		}
	}
	update_fitness(population);
	return 1;
}

int solve(Fixedd given){
	int Nc = 1000;
    int Ne = (int)(0.05 * Nc);
    int Ng = 10000;
    int Nm = 0;
    int phi = 0;
    int sigma = 1;
    float mutation_rate = 0.06;

    can_init(&Solution);

    if(no_duplicates(given)==0){
    	//printf("No no_duplicates\n");
    	return -1;
    }
    Population population;
    pop_init(&population);
    printf("create an initial population.\n");
    if(pop_seed(&population,Nc,given)!=1){
    	return -1;
    }

    int stale=0;
    int stale_cnt=0;
    int theta=given.values[8][1];
    for(int generation=0;generation<Ng;generation++){

    	float best_fitness=0.0;

    	for(int c=0;c<Nc;c++){
    		float fitness= population.candidates[c].fitness;
    		if(fitness>0.99999){
    			printf("Solution found at generation %d!\n",generation);
    			memcpy(Solution.values,population.candidates[c].values,sizeof(int)*Nd*Nd);
    			Solution.fitness=1;
                return generation;
    		}

    		if(fitness>best_fitness)
    			best_fitness=fitness;
    	}
    	printf("Generation:%d, Best fitness:%f\n", generation, best_fitness);

    	Candidate next_population[1000];
    	int np=0;
    	for(int i=0;i<1000;i++){
    		can_init(&next_population[i]);
    	}
    	sort(&population);
    	
    	for(int count=Ne;count<Nc;count+=2){
    		Candidate parent1=compete(&population);
    		Candidate parent2=compete(&population);
    		Candidate childs[2];
    		can_init(&childs[0]);
    		can_init(&childs[1]);
			memcpy(childs,crossover(parent1, parent2, 1.0),sizeof(Candidate)*2);
			can_update_fitness(&childs[0]); 
			float old_fitness=childs[0].fitness;
			int success = mutate(&childs[0],mutation_rate, given);   		
			can_update_fitness(&childs[0]); 

			if(success){
				Nm+=1;
				if(childs[0].fitness>old_fitness)
					phi+=1;
			}

			can_update_fitness(&childs[1]);
			old_fitness=childs[1].fitness;
			success = mutate(&childs[1],mutation_rate, given);   		
			can_update_fitness(&childs[1]); 

			if(success){
				Nm+=1;
				if(childs[1].fitness>old_fitness)
					phi+=1;
			}

			memcpy(next_population[np++].values,childs[0].values,sizeof(int)*Nd*Nd);
			next_population[np].fitness=childs[0].fitness;
			memcpy(next_population[np++].values,childs[1].values,sizeof(int)*Nd*Nd);
			next_population[np].fitness=childs[1].fitness;
    	}
    	for(int e=0;e<Ne;e++){
    		memcpy(next_population[np++].values,population.candidates[e].values,sizeof(int)*Nd*Nd);
    		next_population[np].fitness=population.candidates[e].fitness;
    		//memcpy(elites[ce++].values,
    	}
    	for(int e=0;e<1000;e++){
    		memcpy(population.candidates[e].values,next_population[e].values,sizeof(int)*Nd*Nd);
    		population.candidates[e].fitness=next_population[e].fitness;
    	}
    	update_fitness(&population);
    	if (Nm == 0)
            phi = 0;
        else
            phi = phi/Nm;
        if (phi > 0.2)
            sigma = sigma / 0.998;
        else if (phi < 0.2)
            sigma = sigma * 0.998;
        mutation_rate=((float)rand())/RAND_MAX;

        sort(&population);

        if (population.candidates[0].fitness != population.candidates[1].fitness)
            stale = 0;
        else
            stale += 1;

        if (stale >= 100){
        		stale_cnt+=1;
                printf("The population has gone stale. Re-seeding...\n");
                flagggg=0;
                given.values[8][1]=theta;
                stale = 0;
                sigma = 1;
                phi = 0;
                mutation_rate = 0.06;
        }
        if(stale_cnt>=1000)
        	break;
    }
    printf("No solution found.\n");
    return -2;
}

int main(){
	struct timespec vartime = timer_start();
	Fixedd given;
	int qn[9][9]= {{2,5,0,9,0,0,0,0,0},{6,0,0,4,3,2,0,0,5},{9,0,0,6,5,8,0,2,7},{0,2,0,0,0,0,3,4,0},{0,0,0,0,0,0,0,0,0},{8,0,1,0,4,0,0,5,0},{0,7,9,8,0,0,1,0,0},{0,0,0,0,7,0,5,0,0},{5,0,0,0,2,0,7,0,0}};
	memcpy(given.values,qn,sizeof(int)*Nd*Nd);
	int x=solve(given);
	printf("Proposed solution:\n");
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++)
			printf("%d ",Solution.values[i][j] );
		printf("\n");
	}
	long time_elapsed_nanos = timer_end(vartime);
    printf("Time taken - %ld\n", time_elapsed_nanos);
	return 0;
}

#include "register.h"
extern assamble *assamble_list;
extern int assemble_num;

int *instruction_blocks;
int instruction_block_num;
int block_max_num =  16;// double of max block number

int **data_dep_graph;// for each block
int *assamble_dispatch_index; // array of dispatch
int inst_num_block; //instruction number in current block
int *dispatch_block; // array of dispatch in current block
int current_inst; // the current instruction number in the dispatch block;
int *in_degree; // array of in degree

void inst_block();
void topo_sort(int begin, int end);
void create_dep_graph(int begin, int end);
void do_first(int x);
int adjustSize(void**p, int*size);
void instruction_dispatch()
{
	int i;
	inst_block();
	assamble_dispatch_index = malloc(sizeof(int) * assemble_num);
	for(i = 0 ; i < assemble_num ; i++)	assamble_dispatch_index[i] = i;
	for(i = 0 ; i < instruction_block_num ; i++){
		create_dep_graph(instruction_blocks[i*2], instruction_blocks[i*2+1]);
		topo_sort(instruction_blocks[i*2],instruction_blocks[i*2+1]);
		free(in_degree);
		for(i = 0 ; i < inst_num_block ; i ++){
			free(data_dep_graph[i]);
		}
		free(data_dep_graph);
	}
}

void create_dep_graph(int begin, int end)
{
	int i, j;
	assamble temp;
	inst_num_block = end - begin + 1;
	data_dep_graph = (int **)malloc(sizeof(int *)*inst_num_block);
	for(i = 0 ; i < inst_num_block ; i++){
		data_dep_graph[i] = malloc(sizeof(int) * inst_num_block);
		memset(data_dep_graph[i],0,sizeof(int)* inst_num_block);
	}
	for(i = begin ; i <= end; i++){
		temp = assamble_list[i];
		switch (temp.ins){
			case ldw:{
						 for(j = begin; j < i; j++){
							if(assamble_list[j].ins == ldw || assamble_list[j].ins == stw) data_dep_graph[j][i] = 1;
							else if(assamble_list[j].Rn != -1 && assamble_list[j].Rn == temp.Rd) data_dep_graph[j][i] = 1;
							else if(assamble_list[j].Rs_or_Imm == 1 && assamble_list[j].Rs != -1 && assamble_list[j].Rs == temp.Rd) data_dep_graph[j][i] = 1;
							else if(assamble_list[j].Rm_or_Imm == 1 && assamble_list[j].Rm != -1 && assamble_list[j].Rm == temp.Rd) data_dep_graph[j][i] = 1;						 
						 }
						 for(j = i + 1 ; j <= end; j++){
						 	if(assamble_list[j].ins == ldw || assamble_list[j].ins == stw) data_dep_graph[i][j] = 1;
							else if(assamble_list[j].Rn != -1 && assamble_list[j].Rn == temp.Rd) data_dep_graph[i][j] = 1;
							else if(assamble_list[j].Rs_or_Imm == 1 && assamble_list[j].Rs != -1 && assamble_list[j].Rs == temp.Rd) data_dep_graph[i][j] = 1;
							else if(assamble_list[j].Rm_or_Imm == 1 && assamble_list[j].Rm != -1 && assamble_list[j].Rm == temp.Rd) data_dep_graph[i][j]= 1;
						 }
					 }
			case stw:{
						 for(j = begin ; j < i ; j++){
							if(assamble_list[j].ins == ldw || assamble_list[j].ins == stw) data_dep_graph[j][i] = 1;						 
						 }
						 for(j = i+1 ; j <= end; j++){
						 	if(assamble_list[j].ins == ldw || assamble_list[j].ins == stw) data_dep_graph[i][j] = 1;
						 }
					 }
			default:{
						if(temp.Rd != -1){
							for(j = begin ; j < i ; j++){
								if(assamble_list[j].Rn != -1 && assamble_list[j].Rn == temp.Rd) data_dep_graph[j][i] = 1;
								else if(assamble_list[j].Rs_or_Imm == 1 && assamble_list[j].Rs != -1 && assamble_list[j].Rs == temp.Rd) data_dep_graph[j][i] = 1;
								else if(assamble_list[j].Rm_or_Imm == 1 && assamble_list[j].Rm != -1 && assamble_list[j].Rm == temp.Rd) data_dep_graph[j][i] = 1;						 
							}
							for(j = i ; j <= end; j++){
								if(assamble_list[j].Rn != -1 && assamble_list[j].Rn == temp.Rd) data_dep_graph[i][j] = 1;
								else if(assamble_list[j].Rs_or_Imm == 1 && assamble_list[j].Rs != -1 && assamble_list[j].Rs == temp.Rd) data_dep_graph[i][j] = 1;
								else if(assamble_list[j].Rm_or_Imm == 1 && assamble_list[j].Rm != -1 && assamble_list[j].Rm == temp.Rd) data_dep_graph[i][j]= 1;
							}
						}
					}
		}
	}
	//array of in_degree
	in_degree = (int *)malloc(sizeof(int)*inst_num_block);
	memset(in_degree, 0, sizeof(in_degree));
	for(i = 0 ; i < inst_num_block; i ++){
		for(j = 0 ; j < inst_num_block; j++)
			in_degree[i] += data_dep_graph[j][i];	
	}
}
/*
void find_key_path()
{
	int i, j, degree, flag;
	key_path = malloc(sizeof(int) * inst_num_block);
	for(i = 0 ; i < inst_num_block; i++){
		key_path[i] = -1;
	}
	for(i = inst_num_block - 1 ; i >= 0 ; i--){
		degree = 0;
		for(j = 0 ; j < inst_num_block ; j++) degree += data_dep_graph[i][j];
		if(degree == 0) key_path[i] = 0;
	}
	flag = 1;
	while(flag){
		flag = 0;
		for(i = 0 ; i < inst_num_block ; i++){
			if(key_path[i] != -1){
				for(j = 0 ; j < inst_num_block ; j++){
					if(j == i ) continue;
					if (data_dep_graph[j][i] != 0){
						key_path[j] = key_path[i] + 1;
						flag = 1;
					}
				}		
			}
		}	
	}

}
*/
void topo_sort(int begin , int end)
{
	int i, j, in_degree, flag = 1, temp_index = 0;
	int k, t;
	dispatch_block = malloc(sizeof(int)* inst_num_block);
	memset(dispatch_block, 0, sizeof(dispatch_block));
	current_inst = 0;
	for(i = begin ; i <= end ; i ++){
		if(assamble_list[i].ins == ldw){
			do_first(i);
			for(j = 0 ; j < inst_num_block ; j++){
				if(in_degree[i] == 0 && data_dep_graph[i][j] ==0){
					do_first(j);
					break;
				}
			}
		}	
	}
	flag = 1;
	while(flag){
		flag = 0;
		for(i = begin; i <= end; i++){
			if(in_degree[i] == 0){
				do_first(i);
				flag = 1;
			}
		}
	}
	for(i = 0 ; i < inst_num_block ; i ++) assamble_dispatch_index[begin+i] = dispatch_block[i];
	free(dispatch_block);
}

void do_first(int x)
{
	int i;
	for(i = 0 ; i < inst_num_block ; i++){
		if(data_dep_graph[i][x] != 0){
			do_first(i);		
		}	
	}
	dispatch_block[current_inst++] = x;
	//remove the node in the graph
	for(i = 0 ; i < inst_num_block ; i++){
		if(data_dep_graph[x][i] == 1){
			in_degree[i] -= 1;
			data_dep_graph[x][i] = 0;
		}
	}
	in_degree[x] = -1;
}

void inst_block()
{
	int i, new_block;
	instruction_block_num = 0;
	instruction_blocks = malloc(block_max_num*sizeof(int));
	new_block = 1;
	for(i = 0 ; i < assamble_num ; i++){
		if(new_block == 1){
			if(assamble_list[i].ins==lable)
				if(instruction_block_num*2 == block_max_num){
					adjustSize(&instruction_blocks, sizeof(int)*block_max_num);
				}
				instruction_blocks[instruction_block_num*2] = i;
				new_block = 0;
		}
		if(new_block == 0){
			if(assamble_list[i].ins == lable || assamble_list[i].ins ==b_l || assamble_list[i].ins == jump || assamble_list[i].ins == bne || assamble_list[i].ins == beq || assamble_list[i].ins == bsl || assamble_list[i].ins == beg || assamble_list[i].ins == bel || assamble_list[i].ins == b ||){
				instruction_blocks[instruction_block_num*2+1] = i-1;
				new_block = 1;
				instruction_block_num ++;			
			}		
		}
	}
}

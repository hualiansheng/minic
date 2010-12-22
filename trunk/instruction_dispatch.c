#include "register.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <assert.h>
extern assemble *assemble_list;
extern int assemble_num;

int *instruction_blocks;
int instruction_block_num;
int block_max_num =  16;// double of max block number

int **data_dep_graph;// for each block
int *assemble_dispatch_index; // array of dispatch
int inst_num_block; //instruction number in current block
int *dispatch_block; // array of dispatch in current block
int current_inst; // the current instruction number in the dispatch block;
int *in_degree; // array of in degree

void inst_block();
void topo_sort(int begin, int end);
void create_dep_graph(int begin, int end);
void do_first(int x);
int adjustSize(void**p, unsigned int*size);
void instruction_dispatch()
{
	int i,j;
	inst_block();
	for(i = 0 ; i < instruction_block_num ; i ++){
		printf("%d, %d \n", instruction_blocks[i*2], instruction_blocks[i*2+1]);
	}
	assemble_dispatch_index = malloc(sizeof(int) * assemble_num);
	for(i = 0 ; i < assemble_num ; i++)	assemble_dispatch_index[i] = i;
	for(i = 0 ; i < instruction_block_num ; i++){
		create_dep_graph(instruction_blocks[i*2], instruction_blocks[i*2+1]);
	/*	for(j = 0 ; j < inst_num_block ;j ++){
			for(k = 0 ; k < inst_num_block; k++){
				printf("%d ", data_dep_graph[j][k]);
			}
			printf("\n");
		}*/
		topo_sort(instruction_blocks[i*2],instruction_blocks[i*2+1]);
		free(in_degree);
		for(j = 0 ; j < inst_num_block ; j ++){
			free(data_dep_graph[j]);
		}
		free(data_dep_graph);
	}
	for(i = 0 ;i < assemble_num ;i++){
		printf("%d ",assemble_dispatch_index[i]);
	}
	printf("\n");
}

void create_dep_graph(int begin, int end)
{
	int i, j;
	assemble temp;
	inst_num_block = end - begin + 1;
	data_dep_graph = (int **)malloc(sizeof(int *)*inst_num_block);
	for(i = 0 ; i < inst_num_block ; i++){
		data_dep_graph[i] = malloc(sizeof(int) * inst_num_block);
		memset(data_dep_graph[i],0,sizeof(int)* inst_num_block);
	}
	for(i = begin ; i <= end; i++){
		temp = assemble_list[i];
		switch (temp.ins){
			case ldw:{
						 for(j = begin; j < i; j++){
							if(assemble_list[j].ins == ldw || assemble_list[j].ins == stw) data_dep_graph[j-begin][i-begin] = 1;
							else if(assemble_list[j].Rn != -1 && assemble_list[j].Rn == temp.Rd) data_dep_graph[j-begin][i-begin] = 1;
							else if(assemble_list[j].Rs_or_Imm == 1 && assemble_list[j].Rs_Imm != -1 && assemble_list[j].Rs_Imm == temp.Rd) data_dep_graph[j-begin][i-begin] = 1;
							else if(assemble_list[j].Rm_or_Imm == 1 && assemble_list[j].Rm_Imm != -1 && assemble_list[j].Rm_Imm == temp.Rd) data_dep_graph[j-begin][i-begin] = 1;						 
						 }
						 for(j = i + 1 ; j <= end; j++){
						 	if(assemble_list[j].ins == ldw || assemble_list[j].ins == stw) data_dep_graph[i-begin][j-begin] = 1;
							else if(assemble_list[j].Rn != -1 && assemble_list[j].Rn == temp.Rd) data_dep_graph[i-begin][j-begin] = 1;
							else if(assemble_list[j].Rs_or_Imm == 1 && assemble_list[j].Rs_Imm != -1 && assemble_list[j].Rs_Imm == temp.Rd) data_dep_graph[i-begin][j-begin] = 1;
							else if(assemble_list[j].Rm_or_Imm == 1 && assemble_list[j].Rm_Imm != -1 && assemble_list[j].Rm_Imm == temp.Rd) data_dep_graph[i-begin][j-begin]= 1;
						 }
					 }break;
			case stw:{
						 for(j = begin ; j < i ; j++){
							if(assemble_list[j].ins == ldw || assemble_list[j].ins == stw) data_dep_graph[j-begin][i-begin] = 1;						 
						 }
						 for(j = i+1 ; j <= end; j++){
						 	if(assemble_list[j].ins == ldw || assemble_list[j].ins == stw) data_dep_graph[i-begin][j-begin] = 1;
						 }
					 }break;
			default:{
						if(temp.Rd != -1){
							for(j = begin ; j < i ; j++){
								if(assemble_list[j].Rn != -1 && assemble_list[j].Rn == temp.Rd) data_dep_graph[j-begin][i-begin] = 1;
								else if(assemble_list[j].Rs_or_Imm == 1 && assemble_list[j].Rs_Imm != -1 && assemble_list[j].Rs_Imm == temp.Rd) data_dep_graph[j-begin][i-begin] = 1;
								else if(assemble_list[j].Rm_or_Imm == 1 && assemble_list[j].Rm_Imm != -1 && assemble_list[j].Rm_Imm == temp.Rd) data_dep_graph[j-begin][i-begin] = 1;						 
							}
							for(j = i+1 ; j <= end; j++){
								if(assemble_list[j].Rn != -1 && assemble_list[j].Rn == temp.Rd) data_dep_graph[i-begin][j-begin] = 1;
								else if(assemble_list[j].Rs_or_Imm == 1 && assemble_list[j].Rs_Imm != -1 && assemble_list[j].Rs_Imm == temp.Rd) data_dep_graph[i-begin][j-begin] = 1;
								else if(assemble_list[j].Rm_or_Imm == 1 && assemble_list[j].Rm_Imm != -1 && assemble_list[j].Rm_Imm == temp.Rd) data_dep_graph[i-begin][j-begin]= 1;
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
void topo_sort(int begin , int end)
{
	int i, j,  flag = 1;
	dispatch_block = malloc(sizeof(int)* (inst_num_block+10));
	memset(dispatch_block, 0, sizeof(int) * inst_num_block);
	current_inst = 0;
	for(i = begin ; i <= end ; i ++){
		if(assemble_list[i].ins == ldw && in_degree[i-begin] != -1){
			do_first(i-begin);
			for(j = 0 ; j < inst_num_block ; j++){
				if(in_degree[j] == 0 && j != i+1/*data_dep_graph[i-begin][j] ==0*/){
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
			if(in_degree[i-begin] == 0){
				do_first(i-begin);
				flag = 1;
			}
		}
	}
	//for(i = 0 ; i < inst_num_block ; i++) printf("%d ", dispatch_block[i]);
	for(i = 0 ; i < inst_num_block ; i ++) assemble_dispatch_index[begin+i] = dispatch_block[i] + begin;
	free(dispatch_block);
}

void do_first(int x)
{
	int i;
	for(i = 0 ; i < inst_num_block ; i++){
		if(data_dep_graph[i][x] > 0){
			do_first(i);		
		}	
	}
	assert(current_inst < inst_num_block);
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
	int temp_s;
	instruction_block_num = 0;
	instruction_blocks = malloc(block_max_num*sizeof(int));
	new_block = 1;
	for(i = 0 ; i < assemble_num ; i++){
		if(assemble_list[i].ins == special) continue;
		if(new_block == 1){
			if(assemble_list[i].ins==label)
				if(instruction_block_num*2 == block_max_num){
					temp_s = block_max_num * sizeof(int);
					adjustSize((void **)&instruction_blocks, (unsigned int *)&temp_s);
					block_max_num = temp_s / sizeof(int);
				}
				instruction_blocks[instruction_block_num*2] = i+1;
				new_block = 0;
		}
		else if(new_block == 0){
			if(assemble_list[i].ins ==b_l || assemble_list[i].ins == jump || assemble_list[i].ins == bne || assemble_list[i].ins == beq || assemble_list[i].ins == bsl || assemble_list[i].ins == beg || assemble_list[i].ins == bel || assemble_list[i].ins == b ){
				instruction_blocks[instruction_block_num*2+1] = i-1;
				new_block = 1;
				instruction_block_num ++;
			}
			else if(assemble_list[i].ins == label){
				instruction_blocks[instruction_block_num*2+1] = i -1;
				new_block = 1;
				instruction_block_num ++;
				i--;
			}
		}
	}
}

typedef struct var_vertex {
	int n;
	struct var_vertex* con_vexs;
	int* adj_vexs;
	int deg;
}var_vertex;

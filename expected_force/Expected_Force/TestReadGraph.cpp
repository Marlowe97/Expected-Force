// TestReadGraph.cpp
// Author: Marlowe Chung
// Date: 2018/01/23
// Version: 1.0
// Purpose: Test the reading of graphml files

#include <igraph.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
// #include <unistd.h>		/* unlink */

using namespace std;

void custom_warning_handler(const char *reason, const char *file,
	int line, int igraph_errno) {
	printf("Warning: %s\n", reason);
}

int main(int argc, char **argv) 
{
	double ExpectedForce(igraph_integer_t qnode, igraph_t* graph);

	igraph_t g;
	igraph_error_handler_t* oldhandler;
	igraph_warning_handler_t* oldwarnhandler;
	igraph_bool_t directed = false;
	int result;
	FILE *file;

	errno_t err = fopen_s(&file, "ICO_team.txt", "r");
	if (file == 0) {
		return 10;
	}

	oldhandler = igraph_set_error_handler(igraph_error_handler_ignore);
	oldwarnhandler = igraph_set_warning_handler(custom_warning_handler);
	if ((result = igraph_read_graph_edgelist(&g, file, 0, directed))) {
		// maybe it is simply disabled at compile-time
		cout << result;
		if (result == IGRAPH_UNIMPLEMENTED) return 77;
		return 1;
	}
	igraph_set_error_handler(oldhandler);

	fclose(file);

	/*
	igraph_integer_t a = 1000;
	cout << ExpectedForce(a, &g);
	*/

	ofstream myfile;
	myfile.open("ExF.txt");

	for (int i = 0; i < igraph_vcount(&g); i++)
	{
		myfile << ExpectedForce(i, &g) << endl;
		if (i % 500 == 0) cout << i << endl;
	}

	myfile.close();

	// deallocate memory
	igraph_destroy(&g);
	return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "graph.h"
#include "dlist.h"
#include "list.h"
#include "util.h"

//global var
int num_of_edges;
/* how to run
gcc -std=c99 -Wall -o main is_connected.c graph.c dlist.c
./main airmap1.map

* Implementation of directed graph with adjacency list representation
* Author: Susan Kronberg (id19skg@cs.umu.s)
* Version information: 2021-03-02, v1.0
*/


/*

*/
graph *build_graph(FILE *map){
  graph *g = graph_empty(1000);
  // create buffer string
  char *buff = malloc(sizeof(*buff));
  int buff_size = 150;



  // read file line by line
  while(fgets(buff, buff_size, map) != NULL){

    // check if line is comment, if true skip line
    if (buff[0] == '#') {
      continue;
    }else if(isdigit(buff[0]) ){
      num_of_edges = atoi(&buff[0]);
      printf("%d\n", num_of_edges);
      continue;
    }else{
      char *n1 = malloc(3*sizeof(char));
      char *n2 = malloc(3*sizeof(char));


      sscanf(buff, "%s %s", n1, n2);
      printf("%s %s\n",n1, n2);

      // create nodes and insert edges
      node *origin_node =graph_find_node(g,n1);
      node *destination_node = graph_find_node(g,n2);

      // see if nodes is already in graph: if not -> insert nodes
      if(origin_node == NULL){
        g=graph_insert_node(g,n1);
        origin_node = graph_find_node(g,n1);
      };

      if(destination_node == NULL){
        g=graph_insert_node(g,n2);
        destination_node = graph_find_node(g,n2);
      };
      graph_insert_edge(g,origin_node,destination_node);


    };

    };



  fclose(map);

  free(buff);
  return g;
};




/*

*/
int main(int argc, char *argv[]) {
  // num of argummets must be 2 else print error message
  if(argc != 2){
    fprintf(stderr, "Compile with isConnected 'mapname'.map\n");
    exit(1);
  }

  //open and read second argument passed through file
  FILE *node_map = fopen(argv[1],"r");

  //check if file exists, print error message if not
  if( node_map == NULL){
    fprintf(stderr, "File can not be read\n");
    exit(1);
  };

  graph *graph = build_graph(node_map);

  if(graph_is_empty(graph)){
    printf("graph created and is empty");
  }else{
    printf("graph is not empty\n");
  }

  graph_print(graph);
  return 0;

};

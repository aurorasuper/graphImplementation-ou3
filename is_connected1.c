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


int main(int argc, char *argv[]) {

  graph *g = graph_empty(1000);

  char *ub = "UME BMA";


  char *n1 = malloc(sizeof(*n1));
  char *n2 = malloc(sizeof(*n2));
  sscanf(ub, "%s %s", n1, n2);
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

  graph_print(g);

  char *bu = "BMA UME";
  
  sscanf(bu, "%s %s", n1, n2);
  printf("%s %s\n",n1, n2);

  // create nodes and insert edges
  origin_node =graph_find_node(g,n1);
  destination_node = graph_find_node(g,n2);

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


  graph_print(g);
  return 0;

};

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graph.h"
#include "dlist.h"
#include "list.h"
#include "util.h"
/*
 * Implementation of directed graph with adjacency list representation
 * Author: Susan Kronberg (id19skg@cs.umu.s)
 * Version information: 2021-03-02, v1.0
 */

// =========GLOBAL VARIABLE============
int num_of_nodes;
 // ===========INTERNAL DATA TYPES============

 struct node{
   dlist *neighbours;
    char *name;
   bool seen;
 };

 struct graph{
   dlist *nodes;
 };


 // ===========INTERNAL FUNCTION IMPLEMENTATIONS============

  /**
   * graph_empty() - Create an empty graph.
   * @max_nodes: The maximum number of nodes the graph can hold.
   *
   * Returns: A pointer to the new graph.
   */
  graph *graph_empty(int max_nodes){
    graph *g = calloc(1, sizeof(*g));
    g->nodes = dlist_empty(NULL);
    num_of_nodes = max_nodes;

    return g;
  };

  /**
   * graph_is_empty() - Check if a graph is empty, i.e. has no nodes.
   * @g: Graph to check.
   *
   * Returns: True if graph is empty, otherwise false.
   */
  bool graph_is_empty(const graph *g){
    return dlist_is_empty(g->nodes);
  };


  /**
   * graph_insert_node() - Inserts a node with the given name into the graph.
   * @g: Graph to manipulate.
   * @s: Node name.
   *
   * Creates a new node with a copy of the given name and puts it into
   * the graph.
   *
   * Returns: The modified graph.
   */
  graph *graph_insert_node(graph *g, const char *s){

    // create new node
    node *new_node = malloc(sizeof(*new_node));
    char *node_name = (char*)malloc(sizeof(strlen(s))+1);
    // set links and insert in list of nodes
    strcpy(node_name, s);
    new_node->name = node_name;
    new_node->neighbours = dlist_empty(NULL);
    new_node->seen = false;
    dlist_insert(g->nodes, new_node, dlist_first(g->nodes));

  printf("insert node %s to graph\n", new_node->name);

    //free(new_node);
    return g;
  };


  /**
   * graph_find_node() - Find a node stored in the graph.
   * @g: Graph to manipulate.
   * @s: Node identifier, e.g. a char *.
   *
   * Returns: A pointer to the found node, or NULL.
   */
  node *graph_find_node(const graph *g, const char *s){


    if(dlist_is_empty(g->nodes)){
      return NULL;
    };

    // iterate through list of nodes and return if match exists
    dlist_pos pos = dlist_first(g->nodes);
    while(!dlist_is_end(g->nodes, pos)){

      // create comparing node
      node *inspected = dlist_inspect(g->nodes, pos);

      //printf("compare name %s to node %s stored in graph\n",s,inspected->name);
      if(strncmp(inspected->name, s, strlen(s)+1)==0){
        printf("node found!\n");
        return inspected;
      };

      pos = dlist_next(g->nodes, pos);

    };

    return NULL;

  };

  /**
   * graph_insert_edge() - Insert an edge into the graph.
   * @g: Graph to manipulate.
   * @n1: Source node (pointer) for the edge.
   * @n2: Destination node (pointer) for the edge.
   *
   * NOTE: Undefined unless both nodes are already in the graph.
   *
   * Returns: The modified graph.
   */

  graph *graph_insert_edge(graph *g, node *n1, node *n2){

    node *dest = malloc(sizeof(*dest));
    char *dest_name = (char*)malloc(sizeof(strlen(n2->name))+1);

    // copy name and set links
    strcpy(dest_name, n2->name);
    dest->name = dest_name;
    dest->neighbours = NULL;
    dest->seen = false;

    //add n2 in n1's neighbours list
    dlist_insert(n1->neighbours, dest, dlist_first(n1->neighbours));

    //free(dest_name);
    return g;
  }


  /**
   * graph_print() - Iterate over the graph elements and print their values.
   * @g: Graph to inspect.
   *
   * Iterates over the graph and prints its contents.
   *
   * Returns: Nothing.
   */
  void graph_print(const graph *g){

  // iterate through nodes and print node name
  dlist_pos pos = dlist_first(g->nodes);

  while(!dlist_is_end(g->nodes, pos)){
    node *inspect = dlist_inspect(g->nodes, pos);
    printf("%s ->\n", inspect->name);

    // iterate through nodes neighbours and print neighbour names
    dlist_pos pos2 = dlist_first(inspect->neighbours);

    while(!dlist_is_end(inspect->neighbours, pos2)){
      node *inspect2 = dlist_inspect(inspect->neighbours, pos2);
      printf(" %s\n", inspect2->name);

      pos2 = dlist_next(inspect->neighbours, pos2);

    };


    pos = dlist_next(g->nodes, pos);

  };

};

  /**
   * graph_kill() - Destroy a given graph.
   * @g: Graph to destroy.
   *
   * Return all dynamic memory used by the graph.
   *
   * Returns: Nothing.
   */
  void graph_kill(graph *g){
    // iterate through nodes in graph
    dlist_pos pos1 = dlist_first(g->nodes);
    while(!dlist_is_end(g->nodes, pos1)){
      node *inspect1 = dlist_inspect(g->nodes, pos1);

      //iterate over nodes list of neighbours
      dlist_pos pos2 = dlist_first(inspect1->neighbours);
      while(!dlist_is_end(inspect1->neighbours,pos2)){
        node *inspect2 = dlist_inspect(inspect1->neighbours,pos2);

        // free neighbour
        free(inspect2->name);
        free(inspect2->neighbours);
        free(inspect2);

        pos2=dlist_next(inspect1->neighbours, pos2);
      };

      // kill naighbour list and free node
      dlist_kill(inspect1->neighbours);
      free(inspect1->name);
      free(inspect1);

      pos1 = dlist_next(g->nodes, pos1);
    };

    // kill node list and free graph
    dlist_kill(g->nodes);
    free(g);
  };

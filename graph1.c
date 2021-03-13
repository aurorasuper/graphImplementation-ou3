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
   const char *name;
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

    // set links and insert in list of nodes
    new_node->name = s;
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
      node *inspected = malloc(sizeof(*inspected));
      inspected = dlist_inspect(g->nodes, pos);

      printf("compare name %s to node %s stored in graph\n",s,inspected->name);
      if(inspected->name == s){
        printf("node found!\n");
        return inspected;
      };

      pos = dlist_next(g->nodes, pos);
      //free(inspected);
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

    const char *dest_name = malloc(sizeof(*dest_name));


    // get name of node 2 and insert edge from node 1 to node 2
    dest_name = n2->name;

    dest->name = dest_name;
    dest->neighbours = NULL;
    dest->seen = false;

    // insert edge meaning add n2 in n1's neighbours list
    dlist_insert(n1->neighbours, dest, dlist_first(n1->neighbours));


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

  dlist_pos pos = dlist_first(g->nodes);

  while(!dlist_is_end(g->nodes, pos)){
    node *inspect = malloc(sizeof(*inspect));

    inspect = dlist_inspect(g->nodes, pos);
    printf("%s\n", inspect->name);

    dlist_pos pos2 = dlist_first(inspect->neighbours);

    while(!dlist_is_end(inspect->neighbours, pos2)){
      node *inspect2 = malloc(sizeof(*inspect2));
      inspect2 = dlist_inspect(inspect->neighbours, pos2);
      printf("neighbour %s\n", inspect2->name);

      pos2 = dlist_next(inspect->neighbours, pos2);
      //free(inspect2);
    };

    //free(inspect);
    pos = dlist_next(g->nodes, pos);

  }

  }

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


 // =================== NODE COMPARISON FUNCTION ======================

 /**
  * nodes_are_equal() - Check whether two nodes are equal.
  * @n1: Pointer to node 1.
  * @n2: Pointer to node 2.
  *
  * Returns: true if the nodes are considered equal, otherwise false.
  *
  */
 bool nodes_are_equal(const node *n1,const node *n2){
   char *n1_copy = n1->name;
   char *n2_copy = n2->name;
   if(strcmp(n1_copy, n2_copy)==0){

     return true;
   };

   return false;
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

      // compare node name to char array, if true -> return pointer to the
      // found node
      node *inspected = dlist_inspect(g->nodes, pos);
      if(strncmp(inspected->name, s, strlen(s)+1)==0){
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
    node *dest;
    // iterate through nodes in graph and copy pointer to n2
    dlist_pos  pos = dlist_first(g->nodes);
    while(!dlist_is_end(g->nodes,pos)){
      node* inspect = dlist_inspect(g->nodes, pos);
      if(nodes_are_equal(n2, inspect)){
        dest = inspect;
      }
      pos = dlist_next(g->nodes, pos);
    }
    // insert pointer to n2 in n1's list of neighbours
    dlist_insert(n1->neighbours, dest, dlist_first(n1->neighbours));

    return g;
  };

  /**
   * graph_node_is_seen() - Return the seen status for a node.
   * @g: Graph storing the node.
   * @n: Node in the graph to return seen status for.
   *
   * Returns: The seen status for the node.
   */
  bool graph_node_is_seen(const graph *g, const node *n){
    if(n->seen){
      return true;
    }
    return false;
  };


  /**
   * graph_node_set_seen() - Set the seen status for a node.
   * @g: Graph storing the node.
   * @n: Node in the graph to set seen status for.
   * @s: Status to set.
   *
   * Returns: The modified graph.
   */
  graph *graph_node_set_seen(graph *g, node *n, bool seen){
    n->seen = seen;
    return g;
  };


  /**
   * graph_reset_seen() - Reset the seen status on all nodes in the graph.
   * @g: Graph to modify.
   *
   * Returns: The modified graph.
   */
  graph *graph_reset_seen(graph *g){
    // iterate through nodes and set seen status to false (default)
    dlist_pos node_pos = dlist_first(g->nodes);

    while(!dlist_is_end(g->nodes, node_pos)){
      node *inspected_node = dlist_inspect(g->nodes, node_pos);
      inspected_node->seen = false;

      //iterate through nodes neighbours and set seen status to false (default)
      dlist_pos neighbour_pos = dlist_first(inspected_node->neighbours);
      while (!dlist_is_end(inspected_node->neighbours, neighbour_pos)){
        node *inspected_neighbour = dlist_inspect(inspected_node->neighbours,
        neighbour_pos);
        inspected_neighbour->seen = false;

        neighbour_pos = dlist_next(inspected_node->neighbours, neighbour_pos);
      };

      node_pos = dlist_next(g->nodes, node_pos);
    };
    return g;
  };

  /**
   * graph_neighbours() - Return a list of neighbour nodes.
   * @g: Graph to inspect.
   * @n: Node to get neighbours for.
   *
   * Returns: A pointer to a list of nodes. Note: The list must be
   * dlist_kill()-ed after use.
   */
  dlist *graph_neighbours(const graph *g,const node *n){
    dlist *neighbours;

    // iterate through nodes in graph and get list of neighbours
    dlist_pos pos = dlist_first(g->nodes);
    while (!dlist_is_end(g->nodes,pos)) {
      node *inspected_node = dlist_inspect(g->nodes, pos);
      if(nodes_are_equal(n,inspected_node)){
        neighbours = inspected_node->neighbours;
      }
      pos = dlist_next(g->nodes, pos);
    }

    return neighbours;

  };

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

    // iterate through nodes neighbours and print neighbour names
    dlist_pos pos2 = dlist_first(inspect->neighbours);

    while(!dlist_is_end(inspect->neighbours, pos2)){
      node *inspect2 = dlist_inspect(inspect->neighbours, pos2);
      printf("%s -> %s\n", inspect->name, inspect2->name);

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

      // kill neighbour list and free node
      dlist_kill(inspect1->neighbours);
      free(inspect1->name);

      free(inspect1);

      pos1 = dlist_next(g->nodes, pos1);
    };

    // kill node list and free graph
    dlist_kill(g->nodes);
    free(g);
  };

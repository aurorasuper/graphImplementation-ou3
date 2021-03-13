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
  * graph_has_edges() - Check if a graph has any edges.
  * @g: Graph to check.
  *
  * Returns: True if graph has any edges, otherwise false.
  */
 bool graph_has_edges(const graph *g){
   return false;
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
   node *new_node = malloc(sizeof(*new_node));
   char *node_name = malloc(sizeof(s));


   strncpy(node_name, s, strlen(s));


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
    }

    // iterate over list and return nodematch
    list_pos pos = dlist_first(g->nodes);

    while(!dlist_is_end(g->nodes, pos)){
      node *inspected_node = dlist_inspect(g->nodes, pos);

      // compare name with stored node names
      printf("compare name %s and stored node name %s\n",s,inspected_node->name );
      if(*inspected_node->name == *s){
        return inspected_node;
      };



      pos = dlist_next(g->nodes, pos);
      //free(inspected_node);
    };

    printf("%s is not stored in the graph\n", s);
    return NULL;
 };


 /**
  * graph_node_is_seen() - Return the seen status for a node.
  * @g: Graph storing the node.
  * @n: Node in the graph to return seen status for.
  *
  * Returns: The seen status for the node.
  */
  /*
 bool graph_node_is_seen(const graph *g, const node *n){

 }
*/
 /**
  * graph_node_set_seen() - Set the seen status for a node.
  * @g: Graph storing the node.
  * @n: Node in the graph to set seen status for.
  * @s: Status to set.
  *
  * Returns: The modified graph.
  */
  /*
 graph *graph_node_set_seen(graph *g, node *n, bool seen){

 }
*/
 /**
  * graph_reset_seen() - Reset the seen status on all nodes in the graph.
  * @g: Graph to modify.
  *
  * Returns: The modified graph.
  */
  /*
 graph *graph_reset_seen(graph *g){

 }
*/
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

   char *dest_name = malloc(sizeof(*dest_name));


   // get name of node 2 and insert edge from node 1 to node 2
   dest_name = n2->name;

   dest->name = dest_name;
   dest->neighbours = NULL;
   dest->seen = false;

   // insert edge meaning add n2 in n1's neighbours list
   dlist_insert(n1->neighbours, dest, dlist_first(n1->neighbours));
  // printf("inserting edge from %s to %s\n",n1->name, n2->name );
   free(dest_name);
   return g;
 }

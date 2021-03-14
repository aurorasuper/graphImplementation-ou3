#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "graph.h"
#include "dlist.h"
#include "util.h"
#include "queue.h"
#include "list.h"

//global var
int num_of_edges;
/* how to run
gcc -std=c99 -Wall -o main is_connected.c graph.c dlist.c
./main airmap1.map

* Implementation of directed graph with adjacency list representation
* Author: Susan Kronberg (id19skg@cs.umu.s)
* Version information: 2021-03-02, v1.0
*/


  /**
   * ingore_white_space()- check if a char array starts with white spaces,
   * ignore them if true
   * @g: char to inspect.
   *
   * Iterates over the graph and prints its contents.
   *
   * Returns: NULL if line only consists of white spaces,
   * else modified line with removed initial white spaces
   */
char *ignore_white_space(char *line){
  bool white_space = true;
  int length = strlen(line);
  int char_count = 0;
  while(white_space){
    // se if char at char_count is white space character
    if(isspace(line[char_count])!=0){
      char_count++;
      // if the whole line contains spaces set line value to null and jump
      // out of loop
      if(length == char_count){
        line = 0;
        white_space = false;
      };

    }else{
      /* see if line has contained white space (char_count=0),
      * if true -> jump out of loop without modifying
      * if false -> modify line to remove white spaces
      */
      if(char_count == 0){
        white_space=false;
      }else{
        // allocate space for a line that will copy the non white_space chars
        char *new_line = calloc(256,sizeof(char));

        /* copy the chars from the first non-white-space to the new char array,
         * modify and overwrite the old chars in the original char array*/
        for(int j = 0; j< length; j++){
          new_line[j]= line[char_count];
          line[j]=new_line[j];
          char_count++;
        }
        free(new_line);
        white_space = false;
      }
    }
  };

  return line;
};

/* build_graph() - builds graph from file
* @map - File from which graph is built
*
* NOTE - File must contain same amount of edges as declared
*
* Returns - A new graph with nodes and edges
*/
graph *build_graph(FILE *map){
  // create buffer string assbmad each line contains less than 256 chars
  graph *g = NULL;
  int buff_size = 256;
  char *buff = calloc(buff_size,sizeof(char));
  int edge_count = 0;



  // read file line by line
  while(fgets(buff, buff_size, map) != NULL){


    // check if line is comment, if true skip line
    if (buff[0] == '#') {
      continue;

    //check if line starts with a digit, indicates number of edges in graph
    // store create graph with max number of nodes and continue
    }else if(isdigit(buff[0]) ){
      num_of_edges = atoi(&buff[0]);
      g = graph_empty(2*num_of_edges);
      continue;
    }else{

      /* checks if buffer starts with white spaces and removes them if true
      * returns NULL if buffer only contains whitespaces, if true -> continue to
      * next line
      */
      char *line = ignore_white_space(buff);
      if(line == NULL){
        continue;
      };

      //node names can be maximum of 40 chars
      char *origin_name = (char*)malloc(40*sizeof(char)+1);
      char *dest_name = (char*)malloc(40*sizeof(char)+1);

      // see if nodes is already in graph: if not -> insert nodes

      node *origin_node =graph_find_node(g,origin_name);
      node *destination_node = graph_find_node(g,dest_name);

      if(origin_node == NULL){
        g=graph_insert_node(g,origin_name);
        origin_node = graph_find_node(g,origin_name);
      };

      if(destination_node == NULL){
        g=graph_insert_node(g,dest_name);
        destination_node = graph_find_node(g,dest_name);
      };

      graph_insert_edge(g,origin_node,destination_node);

      edge_count++;
      free(origin_name);
      free(dest_name);
    };

  };

  if(edge_count != num_of_edges){
    fprintf(stderr, "inserted %d edges when the needes to be %d edges\n",
    edge_count, num_of_edges );
    fclose(map);
    free(buff);
    graph_kill(g);
    exit(EXIT_FAILURE);
  };


  fclose(map);
  free(buff);
  return g;
};

void add_to_queue(queue *q, graph *g, node *n){
  queue_enqueue(q,n);
  graph_node_set_seen(g, n, true);
}

/* find_path() - See if path exists between source node and destination node.
* @g - Graph to inspect1.
* @src - Source node.
* @dest - Destination node.
*
* Returns - true if path exists, else false
*/
bool find_path(graph *g,node *src,node *dest){
   node *src_node = src;
   node *dest_node = dest;
   queue *q = queue_empty(NULL);

   add_to_queue(q,g,src_node);

   while(!queue_is_empty(q)){
     // get node from queue and check if node is destination node
     // and get neighbour list from node first in queue
     node *first_node = queue_front(q);
     dlist *neighbours = graph_neighbours(g, first_node);

     //iterate through neighbour list
     dlist_pos pos = dlist_first(neighbours);
     while(!dlist_is_end(neighbours, pos)){
       // get node from list and check if node is destination node
       node *inspected_node = dlist_inspect(neighbours, pos);
       if(nodes_are_equal(inspected_node, dest_node)){

         queue_kill(q);
         graph_reset_seen(g);
         return true;
       };
       // if neighbour node is not seen, add to queue
       if(!graph_node_is_seen(g, inspected_node)){
         add_to_queue(q,g,inspected_node);
       };

       pos = dlist_next(neighbours, pos);
     };

     // when all neighbours have been checked delete from queue
     queue_dequeue(q);
   };

  queue_kill(q);

  graph_reset_seen(g);
  return false;
};

/*

*/
int main(int argc, char *argv[]) {
  // num of argummets must be 2 else print error message
  if(argc != 2){
    fprintf(stderr, "Compile with isConnected 'mapname'.map\n");
    exit(EXIT_FAILURE);
  }

  //open and read second argbmant passed through file
  FILE *node_map = fopen(argv[1],"r");

  //check if file exists, print error message if not
  if( node_map == NULL){
    fprintf(stderr, "File can not be read\n");
    exit(EXIT_FAILURE);
  };

  graph *graph = build_graph(node_map);

  bool running = true;
  char *input = malloc(2*40*sizeof(char));
  //node names can be maximum of 40 chars
  char *origin_name = (char*)malloc(40*sizeof(char)+1);
  char *dest_name = (char*)malloc(40*sizeof(char)+1);
  while(running){


    printf("Enter origin and destination (quit to exit): ");
    fgets(input, 2*40, stdin);
    sscanf(input, "%s %s", origin_name, dest_name);
    node *origin_node = graph_find_node(graph, origin_name);
    node *dest_node = graph_find_node(graph, dest_name);
    if(strcmp(origin_name, "quit")==0){
      running = false;
    };

    if(origin_node == NULL){
      printf("No origin exists with that name. Try Again.\n");
      continue;
    }
    if(dest_node == NULL){
      printf("No destination exists with that name. Try Again.\n");
      continue;
    }
    if(find_path(graph, origin_node, dest_node)){
      printf("There is a path from %s to %s.\n", origin_name, dest_name);
    }else{
      printf("There is no path from %s to %s.\n", origin_name, dest_name);
    }
  };

  free(input);
  free(origin_name);
  free(dest_name);
  graph_kill(graph);
  return 0;

};

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
  printf(" length of line %d\n", length);
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
/*

*/
graph *build_graph(FILE *map){
  // create buffer string assumed each line contains less than 256 chars
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
      printf("%d\n", num_of_edges);
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


      sscanf(line, "%s %s", origin_name, dest_name);
      //printf("%s %s\n",origin_name, dest_name);

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




/*

*/
int main(int argc, char *argv[]) {
  // num of argummets must be 2 else print error message
  if(argc != 2){
    fprintf(stderr, "Compile with isConnected 'mapname'.map\n");
    exit(EXIT_FAILURE);
  }

  //open and read second argument passed through file
  FILE *node_map = fopen(argv[1],"r");

  //check if file exists, print error message if not
  if( node_map == NULL){
    fprintf(stderr, "File can not be read\n");
    exit(EXIT_FAILURE);
  };

  graph *graph = build_graph(node_map);

  

  graph_print(graph);
  graph_kill(graph);
  return 0;

};

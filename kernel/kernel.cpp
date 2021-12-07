#include <cassert>
#include <iostream>
#include <limits>

#include "kernel.h"
#include "omp.h"

using std::cout;
using std::endl;

int THD_COUNT = 1;

using std::string;

void normalize(csr_t* snaph, array2d_t<float> & matrix) {
  
    for (int i = 0 ; i < matrix.row_count ; i++) {
        matrix.row_normalize(i, snaph->get_degree(i));
    }
}



//Takes max of nebrs and applies it
void max(csr_t* snaph, array2d_t<float> & input, array2d_t<float> & output){
	for (int i = 0; i < output.row_count; i++) {
         output.row_add(input.data_ptr + i*output.col_count, i); 
         int max = 0;
        for (int j = offset[i] ; j < offset[i+1] ; j++) {
              if (max < nebrs[j]*output.col_count) {
              max = nebrs[j]*output.col_count;
              
              }
          }

        for (int j = offset[i] ; j < offset[i+1] ; j++) { 
              output.row_add(input.data_ptr + max, nebrs[j]);
        }
      }


}
//Takes mean of nebrs
void mean(csr_t* snaph, array2d_t<float> & input, array2d_t<float> & output){
	for (int i = 0; i < output.row_count; i++) {
         output.row_add(input.data_ptr + i*output.col_count, i); 
         int sum = 0;
         int count = 0;
        for (int j = offset[i] ; j < offset[i+1] ; j++) {
              sum = sum + nebrs[j]*output.col_count;
              count = count + 1;
          }
        int mean = sum/count;
        for (int j = offset[i] ; j < offset[i+1] ; j++) { 
              output.row_add(input.data_ptr + mean, nebrs[j]);
        }
      }

}

//Takes sum of nebrs
void sum(csr_t* snaph, array2d_t<float> & input, array2d_t<float> & output){
	for (int i = 0; i < output.row_count; i++) {
         output.row_add(input.data_ptr + i + output.col_count, i); 
        
        for (int j = offset[i] ; j < offset[i+1] ; j++) {
              output.row_add(input.data_ptr + nebrs[j] + output.col_count, nebrs[j]);
          }
      }

}


void _gspmm(csr_t* snaph, array2d_t<float> & input, array2d_t<float> & output,
                     op_t op, char* graph_pooling, bool reverse, bool norm /*= true*/)
{
    //cout << "spmm " << op << "reverse = " << reverse << endl;

    //If in backward, normalize it first, else normalize it after computation

    //The core logic goes here.
  
  
  
    if(graph_pooling == "sum"){
    normalize(snaph, input);
    sum(snaph, input, output);
    }
    else if(graph_pooling == "mean"){
    normalize(snaph, input);
    mean(snaph, input, output);
    }
    else{
      normalize(snaph, input);
      max(snaph, input, output);
    }

    /*int64_t columnCount = output.columnCount;
    int64_t rowCount = output.rowCount;
    vid_t* nbrs = snaph->get_nebrs();
    vid_t* offset = snaph->get_offset();
    vid_t v_count = snaph->get_vcount();

    for(vid_t i = 0; i <v_count; i++){
    vid_t deg = snaph->get_degree(node)
    if(reverse == 0){
    	for (vid_t nbrID = offset[i]; nbrID < offset[i]+ deg; nbrID++){
    		output.row_add(input.operator[](nbrs[nbrID]), i);
    	}
    	output.row_normalize(i, deg);
    }
    else {
    	input.row_normalize(i, deg);
    	for (vid_t nbrID = offset[i]; nbrID < offset[i]+ deg; nbrID++){
    		output.row_add(input.operator[](nbrs[nbrID]), i);
    	}
    } */
   

}

void invoke_gspmm(graph_t& graph, array2d_t<float> & input_array, array2d_t<float> & output_array,
                 bool reverse, bool norm /*= true*/)
{
    if (reverse) {
         return _gspmm(&graph.csr, input_array, output_array, eSUM, "sum", reverse, norm);
    } else {
         return _gspmm(&graph.csc, input_array, output_array, eSUM, "sum", reverse, norm);
    }
}

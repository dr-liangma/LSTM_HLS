#include "lstm_hls.h"
#include "params.h"
#include "hls_stream.h"

void position(const DataType input[DataSize][Capa_In_Size + Infrared_In_Size],
		DataType p[DataSize][Output_Size]){
#pragma HLS INTERFACE m_axi depth=393*20 port=input bundle=gmem
#pragma HLS INTERFACE m_axi depth=393*2 port=input bundle=gmem
#pragma HLS INTERFACE s_axilite port=return bundle=control
#pragma HLS INTERFACE s_axilite port=input bundle=control
#pragma HLS INTERFACE s_axilite port=p bundle=control

	static const int InputSize = Capa_In_Size + Infrared_In_Size;
	static const int LSTM_SIZE = 16;
	
	static const DataType WI[InputSize][LSTM_SIZE * 4] ={
#include "expr_0.txt"
	};
	static const DataType WS[LSTM_SIZE][LSTM_SIZE * 4] = {
#include "expr_1.txt"
	};
	static const DataType BiasS[LSTM_SIZE * 4]={
#include "expr_2.txt"
	};

	static const DataType W1[LSTM_SIZE][Output_Size] ={
#include "expr_3.txt"
	};
	static const DataType Bias1[Output_Size]={
#include "expr_4.txt"
	};

#pragma HLS DATAFLOW
	hls::stream<DataType> out_stream;
#pragma HLS STREAM variable=out_stream depth=LSTM_SIZE dim=1
	lstm<InputSize, LSTM_SIZE>(input, out_stream, BiasS, WI, WS,DataSize);
	feed_forward<LSTM_SIZE, Output_Size>(out_stream, p, W1, Bias1,DataSize);
}

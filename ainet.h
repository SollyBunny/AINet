
#ifndef AINET_H
#define AINET_H

#ifndef N_TYPE
	#define N_TYPE unsigned int
#endif
#define N_MAX (((0x1ULL << ((sizeof(N_TYPE) * 8ULL) - 1ULL)) - 1ULL) | (0xFULL << ((sizeof(N_TYPE) * 8ULL) - 4ULL)))
#define N_RAND() ((N_TYPE)rand() * N_MAX / RAND_MAX)

typedef struct {
	unsigned int  numin;
	N_TYPE       *in;
	unsigned int  numout;
	N_TYPE       *out;
	unsigned int  layers;
	unsigned int  width;
	unsigned int  numconnections;
	N_TYPE       *connections;
	unsigned int  numnodes;
	N_TYPE       *nodes;
	unsigned int *sortout;
	void         *data; // used for storing data along with each net (fitness)
} Net;
// Allocate memory for a net 
Net *n_init(
	unsigned int numin, unsigned int numout,
	unsigned int layers, unsigned int width
);
// Free memory for a net
void *n_free(Net *net);
// Set all weights to random values
void n_setrand(Net *net);
// Copy one net's weight to another's
void n_setcopy(Net *net, Net *other);
// Take inputs and calculate the outputs
void n_calc(Net *net);
// Internal function
int n_sortout_compare(const void *a, const void *b);
// Sort the output nodes by value
N_TYPE *n_sortout(Net *net);
// Slightly mutates a net's weights
void n_mutate(Net *net);
// Save the net into a file
int n_save(Net *net, const char file[]);
// Load a file into a net
int n_load(Net *net, const char file[]);
// Save the net into a viewable svg file
int n_save_svg(Net *net, const char file[]);

typedef struct {
	Net **net;
	unsigned int num;
} Nets;
// Allocate memory for nets
Nets *ng_init(
	unsigned int num,
	unsigned int numin, unsigned int numout,
	unsigned int layers, unsigned int width
);
// Free memory for nets
void *ng_free(Nets *net);
// Set all weights to random values
void ng_setrand(Nets *net);
// Calculate all outputs from inputs
void ng_calc(Nets *net);

#endif

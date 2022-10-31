
#include "ainet.h"
#include <stdio.h>
#include <stdlib.h>


// Allocate memory for a net 
Net *n_init(
	unsigned int numin, unsigned int numout,
	unsigned int layers, unsigned int width
) {
	Net *net            = malloc(sizeof(Net));
	net->numin          = numin;
	net->numout         = numout;
	net->layers         = layers;
	net->width          = width;

	net->numconnections = (numin * width) + ((layers - 1) * width * width) + (width * numout);
	net->connections    = malloc(sizeof(N_TYPE) * net->numconnections);
	
	net->numnodes       = numin + numout + (layers * width);
	net->nodes          = malloc(sizeof(N_TYPE) * net->numnodes);
	net->in             = net->nodes;
	net->out            = net->nodes + numin + (layers * width);
	
	net->sortout        = malloc(sizeof(unsigned int) * net->numout);
	for (unsigned int i = 0; i < net->numout; ++i) {
		net->sortout[i] = i;
	}

	return net;
}
// Free memory for a net
void *n_free(Net *net) {
	free(net->connections);
	free(net->nodes);
	free(net);
}
// Set all weights to random values
void n_setrand(Net *net) {
	for (unsigned int i = 0; i < net->numconnections; ++i)
		net->connections[i] = N_RAND();
}
// Copy one net's weight to another's
void n_setcopy(Net *net, Net *other) {
	for (unsigned int i = 0; i < net->numconnections; ++i)
		other->connections[i] = net->connections[i];
}
// Take inputs and calculate the outputs
void n_calc(Net *net) {
	for (unsigned int i = net->numin; i < net->numnodes; ++i)
		net->nodes[i] = N_MAX / 2;
	int connection = 0;
	long long int t = 0;
	for (unsigned int i = 0; i < net->numin; ++i) {
		for (unsigned int m = 0; m < net->width; ++m) {
			t = (long long int)net->connections[connection] * (long long int)net->nodes[i] / (long long int)(N_MAX / 2);
			net->nodes[net->numin + m] += (N_TYPE)t;
			++connection;
		}
	}
	for (unsigned int y = 0; y < net->layers - 1; ++y) {
		for (unsigned int x = 0; x < net->width; ++x) {
			for (unsigned int m = 0; m < net->width; ++m) {
				t = (long long int)net->connections[connection] * (long long int)net->nodes[net->numin + y * net->width + x] / (long long int)(N_MAX / 2);
				net->nodes[net->numin + (y + 1) * net->width + m] += (N_TYPE)t;
				++connection;
			}
		}
	}
	for (unsigned int i = 0; i < net->width; ++i) {
		for (unsigned int m = 0; m < net->numout; ++m) {
			t = (long long int)net->connections[connection] * (long long int)net->nodes[net->numin + (net->layers * net->width) + i] / (long long int)(N_MAX / 2);
			net->out[m] += (N_TYPE)t;
			++connection;
		}
	}
}
// Sort the output nodes by value
static Net *n_sortout_curnet;
int n_sortout_compare(const void *a, const void *b) {
	a = (const void*)(long)n_sortout_curnet->out[*(N_TYPE*)a];
	b = (const void*)(long)n_sortout_curnet->out[*(N_TYPE*)b];
	if ((N_TYPE)(long)a == (N_TYPE)(long)b) return 0;
	if ((N_TYPE)(long)a >  (N_TYPE)(long)b) return 1;
	return -1;
}
N_TYPE *n_sortout(Net *net) {
	n_sortout_curnet = net;
	qsort(net->sortout, net->numout, sizeof(net->sortout[0]), &n_sortout_compare);
	return net->sortout;
}
// Slightly mutates a net's weights
void n_mutate(Net *net) {
  for (unsigned int i = 0; i < 10; ++i) {
    net->connections[rand() % net->numconnections] =  N_RAND();
  }
}

/*
	Save Node into a file
	Format:
		"?NET" (4)
		size of decimal format in bytes (1)
		numin  (sizeof unsigned int)
		numout (sizeof unsigned int)
		layers (sizeof unsigned int)
		width  (sizeof unsigned int)
		connections (sizeof decimal * (numin * width) + ((layers - 1) * width))
*/
int n_save(Net *net, const char file[]) {
	FILE *fp = fopen(file, "wb");
	if (fp == 0) return -1;
	fprintf(fp, "?NET%c%c%c%c%c",
		(char)sizeof(N_TYPE),
		net->numin, net->numout,
		net->layers, net->width
	);
	unsigned int written = fwrite(net->connections, sizeof(N_TYPE), net->numconnections, fp);
	if (written != net->numconnections) { // not everything was written
		return -1;
		fclose(fp);
	};
	fflush(fp);
	fclose(fp);
	return 0;
}

/*
	Save Node into a file in .svg format (can be viewed on most browsers)
*/
int n_save_svg(Net *net, const char file[]) {
	FILE *fp = fopen(file, "wb");
	if (fp == 0) return -1;
	#define SIZE 10
	#define YGAP 100
	#define XGAP 100
	#define PAD 30
	unsigned int width = (net->numin > net->numout) ? net->numin : net->numout;
	width = (width > net->width) ? width : net->width;
	unsigned int height = 2 + net->layers;
	fprintf(fp, "<svg xmlns=\"http://www.w3.org/2000/svg\" background-color=\"#222\" width=\"%u\" height=\"%u\"><rect width=\"100%%\" height=\"100%%\" fill=\"#222\" />", 
		(width ) * XGAP - PAD,
		(height) * YGAP - PAD
	);

	unsigned int connection = 0;
		// draw input connections
			for (unsigned int i = 0; i < net->numin; ++i) {
				for (unsigned int m = 0; m < net->width; ++m) {
					fprintf(fp, "<line x1=\"%f\" y1=\"%u\" x2=\"%f\" y2=\"%u\" stroke=\"hsl(%f,100%%,50%%)\" stroke-widh=\"1\" />",
						(double)(i+ (double)(width - net->numin) / 2) * XGAP + PAD,
						PAD,
						(double)(m+ (double)(width - net->width) / 2) * XGAP + PAD,
						PAD + YGAP,
						(double)net->connections[connection] / (double)N_MAX * 120.f
					);
					++connection;
				}
			}
		// draw hidden connections
			for (unsigned int y = 0; y < net->layers - 1; ++y) {
				for (unsigned int x = 0; x < net->width; ++x) {
					for (unsigned int m = 0; m < net->width; ++m) {
						fprintf(fp, "<line x1=\"%f\" y1=\"%u\" x2=\"%f\" y2=\"%u\" stroke=\"hsl(%f,100%%,50%%)\" stroke-widh=\"1\" />",
							(double)(x + (double)(width - net->width) / 2) * XGAP + PAD,
							(y + 1) * YGAP + PAD,
							(double)(m + (double)(width - net->width) / 2) * XGAP + PAD,
							(y + 2) * YGAP + PAD,
							(double)net->connections[connection] / (double)N_MAX * 120.f
						);
						++connection;
					}
				}
			}
		// draw output connections
			for (unsigned int i = 0; i < net->width; ++i) {
				for (unsigned int m = 0; m < net->numout; ++m) {
					fprintf(fp, "<line x1=\"%f\" y1=\"%u\" x2=\"%f\" y2=\"%u\" stroke=\"hsl(%f,100%%,50%%)\" stroke-widh=\"1\" />",
						(double)(i + (double)(width - net->width) / 2) * XGAP + PAD,
						(net->layers) * YGAP + PAD,
						(double)(m + (double)(width - net->numout) / 2) * XGAP + PAD,
						(net->layers + 1) * YGAP + PAD,
						(double)net->connections[connection] / (double)N_MAX * 120.f
					);
					++connection;
				}
			}
			
	connection = 0;
		// draw input nodes
		for (unsigned int i = 0; i < net->numin; ++i) {
			fprintf(fp, "<circle cx=\"%f\" cy=\"%u\" r=\"%u\" fill=\"hsl(0,0%%,%f%%)\" />",
				(double)(i + (double)(width - net->numin) / 2) * XGAP + PAD,
				PAD,
				SIZE,
				(double)net->nodes[connection] / (double)N_MAX * 100.f
			);
			++connection;
		}
		// draw hidden nodes
		for (unsigned int y = 0; y < net->layers; ++y) {
			for (unsigned int x = 0; x < net->width; ++x) {
				fprintf(fp, "<circle cx=\"%f\" cy=\"%u\" r=\"%u\" fill=\"hsl(0,0%%,%f%%)\" />",
					(double)(x + (double)(width - net->width) / 2) * XGAP + PAD,
					(y + 1) * YGAP + PAD,
					SIZE,
					(double)net->nodes[connection] / (double)N_MAX * 100.f
				);
				++connection;
			}
		}
		// draw output nodes
		for (unsigned int i = 0; i < net->numout; ++i) {
			fprintf(fp, "<circle cx=\"%f\" cy=\"%u\" r=\"%u\" fill=\"hsl(0,0%%,%f%%)\" />",
				(double)(i + (double)(width - net->numout) / 2) * XGAP + PAD,
				(net->layers + 1) * YGAP + PAD,
				SIZE,
				(double)net->nodes[connection] / (double)N_MAX * 100.f
			);
			++connection;
		}

	fprintf(fp, "</svg>");
	fflush(fp);
	fclose(fp);
	return 0;
}

// Allocate memory for nets
Nets *ng_init(
	unsigned int num,
	unsigned int numin, unsigned int numout,
	unsigned int layers, unsigned int width
) {
	Nets *net = malloc(sizeof(Nets));
	net->net = malloc(sizeof(Net*) * num);
	net->num = num;
	for (unsigned int i = 0; i < num; ++i) {
		net->net[i] = n_init(numin, numout, layers, width);
	}
	return net;
}
// Free memory for nets
void *ng_free(Nets *net) {
	for (unsigned int i = 0; i < net->num; ++i) {
		n_free(net->net[i]);
	}
	free(net->net);
	free(net);
}
// Set all weights to random values
void ng_setrand(Nets *net) {
	for (unsigned int i = 0; i < net->num; ++i) {
		n_setrand(net->net[i]); 
	}
}
// Calculate all outputs from inputs
void ng_calc(Nets *net) {
	for (unsigned int i = 0; i < net->num; ++i) {
		n_calc(net->net[i]); 
	}
}


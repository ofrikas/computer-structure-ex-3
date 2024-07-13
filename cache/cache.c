/* 
    Ofri Kastenbaum 208708768
    Orel Shai 
*/

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar;

// a struct for a cache line
typedef struct cache_line_s {
    uchar valid;
    uchar frequency;
    long int tag;
    uchar* block;
} cache_line_t;

// a struct for a cache
typedef struct cache_s {
    uchar s;
    uchar t;
    uchar b;
    uchar E;
    int S;
    int B;
    cache_line_t** cache;
} cache_t;

cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E) {
    cache_t new_cache;
    new_cache.s = s;
    new_cache.t = t;
    new_cache.b = b;
    new_cache.E = E;

    // S = 2^s, B = 2^b. S is the number of sets, B is the number of bytes in a block
    new_cache.S = 1 << s;
    new_cache.B = 1 << b;

    // allocate memory for the cache lines and blocks 
    new_cache.cache = malloc(new_cache.S * sizeof(cache_line_t*)); // allocate memory for the sets
    for (int i = 0; i < new_cache.S; i++) {
        new_cache.cache[i] = malloc(E * sizeof(cache_line_t)); // allocate memory for the lines in the set
        for (int j = 0; j < E; j++) { // initialize the lines in the set
            new_cache.cache[i][j].valid = 0;
            new_cache.cache[i][j].frequency = 0;
            new_cache.cache[i][j].tag = 0;
            new_cache.cache[i][j].block = (uchar*)malloc(new_cache.B * sizeof(uchar));
        }
    }
    return new_cache;
}

int* addressCalculator (long int off, cache_t* cache) {
    /*
        calculate the tag, set index and block offset:
            off is the address of the byte we want to read
            tag - is the upper bits of the address so we shift off by s+b
            set - index is the middle bits of the address so we shift off by b and mask with 2^s-1
            block offset - is the lower bits of the address so we mask off with 2^b-1
    */
    long int tag = off >> (cache->s + cache->b);

    // Step 1: Shift off right by cache.b to remove block offset portion
    int shifted_off_for_set_index = off >> cache->b;
    // Step 2: Calculate mask for set index
    int set_index_mask = cache->S - 1; // -1 is for switching from 2^s to 2^s-1 for masking
    // Step 3: Apply mask to obtain set index
    int set_index = shifted_off_for_set_index & set_index_mask; // now we have the set index
 
    int block_offset = off & (cache->B - 1); // agian -1 is for switching from 2^b to 2^b-1 for masking

    int* address = malloc(3 * sizeof(int));
    address[0] = tag;
    address[1] = set_index;
    address[2] = block_offset;
    return address;
}

// update the frequency of use number of the cache line
void update_frequency(cache_t* cache, int set_index, int line_index) {
        cache->cache[set_index][line_index].frequency++;
}

    /*
        write_byte is used to write to the cache and "RAM". 
        The cache we implement should replace lines using the LFU (least frequently used) method, meaning
        if it tries to read in memory to a set with no available lines then it replaces the line whose frequency is
        minimal. If two lines both have the same minimal frequency, then the first line is chosen
    */
void write_byte(cache_t cache, uchar* start, long int off, uchar new) {
    // calculate the address
    int* address = addressCalculator(off, &cache);
    int tag = address[0];
    int set_index = address[1];
    int block_offset = address[2];

    // searching for the line in the set to write to or to overwrite
    int line_index = 0;
    int min_freq_line = 0;
    uchar min_freq = cache.cache[set_index][0].frequency;
    for (int i = 0; i < cache.E; i++) {
        // if the line is already in the cache overwrite it. We can say read_byte should have been called before write_byte
        if (cache.cache[set_index][i].valid && cache.cache[set_index][i].tag == tag) {
            line_index = i;
            break;
        }
        // if the line is not valid we can write to it
        if (!cache.cache[set_index][i].valid) {
            line_index = i;
            break;
        }
        // if the line is valid and the frequency is lower than the minimum frequency
        if (cache.cache[set_index][i].frequency < min_freq) {
            min_freq = cache.cache[set_index][i].frequency;
            min_freq_line = i;
        }
    }

    cache.cache[set_index][line_index].valid = 1;
    cache.cache[set_index][line_index].tag = tag;
    cache.cache[set_index][line_index].block[block_offset] = new;
    update_frequency(&cache, set_index, line_index);
    start[off] = new; // Write to "RAM" memory
}

uchar read_byte(cache_t cache, uchar* start, long int off) {
    // calculate the address
    int* address = addressCalculator(off, &cache);
    int tag = address[0];
    int set_index = address[1];
    int block_offset = address[2];

    // searching for the line in the set
    for (int i = 0; i < cache.E; i++) { 
        // if the line is valid and the tag matches its hit
        if (cache.cache[set_index][i].valid && cache.cache[set_index][i].tag == tag) { 
            update_frequency(&cache, set_index, i);
            return cache.cache[set_index][i].block[block_offset]; // hit: return the byte
        }
    }

    // Miss: Load a cache line into cache
    long int start_address = off - block_offset; // the start address of the block
    for (int i = 0; i< cache.B;i++)
        write_byte(cache, start, start_address + i , start[start_address + i]); 
    return start[off];
}

// print the cache as provided in the exercise
void print_cache(cache_t cache) {
    int S = 1 << cache.s; 
    int B = 1 << cache.b;

    for (int i = 0; i < S; i++) { // for each set
        printf("Set %d\n", i);
        for (int j = 0; j < cache.E; j++) { // for each line
            printf("%1d %d 0x%0*lx ", cache.cache[i][j].valid,cache.cache[i][j].frequency, cache.t, cache.cache[i][j].tag);
            for (int k = 0; k < B; k++) { // for each byte in the block
                printf("%02x ", cache.cache[i][j].block[k]);
            }
        puts("");
        }
    }
 }

// main function as provided in the exercise
int main() {
    int n;
    printf("Size of data: ");
    scanf("%d", &n);
    uchar* mem = malloc(n);
    printf("Input data >> ");
    for (int i = 0; i < n; i++)
        scanf("%hhd", mem + i);

    int s, t, b, E;
    printf("s t b E: ");
    scanf("%d %d %d %d", &s, &t, &b, &E);
    cache_t cache = initialize_cache(s, t, b, E);

    while (1) {
        scanf("%d", &n);
        if (n < 0) break;
        read_byte(cache, mem, n);
    }

    puts("");
    print_cache(cache);

    free(mem);
}
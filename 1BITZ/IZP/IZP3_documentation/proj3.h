/**
 * @file        proj3.h
 * @author      David Bolvansky (xbolva00)
 *
 * @brief       The implementation of the Single-linkage clustering
 *
 * @date        13 December 2015
 *
 */


 /** @defgroup GROUP1 Operations with the array of clusters
 *
 * Functions to work with the array of clusters
 *
 */
 
 /** @defgroup GROUP2 Cluster operations
 *
 * Functions to work with a cluster
 *
 */
 
 
 /**
* @brief Cluster object structure
*
* Every cluster object contains: \n
* - Object ID \n
* - X and Y coordinates \n
*/
struct obj_t {
    /// ID of the object
    int id;
    /// X coordinate of the object
    float x;
    /// Y coordinate of the object
    float y;
};

/**
* @brief Cluster structure
*
* Every cluster contains: \n
* - The number of reserved objects in the memory \n
* - The number of current objects in the cluster \n
* - The array of cluster objects
*/
struct cluster_t {
    /// Current number of objects in the cluster
    int size;
    /// Maximal allowed (reserved) number of objects in the cluster
    int capacity;
    /// Array of objects in the cluster, NULL if uninitialized
    struct obj_t *obj;
};

/** @addtogroup GROUP2 */
/*@{*/


/**
* @brief Inicializes cluster and allocates memory for its objects
*
* Inicializes cluster 'c'. \n
* Allocates memory for the required number of objects (see cap). \n
* Zero capacity means NULL pointer for array of clusters. 

* @param    c       Cluster (to be inicialized)
* @param    cap     Capacity (for allocation)
* @pre      Cluster 'c' is not NULL and cap is greater then zero.
* @post     Allocated memory for cluster objects is not NULL.
*/
void init_cluster(struct cluster_t *c, int cap);

/**
* @brief Clears cluster and reinicializes it to empty cluster.
*
* Clears all cluster objects and inicializes cluster to empty cluster. \n
* Sets cluster size and capacity to zero.
*
* @param    c       Cluster (to be cleared)
* @post     Cluster 'c' is empty and it was fully cleared from the memory.
*/
void clear_cluster(struct cluster_t *c);

/**
 * @var CLUSTER_CHUNK
 * @brief A value recommended for reallocation
 */
extern const int CLUSTER_CHUNK;

/**
* @brief Resizes cluster to the new capacity.
*
* Resizes cluster, if needed.
* Changes capacity of the cluster 'c' to the new capacity 'new_cap'..

* @param    c           Cluster (to be resized)
* @param    new_cap     New capacity (for reallocation)
* @pre      Cluster 'c' is not NULL.
* @pre      Capacity of the cluster 'c' is equal or greater than zero.
* @pre      New capacity is equal or greater than zero.
* @post     After reallocation, the new allocated memory for cluster objects is not NULL.
* @return   Resized cluster with a new capacity 'new_cap'
*/
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
* @brief Appends cluster with a new object.
*
* Adds a object 'obj' to the end of cluster 'c'. \n
* Resizes cluster, if there is no capacity for the new object 'obj'.

* @param    c       Cluster (to be appended)
* @param    obj     Object (to be added to the end of cluster 'c')
* @pre      'c' cannot point to NULL. 
* @post     After reallocation, the new allocated memory for cluster objects is not NULL.
*/
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
* @brief Merges two clusters into the first cluster.
*
* Adds objects from the cluster 'c2' into the cluster 'c1'. \n
* If needed, cluster 'c1' will be resized. \n
* Objects in the cluster 'c1' will be sorted by their IDs. \n
* Cluster 'c2' stays unchanged.
*
* @param    c1      Pointer to the first cluster (to be merged with the cluster 'c2')
* @param    c2      Pointer to the second cluster
* @pre      Clusters 'c1' and 'c2' must not point to NULL.
*/


void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/*@}*/

/** @addtogroup GROUP1 */
/*@{*/

/**
* @brief Removes cluster from the array of clusters.
*
* Removes the cluster 'c' from the array of clusters 'carr'. \n
* The array has 'narr' objects (clusters).
* The cluster to be removed is located at an index 'idx'.
* Returns new size of the array of clusters.
*
* @param    carr    Pointer to the array of clusters
* @param    narr    Number of clusters in the array
* @param    idx     Index of the cluster (to be removed) in the array of clusters
* @return   New size of the array of clusters
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/*@}*/

/** @addtogroup GROUP2 */
/*@{*/
/**
* @brief Calculates distance between two objects.
*
* Counts distance between two objects using Euclidean distance.
* 
* @param    o1      Pointer to the first object
* @param    o2      Pointer to the second object
* @pre      Objects 'o1' and 'o2' must not point to NULL.
* @return   The shortest distance between two objects.
*/
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
* @brief Calculates distance between two clusters.
*
* Counts distance between two clusters. \n
* The distance is calculated on the basis of the closest neighbour.
* 
* @param    c1      Pointer to the first cluster
* @param    c2      Pointer to the second cluster
* @return   The shortest distance between two clusters.
* @pre      Clusters 'c1' and 'c2' must not point to NULL.
* @pre      Sizes of the both clusters are greater than zero.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/*@}*/

/** @addtogroup GROUP1 */
/*@{*/


/**
* @brief Finds two closest neighbours. 
*
* In the array of clusters 'carr' with size'narr' finds two closest clusters (according to closest neighbour).
* Gets the indexes of found closest clusters. \n
* Saves found clusters (indexes for the array 'carr') to the memory on adresses 'c1', 'c2'
*
* @param    carr    Pointer to the array of clusters
* @param    narr    Number of clusters in the array
* @param    c1      Pointer on the first number (first index to be stored here)
* @param    c2      Pointer on the second number (second index to be stored here)
* @pre      'c1' must not point to NULL. 
* @pre      'c2' must not point to NULL. 
* @pre      'carr' must not point to NULL. 
* @pre      Number of clusters in the array 'narr' is greater than zero.
*/

void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/*@}*/

/** @addtogroup GROUP2 */
/*@{*/

/**
* @brief Sorts objects in a cluster
*
* Sorts all objects in the cluster 'c' using Quick Sort by their Object IDs.
*
* @param    c   Cluster (to be sorted)
* @pre     'carr' must not point to NULL.
*/
void sort_cluster(struct cluster_t *c);

/**
* @brief Prints data of a cluster.
*
* Print all data for the cluster 'c' to stdout. \n
* Prints its Object ID, X and Y coordinates.
*
* @param     c      Cluster (to be printed)
* @pre      'c' must not point to NULL.
*/
void print_cluster(struct cluster_t *c);

/*@}*/

/** @addtogroup GROUP1 */
/*@{*/

/**
* @brief Loads clusters from a file.

* Load objects from 'filename'. Creates cluster for every object and adds it to the array of clusters. \n
* Allocates memory for the array of clusters it saves the pointer to the first item of the array, where 'arr' points. \n
* Returns the number of loaded objects (clusters). If any error, 'arr' is NULL.
*
* @param    filename    File with objects
* @param    arr         Pointer to the first item of the array of clusters
* @post     Array of clusters 'arr' must not point to NULL
* @return   Number of loaded objects
*/
int load_clusters(char *filename, struct cluster_t **arr);

/**
* @brief Prints data of all clusters.
*
* Prints the array of clusters. \n
* Paramater 'carr' is the pointer to the first item (cluster). \n
* Prints first 'narr' objects. \n
* Prints their Object IDs, X and Y coordinates.
*
* @param    carr    Pointer to the array of the clusters
* @param    narr    Number of clusters in the array
*/
void print_clusters(struct cluster_t *carr, int narr);

/*@}*/

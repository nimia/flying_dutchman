#ifndef QUEUE__PER_VERTEX_DATA_H_
#define QUEUE__PER_VERTEX_DATA_H_

typedef struct Queue__Per_Vertex_Data {
	struct list_head equi_distance_vertices;
} Queue__Per_Vertex_Data;

static inline void Queue__Per_Vertex_data__init(Queue__Per_Vertex_Data *data)
{
	INIT_LIST_HEAD(&data->equi_distance_vertices);
}

#endif

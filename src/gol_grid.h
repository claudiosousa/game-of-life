
struct gol_grid_t;
typedef struct gol_grid_t gol_grid_t;

gol_grid_t *gol_grid_create(size_t width, size_t height);
void gol_grid_copy(gol_grid_t *gol_grid_dst, gol_grid_t *gol_grid_src);
void gol_grid_init(gol_grid_t *gol_grid, double seed, double alive_prob);
//bool gol_grid_get_next_state(gol_t *gol_grid, double seed, double alive_prob);
bool gol_grid_is_alive(gol_grid_t *gol_grid, size_t x, size_t y);
void gol_grid_get_size(gol_grid_t *gol_grid, size_t *x, size_t *y);
//neightbours, set_state, get_state
void gol_grid_free(gol_grid_t *gol_grid)
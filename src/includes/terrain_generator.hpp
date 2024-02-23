#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class TerrainGenerator {
public:
    static void gen_two_random_points(int terrain_size, glm::vec2& a,glm::vec2& b);
    static void normalize_height_map(std::vector<float>& height_map, int max_h, int min_h);
    static void apply_fir_filter(std::vector<float>& height_map, float filter_val);
    static std::vector<float> gen_fault_formation(int terrain_size,int iterations,float max_h,float min_h,float fir_filter);
};
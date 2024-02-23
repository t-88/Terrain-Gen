#include "terrain_generator.hpp"

void TerrainGenerator::gen_two_random_points(int terrain_size, glm::vec2& a,glm::vec2& b) {
    a.x = rand() % terrain_size;
    a.y = rand() % terrain_size;
    b.x = rand() % terrain_size;
    b.y = rand() % terrain_size;

    int counter = 0;    
    while (a == b && counter < 1000) {
        a.x = rand() % terrain_size;
        a.y = rand() % terrain_size;
        b.x = rand() % terrain_size;
        b.y = rand() % terrain_size;
        counter += 1;
    }
}
void TerrainGenerator::normalize_height_map(std::vector<float>& height_map, int max_h, int min_h) {
    float max = height_map[0], min = height_map[0];
    for (size_t i = 1; i < height_map.size(); i++) {
        if(height_map[i] > max)  max = height_map[i];
        else if(height_map[i] < min) min = height_map[i];
    }

    for (size_t i = 0; i < height_map.size(); i++) {
        height_map[i] = (height_map[i] - min) / (max - min) * (max_h - min_h) + min_h;
    }
}
void TerrainGenerator::apply_fir_filter(std::vector<float>& height_map, float filter_val) {
    int height_map_size = sqrt(height_map.size());


    // left to right
    for (int z = 0; z < height_map_size; z++) {
        for (int x = 1; x < height_map_size; x++) {
            height_map[x + z * height_map_size] = 
                height_map[x + z * height_map_size] + 
                (height_map[x - 1 + z * height_map_size] - height_map[x + z * height_map_size]) * filter_val; 
        }
    }

    // right to left
    for (int z = 0; z < height_map_size; z++) {
        for (int x = height_map_size - 2; x >= 0; x--) {
            height_map[x + z * height_map_size] = 
                 height_map[x + z * height_map_size] + 
                (height_map[x + 1 + z * height_map_size] - height_map[x + z * height_map_size]) * filter_val; 
        }
    }   

    // bottom to top
    for (int x = 0; x < height_map_size; x++) {
        for (int z = 1; z < height_map_size; z++) {
            height_map[x + z * height_map_size] = 
                height_map[x + z * height_map_size] + 
                (height_map[x  + (z - 1) * height_map_size] - height_map[x + z * height_map_size]) * filter_val; 
        }
    }

    // top to bottom
    for (int x = 0; x < height_map_size; x++) {
        for (int z = height_map_size - 2; z >= 0; z--) {
            height_map[x + z * height_map_size] = 
                 height_map[x + z * height_map_size] + 
                (height_map[x + (z + 1) * height_map_size] - height_map[x + z * height_map_size]) * filter_val; 
        }
    }   
 
}
std::vector<float> TerrainGenerator::gen_fault_formation(int terrain_size,int iterations,float max_h,float min_h,float fir_filter) {
    float delta_h = max_h - min_h;
    std::vector<float> height_map(terrain_size * terrain_size,min_h);


    for (size_t cur_iter = 0; cur_iter < iterations; cur_iter++) {
        float iter_ratio = (float) cur_iter / (float) iterations;
        float height = max_h - delta_h * iter_ratio;

        glm::vec2 a, b;
        gen_two_random_points(terrain_size,a,b);
        glm::vec2 line = a - b;
        
        for (size_t z = 0; z < terrain_size; z++) {
            for (size_t x = 0; x < terrain_size; x++) {
                
                glm::vec2 dir = glm::vec2(x,z) - a;
                if(dir.x * line.y - dir.y * line.x > 0) {
                    height_map[x + z * terrain_size] = height_map[x + z * terrain_size] + height;
                } 
            }
        }
        
    }

    normalize_height_map(height_map,max_h,min_h);
    TerrainGenerator::apply_fir_filter(height_map,fir_filter);

    return height_map;
}

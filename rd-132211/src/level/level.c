#include "level/level.h"

char* readFully(Level* this, FILE* file) {
    s32 size = this->width * this->height * this->depth;
    char* uncompressed_data = malloc(size);

    if (!file) {
        fclose(file);
        file = fopen("level.dat", "w");
        if (!file) {
            perror("Failed to open file");
            return uncompressed_data;
        }
        fclose(file);
    }

    fseek(file, 0, SEEK_END);
    s32 file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size == 0) {
        return uncompressed_data;
    }

    u8* gzip_data = (u8*)malloc(file_size);
    if (!gzip_data) {
        fclose(file);
        perror("Failed to allocate memory for compressed data");
        return NULL;
    }

    fread(gzip_data, 1, file_size, file);

    uLongf buffer_size = file_size * 10; // Arbitrary
    if (!uncompressed_data) {
        free(gzip_data);
        perror("Failed to allocate memory for uncompressed data");
        return uncompressed_data;
    }

    s32 result = uncompress((unsigned char*)uncompressed_data, &buffer_size, gzip_data, file_size);

    free(gzip_data);

    if (result != Z_OK) {
        free(uncompressed_data);
        fprintf(stderr, "Failed to uncompress data: %d\n", result);
        return NULL;
    }

    return uncompressed_data;
}

bool write(FILE* file, char* data) {
    if (!file) {
        perror("Failed to open file");
    }

    if (!data) {
        fprintf(stderr, "data has no value");
    }
    
    uLongf file_size = strlen(data);

    u8* gzip_data = (u8*)malloc(file_size);
    if (!gzip_data) {
        fclose(file);
        perror("Failed to allocate memory for compressed data");
        return 0;
    }

    s32 result = compress(gzip_data, &file_size, data, file_size);

    if (result != Z_OK) {
        free(gzip_data);
        fprintf(stderr, "Failed to compress data: %d\n", result);
        return 0;
    }

    fwrite(gzip_data, 1, file_size, file);

    free(gzip_data);

    return 1;
}

void addLevelListener(ArrayList* list, LevelListener* value) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * sizeof(LevelListener*));
    }
    ((LevelListener**)list->data)[list->size++] = value;
}

void getLevelListener(ArrayList* list, size_t index, LevelListener* retVal) {
    if (index < list->size) {
        *retVal = *((LevelListener**)list->data)[index];
    } else {
        retVal = NULL;
    }
}

void removeLevelListener(ArrayList* list, LevelListener* value) {
    for (size_t i = 0; i < list->size; i++) {
        if (((LevelListener**)list->data)[i] == value) {
            for (size_t j = i; j < list->size - 1; j++) {
                ((LevelListener**)list->data)[j] = ((LevelListener**)list->data)[j + 1];
            }
            list->size--;
        }
    }
}

Level* newLevel(s32 w, s32 h, s32 d) {
    Level* level = malloc(sizeof(Level));

    level->width = w;
    level->height = h;
    level->depth = d;
    level->blocks = calloc(w * h * d, sizeof(char));
    level->lightDepths = calloc(w * h, sizeof(s32));
    level->LevelListeners = newArrayList(10);

    for (s32 x = 0; x < w; x++) {
        for (s32 y = 0; y < d; y++) {
            for (s32 z = 0; z < h; z++) {
                s32 i = (y * level->height + z) * level->width + x;
                level->blocks[i] = (y <= d * 2 / 3) ? 1 : 0;
            }
        }
    }

    calcLightDepths(level, 0, 0, w, h);
    load(level);

    return level;
}

void load(Level* this) {
    LevelListener levelListener;
    // FILE* dis = fopen("level.dat", "rb");
    // this->blocks = readFully(this, dis); //! allocs memory smaller than curr amount
    calcLightDepths(this, 0, 0, this->width, this->height);
    for (s32 i = 0; i < this->LevelListeners->size; i++) {
        getLevelListener(this->LevelListeners, i, &levelListener);
        levelListener.allChanged(levelListener.base);
    }

    // fclose(dis);
}

void save(Level* this) {
    FILE* dos = fopen("level.dat", "wb");
    write(dos, this->blocks);
    fclose(dos);
}

void calcLightDepths(Level* this, s32 x0, s32 y0, s32 x1, s32 y1) {
    LevelListener levelListener;

    for (s32 x = x0; x < x0 + x1; x++) {
        for (s32 z = y0; z < y0 + y1; z++) {
            s32 oldDepth = this->lightDepths[x + z * this->width];
            s32 y = this->depth - 1;
            while (y > 0 && !isLightBlocker(this, x, y, z))
                y--;
            
            this->lightDepths[x + z * this->width] = y;

            if (oldDepth != y) {
                s32 yl0 = (oldDepth < y) ? oldDepth : y;
                s32 yl1 = (oldDepth > y) ? oldDepth : y;
                for (s32 i = 0; i < this->LevelListeners->size; i++) {
                    getLevelListener(this->LevelListeners, i, &levelListener);
                    levelListener.lightColumnChanged(levelListener.base, x, z, yl0, yl1);
                }
            }
        }
    }
}

void addListener(Level* this, LevelListener* levelListener) {
    addLevelListener(this->LevelListeners, levelListener);
}

void removeListener(Level* this, LevelListener* levelListener) {
    removeLevelListener(this->LevelListeners, levelListener);
}

bool isTile(Level* this, s32 x, s32 y, s32 z) {
    if (x < 0 || y < 0 || z < 0 || x >= this->width || y >= this->depth || z >= this->height)
        return FALSE;
    else {
        return this->blocks[(y * this->height + z) * this->width + x] == 1;
    }
}

bool isSolidTile(Level* this, s32 x, s32 y, s32 z) {
    return isTile(this, x, y, z);
}

bool isLightBlocker(Level* this, s32 x, s32 y, s32 z) {
    return isSolidTile(this, x, y, z);
}

ArrayList* getCubes(Level* this, AABB* aABB) {
    s32 x0 = aABB->x0;
    s32 x1 = aABB->x1 + 1.0f;
    s32 y0 = aABB->y0;
    s32 y1 = aABB->y1 + 1.0f;
    s32 z0 = aABB->z0;
    s32 z1 = aABB->z1 + 1.0f;

    ArrayList* aABBs = newArrayList(10);

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (z0 < 0) z0 = 0;
    if (x1 > this->width) x1 = this->width;
    if (y1 > this->width) y1 = this->depth;
    if (z1 > this->width) z1 = this->height;

    for (s32 x = x0; x < x1; x++) {
        for (s32 y = y0; y < y1; y++) {
            for (s32 z = z0; z < z1; z++) {
                if (isSolidTile(this, x, y, z))
                    addAABB(aABBs, newAABB(x, y, z, (x + 1), (y + 1), (z + 1))); 
            }
        }
    }
    return aABBs;
}

float getBrightness(Level* this, s32 x, s32 y, s32 z) {
    float dark = 0.8f;
    float light = 1.0f;

    if (x < 0 || y < 0 || z < 0 || x >= this->width || y >= this->depth || z >= this->height)
        return light;
    if (y < this->lightDepths[x + z * this->width])
        return dark;
    return light;
}

void setTile(Level* this, s32 x, s32 y, s32 z, s32 type) {
    LevelListener levelListener;

    if (x < 0 || y < 0 || z < 0 || x >= this->width || y >= this->depth || z >= this->height)
        return;
    this->blocks[(y * this->height + z) * this->width + x] = type;
    calcLightDepths(this, x, z, 1, 1);
    for (s32 i = 0; i < this->LevelListeners->size; i++) {
        getLevelListener(this->LevelListeners, i, &levelListener);
        levelListener.tileChanged(levelListener.base, x, y, z);
    }
}
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void createConfigFile() {
    FILE* config = fopen("config.txt", "w");
    if (config) {
        fprintf(config, "webp: https://example.com\n");
        fclose(config);
    }
}

const char* getWebpFromConfig() {
    FILE* config = fopen("config.txt", "r");
    static char webpValue[256]; // Assuming a reasonable max length for the value
    webpValue[0] = '\0'; // Initialize as an empty string

    if (config) {
        char line[256];
        while (fgets(line, sizeof(line), config)) {
            if (strstr(line, "webp:") == line) {
                // Found the 'webp' line, extract the value
                sscanf(line, "webp: %[^\n]", webpValue);
                break; // No need to continue reading
            }
        }
        fclose(config);
    }

    return webpValue;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL initialization error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Web Browser",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        fprintf(stderr, "Window creation error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    FILE* config = fopen("config.txt", "r");
    if (!config) {
        createConfigFile();
    } else {
        fclose(config);
    }

    const char* webp = getWebpFromConfig();

    printf("WebP value from config: %s\n", webp);

    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

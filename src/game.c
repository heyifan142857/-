//
// Created by user on 2024/1/17.
//
#include <game.h>

static Uint32 tick;
Uint32 start_time;
Uint32 current_time;
Uint32 tnt_time;
Uint32 elapsed_time = 0;
Uint32 reminder;

LinkedList blocks_list;
Blocks *block;

int scores;
int difficulty;
bool fail;
bool restart;
bool automatic;
static bool RKeyPressed = false;
static Kun kun;
static bool gathering = false;
static bool jumping = false;
static Mix_Chunk *chunk_1;
static Mix_Chunk *chunk_2;
static Mix_Chunk *chunk_fail;
static Mix_Chunk *chunk_jump;
static Mix_Chunk *chunk_tnt;
double percentage = 0;
int channel_1;
int channel_2;
int shape;
char *block_path[] = {"./res/img/blocks_game/005.png",
                      "./res/img/blocks_game/015.png",
                      "./res/img/blocks_game/Block_of_Diamond_JE5_BE3.png",
                      "./res/img/blocks_game/Cobblestone_JE5_BE3.png",
                      "./res/img/blocks_game/Crafting_Table_JE4_BE3.png",
                      "./res/img/blocks_game/Glass_JE4_BE2.png",
                      "./res/img/blocks_game/Grass_Block_JE7_BE6.png",
                      "./res/img/blocks_game/Oak_Planks_JE6_BE3.png",
                      "./res/img/blocks_game/Dirt_JE2_BE2.png",
                      "./res/img/blocks_game/Block_of_Bamboo_JE3_BE2.png",
                      "./res/img/blocks_game/Oak_Leaves_JE4.png"
};
char *dish_path[] = {
        "./res/img/blocks_game/OIG2.1JM78Zo-removebg-preview.png",
        "./res/img/blocks_game/OIG2-removebg-preview.png"
};
char *tnt_path[] = {
        "./res/img/blocks_game/TNT_JE3_BE2.png"
};
char *Moist[] = {
        "./res/img/blocks_game/Moist_Farmland_JE4_BE2.png"
};
char *Dirt[] = {
        "./res/img/blocks_game/Dirt_JE2_BE2.png"
};

void do_game_logic(){
    scores = 0;
    difficulty = 0;

    app.keyboard[SDL_SCANCODE_SPACE] = false;
    fail = false;
    restart = false;
    automatic = false;
    reminder = SDL_GetTicks();

    init_kun(&kun);
    init_blocks();

    chunk_1 = Mix_LoadWAV("./res/chunk/chunk_1.mp3");
    chunk_2 = Mix_LoadWAV("./res/chunk/chunk_2.mp3");
    chunk_fail = Mix_LoadWAV("./res/chunk/fail.mp3");
    chunk_jump = Mix_LoadWAV("./res/chunk/jump.mp3");
    chunk_tnt = Mix_LoadWAV("./res/chunk/tnt.mp3");

    srand((unsigned int)time(NULL));

    while(!app.keyboard[SDL_SCANCODE_ESCAPE] && !restart){
        do_game_input();

        draw_game();

        do_fps();

        difficulty = scores / 300;
    }
    quit_kun(&kun);
    quit_blocks();

    Mix_FreeChunk(chunk_1);
    Mix_FreeChunk(chunk_2);
    Mix_FreeChunk(chunk_fail);
    Mix_FreeChunk(chunk_jump);
    Mix_FreeChunk(chunk_tnt);
}

static void do_game_input(){
    SDL_Event event;

    while (SDL_PollEvent(&event)){
        do_event(event);
    }

    if (app.keyboard[SDL_SCANCODE_R]) {
        if (!RKeyPressed) {
            RKeyPressed = true;
            automatic = !automatic;
        }
    } else {
        RKeyPressed = false;
    }

    if(!jumping){
        if(!automatic || fail){
            if(app.keyboard[SDL_SCANCODE_SPACE]){
                reminder = SDL_GetTicks();
                if(fail){
                    restart = true;
                }else if(!gathering){
                    tick = SDL_GetTicks();
                    gathering = true;
                    channel_1 = Mix_PlayChannel(-1,chunk_1,0);
                }else{
                    if( (SDL_GetTicks() - tick) > LEN_CHUNK){
                        channel_2 = Mix_PlayChannel(-1,chunk_2,-1);
                    }
                    if(percentage < 1){
                        percentage = (SDL_GetTicks() - tick) / LEN_CHUNK;
                    }else{
                        percentage = 1;
                    }
                }

            }
            if(!app.keyboard[SDL_SCANCODE_SPACE] && gathering){
                reminder = SDL_GetTicks();
                Uint32 duration = SDL_GetTicks() - tick;
                tick =  SDL_GetTicks();
                Mix_FadeOutChannel(channel_1, 700);
                Mix_FadeOutChannel(channel_2, 10);
                printf("Press the space bar for %u\n",duration);
                gathering = false;
                kun.dh = speed_calculator_h(duration);
                jumping = true;
                percentage = 0;
                start_time = SDL_GetTicks();
                current_time = SDL_GetTicks();
            }
            if(blocks_list.middle->shape == 2 && (SDL_GetTicks() - tnt_time) > 3384){
                fail = true;
            }
        }else{
            if(!fail){
                do_auto();
            }
        }
    }else {
        current_time = SDL_GetTicks();
        elapsed_time = current_time - start_time;

        kun.h += kun.dh * 0.001 * elapsed_time;
        kun.dh -= KUN_ACCELERATION * 0.001 * elapsed_time;
        for (Blocks *p = blocks_list.head; p != NULL; p = p->after) {
            p->x -= KUN_SPEED * 0.001 * elapsed_time;
        }


        if(kun.h < 0){
            reminder = SDL_GetTicks();
            if(blocks_list.middle->x <= 432.0 && blocks_list.middle->x + blocks_list.middle->size>= 358 && !fail){
                printf("LAND\n");
                jumping = false;
                kun.h = 0;
                kun.dh = 0;
                if(blocks_list.middle->shape == 2){
                    Mix_PlayChannel(-1,chunk_tnt,0);
                    tnt_time = SDL_GetTicks();
                }else{
                    Mix_PlayChannel(-1,chunk_jump,0);
                }
                if(blocks_list.middle->shape == 3){
                    do_Moist();
                }
            }else if(blocks_list.middle->after->x <= 432.0 && blocks_list.middle->after->x + blocks_list.middle->after->size>= 358 && !fail){
                Blocks *temp = blocks_list.tail;
                block = malloc(sizeof(Blocks));
                (*block).size = (int)(100 * difficulty_random_size(difficulty));
                (*block).len = (int)(100 * difficulty_random_len(difficulty));
                SDL_Surface *surface_block = IMG_Load(generateRandomNumber());
                SDL_Surface *new_surface_block = SDL_CreateRGBSurfaceWithFormat(0,surface_block->w * (*block).size / 100,surface_block->h* (*block).size / 100,surface_block->format->BitsPerPixel,surface_block->format->format);
                SDL_BlitScaled(surface_block, NULL, new_surface_block, NULL);
                block->texture = SDL_CreateTextureFromSurface(app.renderer, new_surface_block);
                block->shape = shape;
                SDL_FreeSurface(surface_block);
                SDL_FreeSurface(new_surface_block);
                blocks_list.tail->after = block;
                blocks_list.tail = block;
                blocks_list.tail->after = NULL;
                blocks_list.tail->before = temp;
                blocks_list.middle = blocks_list.middle->after;
                temp = blocks_list.head;
                blocks_list.head = blocks_list.head->after;
                blocks_list.head->before = NULL;
                SDL_DestroyTexture(temp->texture);
                free(temp);
                do_score();
                printf("LAND\n");
                jumping = false;
                kun.h = 0;
                kun.dh = 0;
                if(blocks_list.middle->shape == 2){
                    Mix_PlayChannel(-1,chunk_tnt,0);
                    tnt_time = SDL_GetTicks();
                }else{
                    Mix_PlayChannel(-1,chunk_jump,0);
                }
                if(blocks_list.middle->shape == 3){
                    do_Moist();
                }
            }else if(blocks_list.middle->after->after->x <= 432.0 && blocks_list.middle->after->after->x + blocks_list.middle->after->after->size>= 358 && !fail){
                for (int i = 0; i < 2; ++i) {
                    Blocks *temp = blocks_list.tail;
                    block = malloc(sizeof(Blocks));
                    (*block).size = (int)(100 * difficulty_random_size(difficulty));
                    (*block).len = (int)(100 * difficulty_random_len(difficulty));
                    SDL_Surface *surface_block = IMG_Load(generateRandomNumber());
                    SDL_Surface *new_surface_block = SDL_CreateRGBSurfaceWithFormat(0,surface_block->w * (*block).size / 100,surface_block->h* (*block).size / 100,surface_block->format->BitsPerPixel,surface_block->format->format);
                    SDL_BlitScaled(surface_block, NULL, new_surface_block, NULL);
                    block->texture = SDL_CreateTextureFromSurface(app.renderer, new_surface_block);
                    block->shape = shape;
                    SDL_FreeSurface(surface_block);
                    SDL_FreeSurface(new_surface_block);
                    blocks_list.tail->after = block;
                    blocks_list.tail = block;
                    blocks_list.tail->after = NULL;
                    blocks_list.tail->before = temp;
                    blocks_list.middle = blocks_list.middle->after;
                    temp = blocks_list.head;
                    blocks_list.head = blocks_list.head->after;
                    blocks_list.head->before = NULL;
                    SDL_DestroyTexture(temp->texture);
                    free(temp);
                    do_score();
                }
                printf("LAND\n");
                jumping = false;
                kun.h = 0;
                kun.dh = 0;
                if(blocks_list.middle->shape == 2){
                    Mix_PlayChannel(-1,chunk_tnt,0);
                    tnt_time = SDL_GetTicks();
                }else{
                    Mix_PlayChannel(-1,chunk_jump,0);
                }
                if(blocks_list.middle->shape == 3){
                    do_Moist();
                }
            }else{
                printf("YOU LOSE\n");
                if(!fail){
                    fail = true;
                    Mix_PlayChannel(-1,chunk_fail,0);
                }
                if(kun.h <= -400){
                    kun.h = -400;
                    jumping = false;
                }
            }
        }
    }
}

static void draw_game(){
    SDL_RenderClear(app.renderer);

    display_image("./res/img/menu_background_1073x600.jpg",0,0);
    draw_blocks(blocks_list);

    SDL_Rect rect_middle = {.x = (int)((blocks_list.middle)->x), .y = speed_calculator_y(blocks_list.middle->shape,blocks_list.middle->size)};
    SDL_QueryTexture((blocks_list.middle)->texture, NULL, NULL, &rect_middle.w, &rect_middle.h);
    SDL_RenderCopy(app.renderer, (blocks_list.middle)->texture, NULL, &rect_middle);
    draw_kun(&kun);

    if(!fail){
        char scoreText[20];
        sprintf(scoreText, "Scores: %d", scores);
        display_font("./res/font/Gugi-Regular.ttf",scoreText,54,0,0,0,10,10);
        if(difficulty == 0){
            display_font("./res/font/Gugi-Regular.ttf","difficulty: Easy",54,0,0,0,10,80);
        }else if(difficulty == 1){
            display_font("./res/font/Gugi-Regular.ttf","difficulty: Medium",54,0,0,0,10,80);
        }else{
            display_font("./res/font/Gugi-Regular.ttf","difficulty: Hard",54,0,0,0,10,80);
        }
    }else{
        display_font("./res/font/Gugi-Regular.ttf","KUNKUN DIED!",72,0,0,0,120,100);

        char scoresText[20];
        sprintf(scoresText, "The scores is %d", scores);
        display_font("./res/font/Gugi-Regular.ttf",scoresText,54,0,0,0,120,220);
        display_font("./res/font/Gugi-Regular.ttf","press SPACE to try again",36,0,0,0,130,320);
    }

    if(gathering){
        SDL_Rect bgRect = { 348, 358, WIDTH + 4, 9};
        SDL_SetRenderDrawColor(app.renderer, 160, 160, 160, 255);
        SDL_RenderFillRect(app.renderer, &bgRect);
        int barWidth = (int)(percentage * WIDTH);
        SDL_Rect progressBarRect = { 350, 360, barWidth, 5};
        if(difficulty == 0){
            SDL_SetRenderDrawColor(app.renderer, 0, 153, 0, 255);
        }else if(difficulty == 1){
            SDL_SetRenderDrawColor(app.renderer, 0, 0, 204, 255);
        }else{
            SDL_SetRenderDrawColor(app.renderer, 204, 0, 0, 255);
        }

        SDL_RenderFillRect(app.renderer, &progressBarRect);
    }

    if(!jumping && !fail && !automatic && (SDL_GetTicks() - reminder) > 4000){
        const char* text1 = "Press and hold SPACE to jump";
        const char* text2 =  "Or press R for free jump mode";
        SDL_Color textColor = {102, 0, 102, 255};
        TTF_Font *font = TTF_OpenFont("./res/font/Peralta-Regular.ttf",36);
        SDL_Surface *textSurface1 = TTF_RenderText_Solid(font, text1, textColor);
        SDL_Surface *textSurface2 = TTF_RenderText_Solid(font, text2, textColor);
        SDL_Texture *textTexture1 = SDL_CreateTextureFromSurface(app.renderer, textSurface1);
        SDL_Texture *textTexture2 = SDL_CreateTextureFromSurface(app.renderer, textSurface2);
        double alpha = fabsf((sinf((float)((SDL_GetTicks() - tick) / 1500.0))));
        SDL_SetTextureAlphaMod(textTexture1, (int)(255 * alpha));
        SDL_SetTextureAlphaMod(textTexture2, (int)(255 * alpha));
        SDL_Rect rect1 = {.x = 90,.y = 200};
        SDL_QueryTexture(textTexture1,NULL,NULL,&rect1.w,&rect1.h);
        SDL_RenderCopy(app.renderer,textTexture1,NULL,&rect1);
        SDL_Rect rect2 = {.x = 90,.y = 250};
        SDL_QueryTexture(textTexture2,NULL,NULL,&rect2.w,&rect2.h);
        SDL_RenderCopy(app.renderer,textTexture2,NULL,&rect2);

        SDL_FreeSurface(textSurface1);
        SDL_FreeSurface(textSurface2);
        TTF_CloseFont(font);
    }

    if(automatic && !fail){
        const char* text1 = "Free Jump Mode";
        const char* text2 =  "press R to quit";
        SDL_Color textColor = {102, 0, 102, 255};
        TTF_Font *font = TTF_OpenFont("./res/font/Peralta-Regular.ttf",36);
        SDL_Surface *textSurface1 = TTF_RenderText_Solid(font, text1, textColor);
        SDL_Surface *textSurface2 = TTF_RenderText_Solid(font, text2, textColor);
        SDL_Texture *textTexture1 = SDL_CreateTextureFromSurface(app.renderer, textSurface1);
        SDL_Texture *textTexture2 = SDL_CreateTextureFromSurface(app.renderer, textSurface2);
        double alpha = fabsf((sinf((float)((SDL_GetTicks() - reminder) / 1500.0))));
        SDL_SetTextureAlphaMod(textTexture1, (int)(255 * alpha));
        SDL_SetTextureAlphaMod(textTexture2, (int)(255 * alpha));
        SDL_Rect rect1 = {.x = 200,.y = 200};
        SDL_QueryTexture(textTexture1,NULL,NULL,&rect1.w,&rect1.h);
        SDL_RenderCopy(app.renderer,textTexture1,NULL,&rect1);
        SDL_Rect rect2 = {.x = 200,.y = 250};
        SDL_QueryTexture(textTexture2,NULL,NULL,&rect2.w,&rect2.h);
        SDL_RenderCopy(app.renderer,textTexture2,NULL,&rect2);

        SDL_FreeSurface(textSurface1);
        SDL_FreeSurface(textSurface2);
        TTF_CloseFont(font);
    }

    SDL_RenderPresent(app.renderer);

    if(jumping){
        start_time = current_time;

        if (elapsed_time < ANIMATION_DELAY) {
            SDL_Delay(ANIMATION_DELAY - elapsed_time);
        }
    }

}

static void do_fps(){
    //todo
}

static double speed_calculator_h(Uint32 time){
    double speed;
    if(time <= LEN_CHUNK){
        speed = (double)time;
    }else{
        speed = LEN_CHUNK;
    }
    speed = 10 * sqrt(speed);
    return speed;
}

static void init_blocks(){
    Blocks *temp;

    block = malloc(sizeof(Blocks));
    (*block).size = (int)(100 * (((double)rand() / RAND_MAX) * (1.5 - 0.7) + 0.7));
    (*block).len = (int)(100 * (((double)rand() / RAND_MAX) * (2.5 - 0.7) + 0.7));
    SDL_Surface *surface_block1 = IMG_Load(generateRandomNumber());
    SDL_Surface *new_surface_block1 = SDL_CreateRGBSurfaceWithFormat(0,surface_block1->w * (*block).size / 100,surface_block1->h* (*block).size / 100,surface_block1->format->BitsPerPixel,surface_block1->format->format);
    SDL_BlitScaled(surface_block1, NULL, new_surface_block1, NULL);
    block->texture = SDL_CreateTextureFromSurface(app.renderer, new_surface_block1);
    block->shape = shape;
    SDL_FreeSurface(surface_block1);
    SDL_FreeSurface(new_surface_block1);

    block->before = NULL;
    blocks_list.head = block;
    temp = block;

    for (int i = 0; i < NUM_BLOCKS - 2; ++i) {
        block = malloc(sizeof(Blocks));
        (*block).size = (int)(100 * (((double)rand() / RAND_MAX) * (1.5 - 0.7) + 0.7));
        (*block).len = (int)(100 * (((double)rand() / RAND_MAX) * (2.5 - 0.7) + 0.7));
        SDL_Surface *surface_block = IMG_Load(generateRandomNumber());
        SDL_Surface *new_surface_block = SDL_CreateRGBSurfaceWithFormat(0,surface_block->w * (*block).size / 100,surface_block->h* (*block).size / 100,surface_block->format->BitsPerPixel,surface_block->format->format);
        SDL_BlitScaled(surface_block, NULL, new_surface_block, NULL);
        block->texture = SDL_CreateTextureFromSurface(app.renderer, new_surface_block);
        block->shape = shape;
        SDL_FreeSurface(surface_block);
        SDL_FreeSurface(new_surface_block);

        temp->after = block;
        block->before = temp;
        temp = block;
    }

    block = malloc(sizeof(Blocks));
    (*block).size = (int)(100 * (((double)rand() / RAND_MAX) * (1.5 - 0.7) + 0.7));
    (*block).len = (int)(100 * (((double)rand() / RAND_MAX) * (2.5 - 0.7) + 0.7));
    SDL_Surface *surface_block2 = IMG_Load(generateRandomNumber());
    SDL_Surface *new_surface_block2 = SDL_CreateRGBSurfaceWithFormat(0,surface_block2->w * (*block).size / 100,surface_block2->h* (*block).size / 100,surface_block2->format->BitsPerPixel,surface_block2->format->format);
    SDL_BlitScaled(surface_block2, NULL, new_surface_block2, NULL);
    block->texture = SDL_CreateTextureFromSurface(app.renderer, new_surface_block2);
    block->shape = shape;
    SDL_FreeSurface(surface_block2);
    SDL_FreeSurface(new_surface_block2);
    block->before = temp;
    block->after = NULL;
    temp->after = block;
    blocks_list.tail = block;

    blocks_list.middle = blocks_list.head;
    for (int i = 0; i < (NUM_BLOCKS) / 2; ++i) {
        blocks_list.middle = blocks_list.middle->after;
    }

    (blocks_list.middle)->x = 400 - 0.5 * (blocks_list.middle)->size;
}

static void quit_blocks(){
    Blocks *p = blocks_list.head;
    while (p != NULL) {
        SDL_DestroyTexture(p->texture);
        Blocks *temp = p;
        p = p->after;
        free(temp);
    }
}

void draw_blocks(LinkedList list){
    SDL_Rect rect_middle = {.x = (int)((list.middle)->x), .y = speed_calculator_y(list.middle->shape,list.middle->size)};
    SDL_QueryTexture((list.middle)->texture, NULL, NULL, &rect_middle.w, &rect_middle.h);
    SDL_RenderCopy(app.renderer, (list.middle)->texture, NULL, &rect_middle);


    for (Blocks *p = (list.middle)->before; p != NULL ; p = p->before) {
        p->x = (p->after)->x - (p->len+p->size);
        SDL_Rect rect = {.x = (int)(p->x), .y = speed_calculator_y(p->shape,p->size)};
        SDL_QueryTexture(p->texture, NULL, NULL, &rect.w, &rect.h);
        SDL_RenderCopy(app.renderer, p->texture, NULL, &rect);
    }

    for (Blocks *p = (list.middle)->after; p != NULL; p = p->after) {
        p->x = (p->before)->x + ((p->before)->len + (p->before)->size);
        SDL_Rect rect = {.x = (int)(p->x), .y = speed_calculator_y(p->shape,p->size)};
        SDL_QueryTexture(p->texture, NULL, NULL, &rect.w, &rect.h);
        SDL_RenderCopy(app.renderer, p->texture, NULL, &rect);
    }
}

char *generateRandomNumber() {
    int random = rand() % 20;
    if(difficulty < 2){
        if(random < 15){
            shape = 0;
            return block_path[rand() % 11];
        }else if(random < 18){
            shape = 1;
            return dish_path[rand() % 2];
        }else{
            shape = 3;
            return Moist[0];
        }
    }else{
        if(random < 10){
            shape = 0;
            return block_path[rand() % 11];
        }else if(random < 13){
            shape = 1;
            return dish_path[rand() % 2];
        }else if(random < 18){
            shape = 2;
            return tnt_path[0];
        }else{
            shape = 3;
            return Moist[0];
        }
    }
}

static int speed_calculator_y(int type,int size){
    if(type == 0 || type == 2 || type == 3){
        return (480 - (int)(size * 0.25));
    } else{
        return (480 - (int)(size * 0.5));
    }
}

static double difficulty_random_len(int diff){
    if(diff == 0){
        return ((double)rand() / RAND_MAX) * (2.5 - 0.7) + 0.7;
    }else if(diff == 1){
        return ((double)rand() / RAND_MAX) * (2.5 - 1.3) + 1.3;
    }else{
        return ((double)rand() / RAND_MAX) * (2.5 - 1.3) + 1.3;
    }
}

static double difficulty_random_size(int diff){
    if(diff == 0){
        return ((double)rand() / RAND_MAX) * (1.5 - 1.0) + 1.0;
    }else if(diff == 1){
        return ((double)rand() / RAND_MAX) * (1.2 - 0.7) + 0.7;
    }else{
        return ((double)rand() / RAND_MAX) * (1 - 0.5) + 0.5;
    }
}

static void do_auto(){
    double len = blocks_list.middle->x + blocks_list.middle->len + blocks_list.middle->size + blocks_list.middle->after->size * 0.5 - 400;
    double time_jump = len / KUN_SPEED;
    double speed_jump = time_jump * 0.5 * KUN_ACCELERATION;
    double gathering_time = speed_jump * speed_jump / 10.0 / 10.0;

    if(!gathering){
        tick = SDL_GetTicks();
        gathering = true;
        channel_1 = Mix_PlayChannel(-1,chunk_1,0);
    }else{
        if( (SDL_GetTicks() - tick) > LEN_CHUNK){
            channel_2 = Mix_PlayChannel(-1,chunk_2,-1);
        }
        if(percentage < 1){
            percentage = (SDL_GetTicks() - tick) / LEN_CHUNK;
        }else{
            percentage = 1;
        }
    }

    if((SDL_GetTicks() - tick) > gathering_time){
        Uint32 duration = SDL_GetTicks() - tick;
        tick =  SDL_GetTicks();
        Mix_FadeOutChannel(channel_1, 700);
        Mix_FadeOutChannel(channel_2, 10);
        printf("Press the space bar for %u\n",duration);
        gathering = false;
        kun.dh = speed_calculator_h(duration);
        jumping = true;
        percentage = 0;
        start_time = SDL_GetTicks();
        current_time = SDL_GetTicks();
    }
    if(blocks_list.middle->shape == 2 && (SDL_GetTicks() - tnt_time) > 3384){
        fail = true;
    }
}

static void do_score(){
    scores += (blocks_list.middle->before->len) / 10;
    if(blocks_list.middle->size < 80){
        scores += 20;
    }
    if(blocks_list.middle->before->shape == 2){
        scores += 50;
    }
}

static void do_Moist(){
    SDL_Surface *surface = IMG_Load(Dirt[0]);
    SDL_Surface *new_surface = SDL_CreateRGBSurfaceWithFormat(0,surface->w * blocks_list.middle->size / 100,surface->h *  blocks_list.middle->size / 100,surface->format->BitsPerPixel,surface->format->format);
    SDL_BlitScaled(surface, NULL, new_surface, NULL);
    blocks_list.middle->texture = SDL_CreateTextureFromSurface(app.renderer, new_surface);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(new_surface);
}
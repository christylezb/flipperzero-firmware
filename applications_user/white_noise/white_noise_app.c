#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <audio/audio.h>

typedef struct {
    bool is_playing;
    uint8_t volume; // Range: 0-100
} WhiteNoiseState;

void generate_white_noise(Audio* audio, uint8_t volume) {
    int16_t sample;
    for(size_t i = 0; i < BUFFER_SIZE; i++) {
        sample = (rand() % 65536) - 32768; // Generate random sample
        sample = (sample * volume) / 100; // Apply volume
        audio_write_sample(audio, sample);
    }
}

void input_callback(InputEvent* event, void* ctx) {
    WhiteNoiseState* state = ctx;
    if(event->type == InputTypePress) {
        switch(event->key) {
            case InputKeyUp:
                if(state->volume < 100) state->volume += 10;
                break;
            case InputKeyDown:
                if(state->volume > 0) state->volume -= 10;
                break;
            case InputKeyOk:
                state->is_playing = !state->is_playing;
                break;
            default:
                break;
        }
    }
}

int32_t white_noise_app(void* p) {
    WhiteNoiseState state = {.is_playing = false, .volume = 50};
    Gui* gui = furi_record_open(RECORD_GUI);
    Input* input = furi_record_open(RECORD_INPUT);
    Audio* audio = furi_record_open(RECORD_AUDIO);

    input_set_callback(input, input_callback, &state);

    while(1) {
        if(state.is_playing) {
            generate_white_noise(audio, state.volume);
        } else {
            furi_delay_ms(100);
        }
    }

    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_INPUT);
    furi_record_close(RECORD_AUDIO);
    return 0;
}
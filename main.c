#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#define PI_2    6.283185307f


float sin_step = 0;
float sin_pos = 0;
float frequency;
float volume = 25000;
float duration;

void print_help()
{
    printf( "tone_generator [-f frequency] [-d duration]\n");
    printf( "frequency : in Hz, must be > 0\n");
    printf( "duration  : in seconds must be > 0\n ");
}

void sin_generator(  void* data, Uint8* stream, int bytes_count )
{
    sin_step = 1.f / 48000.f;

    int16_t* samples = (int16_t*) stream;
    int samples_count = bytes_count / 2;

    for( int i = 0; i < samples_count; ++i )
    {
        // https://en.wikipedia.org/wiki/Radian_per_second
        samples[i] = volume * sin( sin_pos * PI_2 * frequency );
        sin_pos += sin_step;
    }
}

int main( int argc, char** argv )
{
    int r;

    frequency = 0;
    duration = 0;

    while( (r = getopt(argc, argv, "d:f:h")) != -1 )
    {
        switch( r )
        {
            case 'd':
                duration = atoi( optarg );
                break;

            case 'f':
                frequency = atof( optarg );
                break;

            case 'v':
                volume = atoi( optarg );
                break;                

            case 'h':
            default:                
                print_help();
                return 0;


       }
    }

    if( (0 == frequency) || (0 == duration) ) 
    {
        print_help();        
        return 0;
    }

    r = SDL_Init( SDL_INIT_AUDIO );
    if( r != 0 )
    {
        fprintf( stderr, "Unable to initialize SDL\n");
        return -1;
    }



    SDL_AudioSpec specs[2];
    memset( specs, 0, sizeof(SDL_AudioSpec) * 2 );



    specs[0].freq = 48000 ;
    specs[0].format = AUDIO_S16;
    specs[0].channels = 1;
    specs[0].samples = 4096;
    specs[0].callback = sin_generator;


    SDL_AudioDeviceID dev;

    dev = SDL_OpenAudioDevice( NULL, 0, &specs[0], &specs[1], 0 );
    if( dev == 0 )
    {
        fprintf( stderr, "Unable to open audio device\n" );
        return -1;
    }



    SDL_PauseAudioDevice( dev, 0 );
    SDL_Delay( duration * 1000. );
    SDL_CloseAudioDevice( dev );

    SDL_Quit();

    return 0;
}

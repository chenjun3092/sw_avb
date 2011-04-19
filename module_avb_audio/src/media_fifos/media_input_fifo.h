 /*
 * @ModuleName Audio DAC/LOCAL_TALKER_STREAM FIFO Defines.
 *
 *
 */
 
#ifndef _LOCAL_TALKER_STREAM_H_ 
#define _LOCAL_TALKER_STREAM_H_ 1
#include "avb_conf.h"

#ifndef AVB_MAX_AUDIO_SAMPLE_RATE
#define AVB_MAX_AUDIO_SAMPLE_RATE (48000)
#endif

#ifndef MEDIA_INPUT_FIFO_SAMPLE_FIFO_SIZE
#define GET_SIZE(x) (x == 44100 || x == 48000) ? 64 : 128
#define MEDIA_INPUT_FIFO_SAMPLE_FIFO_SIZE (GET_SIZE(AVB_MAX_AUDIO_SAMPLE_RATE))
#endif

typedef struct ififo_t {
  int wrIndex;				//!< Current write pointer
  int rdIndex;				//!< Current read pointer
  int startIndex;			//!< The pointer for the start of the packet being written
  int packetSize;			//!< Size of the packet, exclusing the Timestamp and DBC value.
  int sampleCountInPacket;	//!< The current number of words in the FIFO (samples only)
  unsigned int dbc;			//!< The Data Block Count from IEC.61883
  int fifoEnd;				//!< The index of the end of the FIFO
  int ptr;					//!< The read pointer used by the packet reader in the talker
  unsigned int buf[MEDIA_INPUT_FIFO_SAMPLE_FIFO_SIZE];
} ififo_t;

/** This type provides the data structure used by a media input fifo.
 */
typedef struct ififo_t media_input_fifo_data_t;

/**
 * This type provides a handle to a media input fifo.
 **/
typedef int media_input_fifo_t;

/**
 *  \brief Push a sample into the FIFO
 *
 *  If the packet of samples has just begun then a timestamp and data
 *  block count are added along with the first sample.  If the last sample
 *  of a packet has been written then the pointer to the start of the
 *  packet is updated.
 */
void 
media_input_fifo_push_sample(media_input_fifo_t media_input_fifo,
                             unsigned int sample,
                             unsigned int ts);

#ifndef __XC__
/**
 *  \brief Perform a blocking read of the next audio packet
 *
 *  \param media_input_fifo the media fifo
 */
unsigned int *
media_input_fifo_get_packet(media_input_fifo_t media_input_fifo,
                               unsigned int *ts,
                               unsigned int *dbc);
#endif
                             
void 
media_input_fifo_release_packet(media_input_fifo_t media_input_fifos);

void
media_input_fifo_init(media_input_fifo_t media_input_fifo,
                      int rate);

/**
 *  \brief Start storing samples/packets in the FIFO
 *
 *  \param media_input_fifo the media fifo
 *  \param the rate in Hz
 *  \return the number of samples in each audio packet
 */
int
media_input_fifo_enable(media_input_fifo_t media_input_fifo, int rate);

/**
 *  \brief Stop storing samples/packets in the FIFO
 *
 *  \param media_input_fifo the media fifo
 */
void
media_input_fifo_disable(media_input_fifo_t media_input_fifo);

/** Initialize media input fifos.
 *
 *  This function intializes media input FIFOs and ties the handles
 *  to their associated data structures. It should be called before the main 
 *  component function on a thread to setup the FIFOs.
 * 
 *  \param ififos      an array of media input FIFO handles to initialize
 *  \param ififo_data  an array of associated data structures
 *  \param n           the number of FIFOs to initialize
 **/
void
init_media_input_fifos(media_input_fifo_t ififos[],
                       media_input_fifo_data_t ififo_data[],
                       int n);
                       

/**
 *  \brief Get the FIFO for the given stream number
 */
media_input_fifo_t get_media_input_fifo(int stream_num);

/**
 *  \brief Check if the FIFO is empty
 */
int media_input_fifo_empty(media_input_fifo_t media_input_fifo0);



#ifndef __XC__

inline void media_input_fifo_set_ptr(media_input_fifo_t media_input_fifo0,
                                     int *p)
{
  volatile ififo_t *media_input_fifo =  (ififo_t *) media_input_fifo0;
  media_input_fifo->ptr = (int) p;
  return;
}

inline int *media_input_fifo_get_ptr(media_input_fifo_t media_input_fifo0)
{
  volatile ififo_t *media_input_fifo =  (ififo_t *) media_input_fifo0;
  return (int *) (media_input_fifo->ptr);
}
#endif

#endif

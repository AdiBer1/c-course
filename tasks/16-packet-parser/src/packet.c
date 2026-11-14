
#include "packet.h"

void pkt_init(pkt_parser_t *p)
{
    if (p == NULL) {
        return;
    }

    p->state = PKT_STATE_WAIT_START; //init status
    p->len = 0u;                     //init length
    p->index = 0u;                   //init index
    p->checksum = 0u;                //init checksum
    p->done_len = 0u;                //init done length
    p->errors = 0u;                  //init error count
}

pkt_result_t pkt_feed(pkt_parser_t *p, uint8_t byte)
{
    if (p == NULL) { //null error
        return PKT_NONE;
    }

    switch (p->state) { //this is the state machine for the packet parser

    case PKT_STATE_WAIT_START: //init state, waiting for the start byte
        if (byte == PKT_START) {
            p->state = PKT_STATE_LEN;
        }
        break;

    case PKT_STATE_LEN: //waiting for the length byte
        if (byte > PKT_MAX_PAYLOAD) {
            p->errors++;
            p->state = PKT_STATE_WAIT_START;
            return PKT_ERR_LENGTH;
        }

        p->len = byte;
        p->index = 0u;
        p->checksum = byte;

        if (p->len
            == 0u) { //if length is 0, go to checksum state, an empty payload is valid but nothing realy happens
            p->state = PKT_STATE_CHECKSUM;
        } else {
            p->state = PKT_STATE_PAYLOAD;
        }
        break;

    case PKT_STATE_PAYLOAD: //add the payload according to the length, and calculate the checksum
        p->payload[p->index] = byte; //the current place
        p->checksum =
            (uint8_t)(p->checksum
                      ^ byte); //the uint8_t to avoid -Wconversion, checksum is calculated by XORing the length and all payload bytes together,
        p->index++;

        if (p->index == p->len) { //the last payload expected, go to checksum state
            p->state = PKT_STATE_CHECKSUM;
        }
        break;

    case PKT_STATE_CHECKSUM: //check if we got the the end
        p->state = PKT_STATE_WAIT_START;

        if (byte != p->checksum) {
            p->errors++;
            return PKT_ERR_CHECKSUM;
        }

        p->done_len = p->len;

        for (uint8_t i = 0u; i < p->len; i++) { //copy the payload to the done buffer
            p->done[i] = p->payload[i];
        }

        return PKT_COMPLETE;

    default: //should never happen by mistake, but if it does, reset the state machine
        p->state = PKT_STATE_WAIT_START;
        break;
    }

    return PKT_NONE;
}

const uint8_t *pkt_payload(const pkt_parser_t *p, uint8_t *len_out)
{
    if (p == NULL || len_out == NULL) {
        return NULL;
    }

    *len_out = p->done_len;
    return p->done;
}

uint32_t pkt_error_count(const pkt_parser_t *p)
{
    if (p == NULL) {
        return 0u;
    }

    return p->errors;
}

size_t pkt_build(uint8_t *out, size_t out_size, const uint8_t *payload, uint8_t len)
{
    size_t frame_size = (size_t)len + 3u;
    uint8_t checksum = len;

    if (out == NULL || out_size < frame_size || len > PKT_MAX_PAYLOAD
        || (payload == NULL && len > 0u)) {
        return 0u;
    }

    out[0] = (uint8_t)PKT_START;
    out[1] = len;

    for (size_t i = 0u; i < (size_t)len; i++) {
        out[i + 2u] = payload[i];
        checksum = (uint8_t)(checksum ^ payload[i]);
    }

    out[frame_size - 1u] = checksum;

    return frame_size;
}
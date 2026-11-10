#include "packet.h"

/* TODO: implement the state machine and the helpers. */

void pkt_init(pkt_parser_t *p)
{
    if (p == NULL) {
        return;
    }
    p->state = PKT_STATE_WAIT_START;
    p->len = 0;
    p->index = 0;
    p->checksum = 0;
    p->done_len = 0;
    p->errors = 0;
}

pkt_result_t pkt_feed(pkt_parser_t *p, uint8_t byte)
{
    (void)p;
    (void)byte;
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
        return 0;
    }
    return p->errors;
}

size_t pkt_build(uint8_t *out, size_t out_size, const uint8_t *payload, uint8_t len)
{
    (void)out;
    (void)out_size;
    (void)payload;
    (void)len;
    return 0;
}

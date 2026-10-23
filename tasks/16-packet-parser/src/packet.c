#include "packet.h"

/* TODO: implement the state machine and the helpers. */

void pkt_init(pkt_parser_t *p)
{
    (void)p;
}

pkt_result_t pkt_feed(pkt_parser_t *p, uint8_t byte)
{
    (void)p;
    (void)byte;
    return PKT_NONE;
}

const uint8_t *pkt_payload(const pkt_parser_t *p, uint8_t *len_out)
{
    (void)p;
    (void)len_out;
    return NULL;
}

uint32_t pkt_error_count(const pkt_parser_t *p)
{
    (void)p;
    return 0;
}

size_t pkt_build(uint8_t *out, size_t out_size, const uint8_t *payload, uint8_t len)
{
    (void)out;
    (void)out_size;
    (void)payload;
    (void)len;
    return 0;
}
